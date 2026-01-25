#ifndef __HANGAR_TASK__
#define __HANGAR_TASK__

#include "model/Context.h"
#include "kernel/PeriodicTask.h"
#include "devices/button/Button.h"
#include "devices/led/Led.h"
#include "devices/lcd/LcdI2C.h"

enum HangarState {
    DRONE_INSIDE,
    TAKE_OFF,
    DRONE_OUT,
    LANDING,
    PRE_ALARM,
    ALARM
};

class HangarTask: public PeriodicTask {

public:
    HangarTask(LcdI2C* lcd, Led* l1, Button* resetButton, Context* context);
    void tick();

private:
    void setState(HangarState state);
    long elapsedTimeInState();
    bool checkAndSetJustEntered();
    void log(const char& text);
    void updateDisplay();

    HangarState state;
    long stateTimestamp;
    bool justEntered;
    unsigned long takeoffConfirmationTimer;
    unsigned long landingConfirmationTimer;
    Led* l1;
    Button* resetButton;
    Context* context;
    LcdI2C* lcd;

    bool isTakeOffConfirmed();
    bool isLandingConfirmed();
    void updateContextState();
};

#endif