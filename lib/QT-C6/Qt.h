#ifndef QT_H
#define QT_H
#include <Arduino.h>

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

protected:
  virtual void setupBacklight();
  virtual void setupBatteryMessurement();
  virtual void setupBreathingLight();
  virtual void setupTouch();

private:
  void initPowerChip();
};

#endif