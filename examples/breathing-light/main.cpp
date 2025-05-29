#include <Arduino.h>
#include <DebugUtils.h>
#include "Arduino_DriveBus_Library.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "pin_config.h"
#include "Qt.h"

#define BREATHING_INTERVAL 1000

Qt qt;

// non blocking loop with a task
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

void setup()
{
  serialBegin(115200);

  qt.begin();

  xTaskCreate(
      breathingTask,   // Task function
      "BreathingTask", // Name of the task
      512,             // Stack size in bytes
      NULL,            // Parameters to pass (not in use here)
      1,               // Task priority (1 is the default level)
      NULL             // Task handle (not in use here)
  );
}

void loop()
{
  Serial.println("Breathing Task is running");
  vTaskDelete(NULL); // deletes the loop if not needed otherwise remove this line
}