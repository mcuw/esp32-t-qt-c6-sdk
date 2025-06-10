#include <Arduino.h>
#include "WebServer.h"
#include <FS.h>
#include <LittleFS.h>
#include <FFat.h> // or.. FAT
#include <WiFi.h>
#include <ArduinoJson.h> // https://arduinojson.org
#include "pin_config.h"
#include "Qt.h"

#define CHANNEL 1
#define HOSTNAME "webserver"
#define LIGHT_ON 0
#define LIGHT_OFF 255

WebServer server(80);
uint8_t state = LIGHT_OFF;
Qt qt;

void sendStateResponse()
{
  JsonDocument root;
  root["state"] = state;

  char output[14];
  serializeJson(root, output);

  server.sendHeader("Cache-Control", "no-cache");
  server.send(200, "application/json", output);
}

void handleOn()
{
  state = LIGHT_ON;
  qt.setBreathingLight(state);
  sendStateResponse();
}

void handleOff()
{
  state = LIGHT_OFF;
  qt.setBreathingLight(state);
  sendStateResponse();
}

void handleState()
{
  sendStateResponse();
}

void handleStateChanged()
{
  JsonDocument doc;
  deserializeJson(doc, server.arg(0));

  state = doc["state"];
  qt.setBreathingLight(state);

  server.send(204);
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
  char stateString[4];
  sprintf(stateString, "%d", state);
  content.replace("%STATE%", stateString);

  server.send(200, "text/html", content);
  file.close();
}

void handleNotFound()
{
  // standard not found in browser
  // TODO: use 404.html file
  server.send(404, "text/html", FPSTR(notFoundContent));
}

void setup()
{
  qt.begin();

  qt.setupFs();

  // Go into software AP mode.
  // TODO: use wifimanager
  qt.setupAp("T-QT-C6", "qtqtpasspass", WIFI_AUTH_WPA3_PSK);

  server.enableCORS(true);

  server.on("/", HTTP_GET, handleRoot);

  // API
  server.on("/api/on", HTTP_GET, handleOn);
  server.on("/api/off", HTTP_GET, handleOff);
  server.on("/api/state", HTTP_GET, handleState);
  server.on("/api/state-changed", HTTP_POST, handleStateChanged);

  // serve all static files - have to declare after the API routes
  server.serveStatic("/", LittleFS, "/");

  // handle cases when path/ file is not found
  server.onNotFound(handleNotFound);

  // Start server
  server.begin();

  Serial.println("App served on port 80");
}

void loop()
{
  server.handleClient();
}