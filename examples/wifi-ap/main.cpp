#include <Arduino.h>
#include "Qt.h"
#include "WebServer.h"
#include <FS.h>
#include <LittleFS.h>
#include <FFat.h> // or.. FAT
#include <WiFi.h>
#include <ArduinoJson.h>
#include "pin_config.h"

#define CHANNEL 1
#define HOSTNAME "webserver"

WebServer server(80);
String state("off");
Qt qt;
fs::FS *fsys = nullptr;

void sendStateResponse()
{
  JsonDocument root;
  root["state"] = state;

  char output[256];
  serializeJson(root, output);

  server.sendHeader("Cache-Control", "no-cache");
  server.send(200, "application/json", output);
}

void handleOn()
{
  state = "on";
  qt.setBreathingLight(0);
  Serial.println("BreathingLight on");
  sendStateResponse();
}

void handleOff()
{
  state = "off";
  qt.setBreathingLight(255);
  Serial.println("BreathingLight off");
  sendStateResponse();
}

void handleRedirect()
{
  Serial.println("Redirect...");
  String url = "/index.html";
  // TODO remove?
  if (!fsys->exists(url))
  {
    url = "/$upload.htm";
  }

  server.sendHeader("Location", url, true);
  server.send(302);
}

static const char notFoundContent[] PROGMEM = R"==(
  <html>
  <head>
    <title>Resource not found</title>
  </head>
  <body>
    <p>The resource was not found.</p>
    <p><a href="/">Start again</a></p>
  </body>
  )==";

void handleRoot()
{
  File file = LittleFS.open("/index.html", "r");
  if (!file)
  {
    server.send(404, "text/html", FPSTR(notFoundContent));
    return;
  }

  String content(file.readString());
  content.replace("%STATE%", state);

  server.send(200, "text/html", content);
  file.close();
}

void setup()
{
  qt.begin();

  qt.setupFs();

  // Go into software AP mode.
  // TODO: use wifimanager
  qt.setupAp("T-QT-C6", "qtqtpasspass", WIFI_AUTH_WPA3_PSK);

  fsys = &LittleFS;

  Serial.println("Mounting the filesystem...");
  if (!fsys)
  {
    Serial.println("need to change the board configuration to include a partition for files.");
    delay(30000);
  }
  else if ((fsys == (fs::FS *)&FFat) && (!FFat.begin()))
  {
    Serial.println("could not mount the filesystem...");
    delay(2000);
    Serial.println("formatting FAT...");
    FFat.format();
    delay(2000);
    Serial.println("restarting...");
    delay(2000);
    ESP.restart();
  }
  else if ((fsys == (fs::FS *)&LittleFS) && (!LittleFS.begin()))
  {
    Serial.println("could not mount the filesystem...");
    delay(2000);
    Serial.println("formatting LittleFS...");
    LittleFS.format();
    delay(2000);
    Serial.println("restarting...");
    delay(2000);
    ESP.restart();
  }

  server.on("/", HTTP_GET, handleRoot);
  server.enableCORS(true);

  // API
  server.on("/api/on", HTTP_GET, handleOn);
  server.on("/api/off", HTTP_GET, handleOff);

  // serve all static files
  server.serveStatic("/", *fsys, "/");
  Serial.println("Register default (not found) answer...\n");

  // handle cases when file is not found
  server.onNotFound([]()
                    {
    // standard not found in browser.
    //TODO: use 404.html file
    server.send(404, "text/html", FPSTR(notFoundContent)); });

  // Start server
  server.begin();

  Serial.println("App served on port 80");
}

void loop()
{
  server.handleClient();
}