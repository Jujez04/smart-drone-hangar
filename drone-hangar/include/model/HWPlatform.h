#ifndef __HW_PLATFORM__
#define __HW_PLATFORM__

#include "config.h"
#include "devices/button/Button.h"
#include "devices/led/Led.h"
#include "devices/servo/ServoMotor.h"

class HWPlatform {

public:
  HWPlatform();
  void init();
  void test();

  Button* getButton();
  Led*  getLed();
  ServoMotor* getMotor();

private:
  Button* pButton;
  Led* pLed;
  ServoMotor* pMotor;
};

#endif