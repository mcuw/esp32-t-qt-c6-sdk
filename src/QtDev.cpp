#include "pin_config.h"
#include "QtDev.h"

QtDev::QtDev() : Qt() {
                 };

QtDev::~QtDev() {
};

void QtDev::setupBacklight()
{
  Qt::setupBacklight();
  Qt::setBacklight(0); // on
}

void QtDev::setupBatteryMessurement()
{
  Qt::setupBatteryMessurement();
  digitalWrite(BATTERY_MEASUREMENT_CONTROL, LOW); // on
}

void QtDev::setupBreathingLight()
{
  Qt::setupBreathingLight();
  Qt::setBreathingLight(0); // on
}
