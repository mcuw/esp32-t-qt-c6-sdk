#ifndef QT_H
#define QT_H
#include <Arduino.h>
#include <Wifi.h>

typedef struct
{
  bool touched;
  String gesture;
  uint8_t fingers;
  uint8_t x;
  uint8_t y;
} TouchState;

class Qt
{
public:
  Qt();
  virtual ~Qt() {};
  void begin();

  void setBacklight(uint32_t duty = 255);      // off=255 max=0, deactivated backlight as default to reduce power consumption
  void setBreathingLight(uint32_t duty = 255); // breathing light off=255, max=0
  void setBatteryMessurement(bool activate = false);

  float getBatteryVoltage();

  TouchState getTouch();

  void fillScreenGfx(uint16_t color);
  void printfGfx(uint16_t color, int16_t x, int16_t y, const char *format, ...);
  // Desired text size. 1 is default 6x8, 2 is 12x16, 3 is 18x24, etc
  void setTextSizeGfx(uint8_t s = 1);

  void setupAp(const char *ssid, const char *passphrase = "qtqtpasspass",
               wifi_auth_mode_t auth_mode = WIFI_AUTH_WPA2_WPA3_PSK, int channel = 1, int ssid_hidden = 0, int max_connection = 4, bool ftm_responder = false);

  void setupFs();

protected:
  virtual void setupBacklight();
  virtual void setupBatteryMessurement();
  virtual void setupBreathingLight();
  virtual void setupTouch();
  virtual void setupGfx();

private:
  void initPowerChip();
};

#endif