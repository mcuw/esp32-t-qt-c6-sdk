#include <Arduino.h>
#include <WiFi.h>
#include <DebugUtils.h>

// pinouts: https://github.com/espressif/arduino-esp32/blob/master/variants/esp32c6/pins_arduino.h

void setup()
{
  serialBegin(115200);
}

void loop()
{
  serialPrintln("loop");
  delay(1000);
}