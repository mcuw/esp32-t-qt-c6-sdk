#include <Arduino.h>
#include "Qt.h"

#define CHANNEL 1

Qt qt;

void setup()
{
  // Go into software AP mode.
  qt.startAP("T-QT-C6", NULL, WIFI_AUTH_WPA3_PSK);
}

void loop()
{
  delay(200);
}