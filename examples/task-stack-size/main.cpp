#include <Arduino.h>
#include <DebugUtils.h>
#include "Arduino_DriveBus_Library.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "pin_config.h"
#include "Qt.h"

#define BREATHING_INTERVAL 1000
#define STACK_SIZE 1024

TaskHandle_t taskHandle;
Qt qt;

void breathingTask(void *pvParameters)
{
  for (;;)
  {
    for (int i = 255; i > 0; i--)
    {
      ledcWrite(BREATHING_LIGHT, i);
      vTaskDelay(pdMS_TO_TICKS(2)); // wait 2 ms
    }

    for (int i = 0; i < 255; i++)
    {
      ledcWrite(BREATHING_LIGHT, i);
      vTaskDelay(pdMS_TO_TICKS(2)); // wait 2 ms
    }

    vTaskDelay(pdMS_TO_TICKS(BREATHING_INTERVAL)); // wait 1000 ms
  }
}

void printWatermark(void *pvParameters)
{
  for (;;)
  {
    delay(2000);
    Serial.print("TASK: ");
    Serial.print(pcTaskGetName(taskHandle)); // Get task name with handler
    Serial.print(", needs more than [bytes]: ");
    Serial.print(STACK_SIZE - uxTaskGetStackHighWaterMark(taskHandle));
    Serial.println();
  }
}

void setup()
{
  serialBegin(115200);

  qt.begin();

  xTaskCreate(
      breathingTask,     // Task function
      "Breathing Light", // Name of the task
      STACK_SIZE,        // Stack size in bytes
      NULL,              // Parameters to pass
      1,                 // Task priority (1 is the default level)
      &taskHandle        // Task handle
  );

  xTaskCreate(
      printWatermark,    // Task function
      "Print Watermark", // Name of the task (e.g. for debugging)
      2048,              // Stack size in bytes
      NULL,              // Parameter to pass
      1,                 // Task priority (1 is the default level)
      NULL               // Task handle
  );
}

void loop()
{
  Serial.println("Breathing Task is running");
  vTaskDelete(NULL); // deletes the loop if not needed otherwise remove this line
}