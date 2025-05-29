#include <Arduino.h>
#include "Arduino_DriveBus_Library.h"
#include <DebugUtils.h>
#include <pin_config.h>

// see https://github.com/Xinyuan-LilyGO/T-QT-C6
std::shared_ptr<Arduino_IIC_DriveBus> IIC_Bus =
    std::make_shared<Arduino_HWIIC>(IIC_SDA, IIC_SCL, &Wire);

std::unique_ptr<Arduino_IIC> ETA4662(new Arduino_ETA4662(IIC_Bus, ETA4662_DEVICE_ADDRESS,
                                                         DRIVEBUS_DEFAULT_VALUE, DRIVEBUS_DEFAULT_VALUE));

std::unique_ptr<Arduino_IIC> SGM41562(new Arduino_SGM41562(IIC_Bus, SGM41562_DEVICE_ADDRESS,
                                                           DRIVEBUS_DEFAULT_VALUE, DRIVEBUS_DEFAULT_VALUE));

void initPowerChip()
{
  if (SGM41562->begin())
  {
    Serial.println("SGM41562 initialization successfully");
    return;
  }

  if (ETA4662->begin())
  {
    Serial.println("ETA4662 initialization successfully");
    return;
  }

  Serial.println("Power chip initialization failed");
}

void setupBreathingLight()
{
  // setup breathing light
  pinMode(BREATHING_LIGHT, OUTPUT);
  ledcAttach(BREATHING_LIGHT, 2000, 8);
  ledcWrite(BREATHING_LIGHT, 255); // breathing light off
  // ledcWrite(BREATHING_LIGHT, 0); // breathing light on
}

void setupBatteryMessurement()
{
  pinMode(BATTERY_ADC_DATA, INPUT_PULLDOWN);
  pinMode(BATTERY_MEASUREMENT_CONTROL, OUTPUT);
  digitalWrite(BATTERY_MEASUREMENT_CONTROL, HIGH); // set battery messurement off
  analogReadResolution(12);
}

void setupBacklight()
{
  pinMode(LCD_BL, OUTPUT);
  ledcAttach(LCD_BL, 2000, 8);
  ledcWrite(LCD_BL, 255); // deactivate backlight as default to reduce power consumption
  // ledcWrite(LCD_BL, 0); // activate backlight
}

void setup()
{
  serialBegin(115200);

  initPowerChip();
  setupBreathingLight();
  setupBatteryMessurement();
  setupBacklight();
}

void loop()
{
  for (int i = 255; i > 0; i--)
  {
    ledcWrite(BREATHING_LIGHT, i);
    delay(2);
  }

  for (int i = 0; i < 255; i++)
  {
    ledcWrite(BREATHING_LIGHT, i);
    delay(2);
  }

  delay(1000);
}