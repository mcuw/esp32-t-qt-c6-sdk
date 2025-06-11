#include <Arduino.h>
#include <DebugUtils.h>
#include "Arduino_DriveBus_Library.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "pin_config.h"
#include "Qt.h"

#define MEASUREMENT_INTERVAL 2000
#define STACK_SIZE 2048

Qt qt;

// non blocking loop with a task
void batteryVoltageTask(void *pvParameters)
{
  for (;;)
  {
    Serial.print("ADC Value:");
    Serial.println(analogRead(BATTERY_ADC_DATA));

    Serial.printf("ADC Voltage: %.03f V\n", ((float)analogReadMilliVolts(BATTERY_ADC_DATA)) / 1000.0);

    Serial.printf("Battery Voltage: %.03f V\n", qt.getBatteryVoltage());

    vTaskDelay(pdMS_TO_TICKS(MEASUREMENT_INTERVAL)); // wait 1000 ms
  }
}

void setup()
{
  Serial.begin(115200);

  qt.begin();
  qt.setBatteryMessurement(true);

  xTaskCreate(
      batteryVoltageTask,      // Task function
      "battery voltage check", // Name of the task
      STACK_SIZE,              // Stack size in bytes
      NULL,                    // Parameters to pass (not in use here)
      1,                       // Task priority (1 is the default level)
      NULL                     // Task handle (not in use here)
  );
}

void loop()
{
  Serial.println("Messurement task is running");
  vTaskDelete(NULL); // deletes the loop if not needed otherwise remove this line
}