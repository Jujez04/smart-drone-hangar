#ifndef __ALARM_TASK__
#define __ALARM_TASK__

#include <Arduino.h>

#include "kernel/PeriodicTask.h"
#include "model/Context.h"
#include "devices/temperature/TempSensorTMP36.h"
#include "config.h"

class AlarmTask : public PeriodicTask {
public:
    AlarmTask(TempSensor* pTempSensor, Context* pContext);
    void init(int period) override;
    void tick() override;
private:
    TempSensor* pTempSensor;
    Context* pContext;

    enum AlarmState {
        IDLE,
        PRE_ALARM,
        ALARM
    } state;

    unsigned long timeAboveT1;
    unsigned long timeAboveT2;
    unsigned long stateTimestamp;
    bool justEntered;

    void setState(AlarmState newState);
    unsigned long elapsedTimeInState();
    bool checkAndSetJustEntered();
    void log(const String& msg);
};

#endif