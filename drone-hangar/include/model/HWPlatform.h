#ifndef __HW_PLATFORM__
#define __HW_PLATFORM__

#include "devices/button/Button.h"
#include "devices/led/Led.h"
#include "devices/servo/ServoMotor.h"
#include "devices/pir/Pir.h"
#include "devices/sonar/Sonar.h"
#include "devices/temperature/TempSensor.h"
#include "devices/lcd/Display.h"


class HWPlatform {
public:
    HWPlatform();
    void init();

    Button* getButton();
    ServoMotor* getMotor();
    Led* getL1();
    Led* getL2();
    Led* getL3();
    Pir* getPir();
    Sonar* getSonar();
    TempSensor* getTempSensor();
    Display* getLcd();

    void test();

private:
    Button* pButton;
    ServoMotor* pMotor;
    Led* pL1;
    Led* pL2;
    Led* pL3;
    Pir* pPir;
    Sonar* pSonar;
    TempSensor* pTemp;
    Display* pLcd;
};

#endif