#ifndef __ALARM_TASK__
#define __ALARM_TASK__

#include <Arduino.h>

#include "kernel/PeriodicTask.h"
#include "model/Context.h"
#include "devices/temperature/TempSensorTMP36.h"
#include "devices/button/Button.h"
#include "config.h"

class AlarmTask : public PeriodicTask {
public:
    AlarmTask(TempSensor* pTempSensor, Button* pResetButton, Context* pContext);
    void init(int period) override;
    void tick() override;
private:
    TempSensor* pTempSensor;
    Button* pResetButton;
    Context* pContext;

    enum AlarmState {
        IDLE,
        PRE_ALARM,
        ALARM
    } state;

    unsigned long timeAboveTemp1;
    unsigned long timeAboveTemp2;
    unsigned long stateTimestamp;
    bool justEntered;

    void setState(AlarmState newState);
    unsigned long elapsedTimeInState();
    bool checkAndSetJustEntered();
    void log(const String& msg);
};

#endif