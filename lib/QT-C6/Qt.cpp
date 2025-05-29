#include <ArduinoOTA.h>
#include "Arduino_DriveBus_Library.h"
#include <WiFi.h>
#include "pin_config.h"
#include "./Qt.h"

// see https://github.com/Xinyuan-LilyGO/T-QT-C6
std::shared_ptr<Arduino_IIC_DriveBus> IIC_Bus =
    std::make_shared<Arduino_HWIIC>(IIC_SDA, IIC_SCL, &Wire);

std::unique_ptr<Arduino_IIC> ETA4662(new Arduino_ETA4662(IIC_Bus, ETA4662_DEVICE_ADDRESS,
                                                         DRIVEBUS_DEFAULT_VALUE, DRIVEBUS_DEFAULT_VALUE));

std::unique_ptr<Arduino_IIC> SGM41562(new Arduino_SGM41562(IIC_Bus, SGM41562_DEVICE_ADDRESS,
                                                           DRIVEBUS_DEFAULT_VALUE, DRIVEBUS_DEFAULT_VALUE));

Qt::Qt()
{
}

void Qt::begin()
{
  initPowerChip();
  setupBreathingLight();
  setupBatteryMessurement();
  setupBacklight();
}

void Qt::initPowerChip()
{
  if (SGM41562->begin())
  {
    Serial.println("SGM41562 initialized");
    return;
  }

  if (ETA4662->begin())
  {
    Serial.println("ETA4662 initialized");
    return;
  }

  Serial.println("Power chip initialization failed");
}

void Qt::setupBacklight()
{
  pinMode(LCD_BL, OUTPUT);
  ledcAttach(LCD_BL, 2000, 8);
  ledcWrite(LCD_BL, 255); // deactivate backlight as default to reduce power consumption
  // ledcWrite(LCD_BL, 0); // activate backlight
}

void Qt::setupBatteryMessurement()
{
  pinMode(BATTERY_ADC_DATA, INPUT_PULLDOWN);
  pinMode(BATTERY_MEASUREMENT_CONTROL, OUTPUT);
  digitalWrite(BATTERY_MEASUREMENT_CONTROL, HIGH); // off for low power consumption
  analogReadResolution(12);
}

void Qt::setupBreathingLight()
{
  pinMode(BREATHING_LIGHT, OUTPUT);
  ledcAttach(BREATHING_LIGHT, 2000, 8);
  ledcWrite(BREATHING_LIGHT, 255); // off for low power consumption
}

void Qt::setBacklight(uint32_t duty)
{
  ledcWrite(LCD_BL, duty);
}

void Qt::setBreathingLight(uint32_t duty)
{
  ledcWrite(BREATHING_LIGHT, duty);
}

void Qt::setBatteryMessurement(bool activate)
{
  digitalWrite(BATTERY_MEASUREMENT_CONTROL, activate ? LOW : HIGH); // set battery messurement off
}
