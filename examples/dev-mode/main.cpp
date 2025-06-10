#include <Arduino.h>
#include <DebugUtils.h>
#include "Arduino_DriveBus_Library.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "pin_config.h"
#include "QtDev.h"

QtDev qt;

void setup()
{
  Serial.begin(115200);

  qt.begin();
}

void loop()
{
  Serial.println("QtDev: dev mode with all features");
  delay(2000);
}