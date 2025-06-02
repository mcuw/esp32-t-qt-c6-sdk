#include <Arduino.h>
#include <DebugUtils.h>
// #include "Arduino_GFX_Library.h"
// #include "Arduino_DriveBus_Library.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "pin_config.h"
#include "QtDev.h"
#include <Arduino_GFX.h>

#define STACK_SIZE 2048
#define REFRESH_INTERVAL 100

Qt qt;

void gfxTask(void *pvParameters)
{
  for (;;)
  {
    // RGB
    qt.fillScreenGfx(RED);
    qt.setTextSizeGfx(3);
    qt.printfGfx(WHITE, 40, 50, "RGB");
    qt.setTextSizeGfx(2);
    qt.printfGfx(WHITE, 50, 74, "red");
    vTaskDelay(pdMS_TO_TICKS(4000));

    qt.fillScreenGfx(GREEN);
    qt.setTextSizeGfx(3);
    qt.printfGfx(BLACK, 40, 50, "RGB");
    qt.setTextSizeGfx(2);
    qt.printfGfx(BLACK, 40, 74, "green");
    vTaskDelay(pdMS_TO_TICKS(2000));

    qt.fillScreenGfx(BLUE);
    qt.setTextSizeGfx(3);
    qt.printfGfx(WHITE, 40, 50, "RGB");
    qt.setTextSizeGfx(2);
    qt.printfGfx(WHITE, 40, 74, "blue");
    vTaskDelay(pdMS_TO_TICKS(2000));

    qt.fillScreenGfx(BLACK);
    qt.printfGfx(WHITE, 36, 50, "B/W");
    qt.printfGfx(WHITE, 36, 70, "Black");
    vTaskDelay(pdMS_TO_TICKS(4000));
    qt.fillScreenGfx(WHITE);
    qt.printfGfx(BLACK, 36, 50, "B/W");
    qt.printfGfx(BLACK, 36, 70, "White");
    vTaskDelay(pdMS_TO_TICKS(4000));

    qt.fillScreenGfx(NAVY);
    qt.printfGfx(WHITE, 40, 50, "NAVY");
    vTaskDelay(pdMS_TO_TICKS(2000));

    qt.fillScreenGfx(DARKGREEN);
    qt.printfGfx(WHITE, 14, 50, "DARKGREEN");
    vTaskDelay(pdMS_TO_TICKS(2000));

    qt.fillScreenGfx(DARKCYAN);
    qt.printfGfx(WHITE, 14, 50, "DARKCYAN");
    vTaskDelay(pdMS_TO_TICKS(2000));

    qt.fillScreenGfx(MAROON);
    qt.printfGfx(WHITE, 20, 50, "MAROON");
    vTaskDelay(pdMS_TO_TICKS(2000));

    qt.fillScreenGfx(PURPLE);
    qt.printfGfx(WHITE, 14, 50, "PURPLE");
    vTaskDelay(pdMS_TO_TICKS(2000));

    qt.fillScreenGfx(OLIVE);
    qt.printfGfx(WHITE, 14, 50, "OLIVE");
    vTaskDelay(pdMS_TO_TICKS(2000));

    qt.fillScreenGfx(LIGHTGREY);
    qt.printfGfx(WHITE, 10, 50, "LIGHTGREY");
    vTaskDelay(pdMS_TO_TICKS(2000));

    qt.fillScreenGfx(DARKGREY);
    qt.printfGfx(WHITE, 14, 50, "DARKGREY");
    vTaskDelay(pdMS_TO_TICKS(2000));

    qt.fillScreenGfx(CYAN);
    qt.setTextSizeGfx(3);
    qt.printfGfx(BLACK, 30, 50, "CYAN");
    qt.setTextSizeGfx(2);
    vTaskDelay(pdMS_TO_TICKS(2000));

    qt.fillScreenGfx(MAGENTA);
    qt.printfGfx(WHITE, 14, 50, "MAGENTA");
    vTaskDelay(pdMS_TO_TICKS(2000));

    qt.fillScreenGfx(YELLOW);
    qt.printfGfx(BLUE, 14, 50, "YELLOW");
    vTaskDelay(pdMS_TO_TICKS(2000));

    qt.fillScreenGfx(ORANGE);
    qt.printfGfx(WHITE, 14, 50, "ORANGE");
    vTaskDelay(pdMS_TO_TICKS(2000));

    qt.fillScreenGfx(GREENYELLOW);
    qt.printfGfx(BLACK, 20, 40, "Green-");
    qt.printfGfx(BLACK, 20, 60, "Yellow");
    vTaskDelay(pdMS_TO_TICKS(2000));

    qt.fillScreenGfx(PALERED);
    qt.printfGfx(WHITE, 14, 50, "PALERED");
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

void setup()
{
  serialBegin(115200);

  qt.begin();
  qt.setBacklight(0);

  xTaskCreate(
      gfxTask,    // Task function
      "gfx task", // Name of the task
      STACK_SIZE, // Stack size in bytes
      NULL,       // Parameters to pass (not in use here)
      1,          // Task priority (1 is the default level)
      NULL        // Task handle (not in use here)
  );
}

void loop()
{
  Serial.println("gfx task is running");
  vTaskDelete(NULL); // deletes the loop if not needed otherwise remove this line
}