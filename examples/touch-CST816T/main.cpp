#include <Arduino.h>
#include <DebugUtils.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "pin_config.h"
#include "Qt.h"

// disconnect the ring when using the Serial Debug
// otherwise the touch initialization fails

// Supports
// up sliding, down sliding,
// left sliding, right sliding,
// double clicking, clicking, and long pressing gestures for triggering,
// as well as a combination of multiple interrupt triggering methods.
// By default, it automatically enters sleep power-saving mode after a few seconds without touch

#define STACK_SIZE 2048
#define MEASUREMENT_INTERVAL 100

Qt qt;

void touchTask(void *pvParameters)
{
  TouchState state;

  for (;;)
  {
    state = qt.getTouch();
    if (state.touched)
    {
      if (state.gesture == "Swipe Up")
      {
        Serial.println("Gesture: Swipe Up");
      }
      else if (state.gesture == "Swipe Down")
      {
        Serial.println("Gesture: Swipe Down");
      }
      else
      {
        Serial.printf("Gesture: %s\n", state.gesture);
      }

      Serial.printf("Touch X:%d Y:%d\n", state.x, state.y);

      Serial.printf("Fingers number: %d\n\n", state.fingers); // 0 on clicks otherwise 1

      vTaskDelay(pdMS_TO_TICKS(MEASUREMENT_INTERVAL)); // wait 100 ms
    }
  }
}

void setup()
{
  serialBegin(115200);

  qt.begin();

  xTaskCreate(
      touchTask,     // Task function
      "touch check", // Name of the task
      STACK_SIZE,    // Stack size in bytes
      NULL,          // Parameters to pass (not in use here)
      1,             // Task priority (1 is the default level)
      NULL           // Task handle (not in use here)
  );
}

void loop()
{
  Serial.println("Touch sensor task is running");
  vTaskDelete(NULL); // deletes the loop if not needed otherwise remove this line
}