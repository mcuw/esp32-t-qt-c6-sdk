#include <ArduinoOTA.h>
#include <WiFi.h>
#include <LittleFS.h>
#include "Arduino_GFX_Library.h"
#include "Arduino_DriveBus_Library.h"
#include "pin_config.h"
#include "./Qt.h"

void Arduino_IIC_Touch_Interrupt(void);

Arduino_DataBus *bus = new Arduino_HWSPI(LCD_DC, LCD_CS, LCD_SCLK, LCD_MOSI, LCD_MISO);

Arduino_GFX *gfx = new Arduino_GC9107(
    bus, LCD_RST, 0 /* rotation */, true /* IPS */,
    LCD_WIDTH, LCD_HEIGHT,
    2 /* col offset 1 */, 1 /* row offset 1 */, 0 /* col_offset2 */, 0 /* row_offset2 */);

std::shared_ptr<Arduino_IIC_DriveBus> IIC_Bus =
    std::make_shared<Arduino_HWIIC>(IIC_SDA, IIC_SCL, &Wire);

std::unique_ptr<Arduino_IIC> ETA4662(new Arduino_ETA4662(IIC_Bus, ETA4662_DEVICE_ADDRESS,
                                                         DRIVEBUS_DEFAULT_VALUE, DRIVEBUS_DEFAULT_VALUE));

std::unique_ptr<Arduino_IIC> SGM41562(new Arduino_SGM41562(IIC_Bus, SGM41562_DEVICE_ADDRESS,
                                                           DRIVEBUS_DEFAULT_VALUE, DRIVEBUS_DEFAULT_VALUE));

std::unique_ptr<Arduino_IIC> CST816T(new Arduino_CST816x(IIC_Bus, CST816T_DEVICE_ADDRESS,
                                                         TP_RST, TP_INT, Arduino_IIC_Touch_Interrupt));

void Arduino_IIC_Touch_Interrupt(void)
{
  CST816T->IIC_Interrupt_Flag = true;
}

Qt::Qt()
{
}

void Qt::begin()
{
  initPowerChip();
  setupBreathingLight();
  setupBatteryMessurement();
  setupBacklight();
  setupTouch();
  setupGfx();
}

void Qt::initPowerChip()
{
  if (SGM41562->begin())
  {
    log_v("SGM41562 initialized");
    return;
  }

  if (ETA4662->begin())
  {
    log_v("ETA4662 initialized");
    return;
  }

  log_e("Power chip initialization failed");
}

void Qt::setupBacklight()
{
  pinMode(LCD_BL, OUTPUT);
  ledcAttach(LCD_BL, 2000, 8);
  setBacklight(255); // deactivate backlight as default to reduce power consumption
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
  digitalWrite(BATTERY_MEASUREMENT_CONTROL, activate ? LOW : HIGH); // set battery messurement activation
}

float Qt::getBatteryVoltage()
{
  return (((float)analogReadMilliVolts(BATTERY_ADC_DATA)) / 1000.0) * 2.0;
}

void Qt::setupTouch()
{
  while (!CST816T->begin())
  {
    log_e("CST816T initialization failed");
    delay(2000);
  }

  log_v("CST816T initialized");
}

TouchState Qt::getTouch()
{
  TouchState state = {false, "", 0, 0};

  if (!CST816T->IIC_Interrupt_Flag)
    return state;

  CST816T->IIC_Interrupt_Flag = false;
  state.touched = true;

  state.gesture = CST816T->IIC_Read_Device_State(CST816T->Arduino_IIC_Touch::Status_Information::TOUCH_GESTURE_ID);
  if (state.gesture.endsWith("fail"))
  {
    state.gesture = "None";
  }

  state.x = CST816T->IIC_Read_Device_Value(CST816T->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_X);
  state.y = CST816T->IIC_Read_Device_Value(CST816T->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_Y);

  state.fingers = CST816T->IIC_Read_Device_Value(CST816T->Arduino_IIC_Touch::Value_Information::TOUCH_FINGER_NUMBER);

  return state;
}

void Qt::setupGfx()
{
  gfx->begin();
  // example overwrite setupGfx()s and set default color
  // gfx->fillScreen(color);
}

void Qt::fillScreenGfx(uint16_t color)
{
  gfx->fillScreen(color);
}

void Qt::printfGfx(uint16_t color, int16_t x, int16_t y, const char *format, ...)
{
  gfx->setCursor(x, y);
  gfx->setTextColor(color);

  va_list args;
  va_start(args, format);
  gfx->vprintf(format, args);
  va_end(args);
}

void Qt::setTextSizeGfx(uint8_t s)
{
  gfx->setTextSize(s);
}

void Qt::setupAp(const char *ssid, const char *passphrase, wifi_auth_mode_t auth_mode, int channel,
                 int ssid_hidden, int max_connection, bool ftm_responder)
{
  if (!passphrase)
  {
    log_w("Setup an empty passphrase is insecure");
  }
  WiFi.mode(WIFI_AP);
  delay(10);
  WiFi.softAP(ssid, passphrase, channel, ssid_hidden, max_connection, false, auth_mode);
}

void Qt::setupFs()
{
  if (!LittleFS.begin(true))
  {
    log_e("Could not mount the filesystem.");
    log_w("restarting...");
    delay(2000);
    ESP.restart();
  }
}