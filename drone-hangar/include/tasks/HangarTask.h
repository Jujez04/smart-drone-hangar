#ifndef __HANGAR_TASK__
#define __HANGAR_TASK__

#include "model/Context.h"
#include "kernel/PeriodicTask.h"
#include "devices/button/Button.h"
#include "devices/led/Led.h"
#include "devices/lcd/LcdI2C.h"
#include <Arduino.h>


class HangarTask: public PeriodicTask {

public:
    HangarTask(LcdI2C* lcd, Led* l1, Led* l3, Button* resetButton, Context* context);
    void tick();

private:
    enum HangarState {
        DRONE_INSIDE,
        TAKE_OFF,
        DRONE_OUT,
        LANDING,
        PRE_ALARM,
        ALARM
    };

    void setState(HangarState state);
    long elapsedTimeInState();
    bool checkAndSetJustEntered();
    void log(const String text);
    void updateDisplay();

    HangarState state;
    long stateTimestamp;
    bool justEntered;
    unsigned long takeoffConfirmationTimer;
    unsigned long landingConfirmationTimer;
    Led* l1;
    Led* l3;
    Button* resetButton;
    Context* context;
    LcdI2C* lcd;

    bool isTakeOffConfirmed();
    bool isLandingConfirmed();
    void updateContextState();
};

#endif