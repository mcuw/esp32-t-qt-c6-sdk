#include <Arduino.h>
#include <DebugUtils.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "pin_config.h"
#include "Qt.h"
#include <Arduino_GFX.h>

#define STACK_SIZE 1024 * 5
#define REFRESH_INTERVAL 100

// rounded edges
#define SAFE_Y_TOP 14
#define SAFE_Y_BOTTOM 98

Qt qt;

void touchGfxTask(void *pvParameters)
{
  TouchState state;
  qt.setTextSizeGfx(2);

  for (;;)
  {
    state = qt.getTouch();
    if (state.touched)
    {
      qt.fillScreenGfx(BLACK);

      if (state.gesture == "Swipe Up")
      {
        qt.printfGfx(WHITE, 0, SAFE_Y_TOP, "Swipe Up");
        Serial.println("Gesture: Swipe Up");
      }
      else if (state.gesture == "Swipe Down")
      {
        qt.printfGfx(WHITE, 0, SAFE_Y_TOP, "Swipe Down");
        Serial.println("Gesture: Swipe Down");
      }
      else
      {
        // more gestures "Swipe Right", "Swipe Left" and
        // these are w/o finger numbers: "Single Click", "Double Click"
        qt.printfGfx(WHITE, 0, SAFE_Y_TOP, state.gesture.c_str());
        Serial.printf("Gesture: %s\n", state.gesture);
      }

      qt.printfGfx(WHITE, 0, 46, "Touch");
      qt.printfGfx(WHITE, 0, 66, "x:%d y:%d", state.x, state.y);
      Serial.printf("Touch X:%d Y:%d\n", state.x, state.y);

      qt.printfGfx(WHITE, 0, SAFE_Y_BOTTOM, "Fingers: %d", state.fingers);
      Serial.printf("Fingers number: %d\n", state.fingers); // 0 on clicks otherwise 1

      vTaskDelay(pdMS_TO_TICKS(REFRESH_INTERVAL)); // wait 10 ms
    }
  }
}

void setup()
{
  serialBegin(115200);

  qt.begin();
  qt.setBacklight(0); // activate maximal backlight

  xTaskCreate(
      touchGfxTask,     // Task function
      "touch gfx task", // Name of the task
      STACK_SIZE,       // Stack size in bytes
      NULL,             // Parameters to pass (not in use here)
      1,                // Task priority (1 is the default level)
      NULL              // Task handle (not in use here)
  );
}

void loop()
{
  Serial.println("touch gfx task is running");
  vTaskDelete(NULL); // deletes the loop if not needed otherwise remove this line
}