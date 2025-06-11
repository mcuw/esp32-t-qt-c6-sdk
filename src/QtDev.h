#ifndef QTDEV_H
#define QTDEV_H

#include "./Qt.h"

class QtDev : public Qt
{
public:
  QtDev();
  virtual ~QtDev();

protected:
  virtual void setupBacklight();
  virtual void setupBatteryMessurement();
  virtual void setupBreathingLight();
};

#endif