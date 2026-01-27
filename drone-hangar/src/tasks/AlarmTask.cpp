#include "tasks/AlarmTask.h"

#include "kernel/Logger.h"
#include "config.h"

AlarmTask::AlarmTask(TempSensor *pTempSensor, Context *pContext)
    : pTempSensor(pTempSensor), pContext(pContext)
{
    setState(IDLE);
}

void AlarmTask::init(int period)
{
    PeriodicTask::init(period);
}

void AlarmTask::tick()
{
    float currentTemp = pTempSensor->getTemperature();
    switch (state)
    {
    case IDLE:
        if (checkAndSetJustEntered())
        {
            timeAboveTemp1 = 0;
            timeAboveTemp2 = 0;
            pContext->clearPreAlarm();
        }
        if (currentTemp >= TEMP_1)
        {
            if (timeAboveTemp1 == 0)
            {
                timeAboveTemp1 = millis();
            }
            if (millis() - timeAboveTemp1 >= T3) {
                setState(PRE_ALARM);
            }
        } else {
             timeAboveTemp1 = 0;
        }
        break;

    case PRE_ALARM:
        if (checkAndSetJustEntered())
        {
            Serial.println(F("lo:[AlarmTask] State: PRE_ALARM"));
            timeAboveTemp1 = 0;
            timeAboveTemp2 = 0;
            pContext->triggerPreAlarm();
        }

        if (currentTemp < TEMP_1) {
            Serial.println(F("lo:[AlarmTask] Temp OK -> IDLE"));
            setState(IDLE);
        }
        break;

    case ALARM:
        break;
    }
}

void AlarmTask::setState(AlarmState s)
{
    state = s;
    stateTimestamp = millis();
    justEntered = true;
}

unsigned long AlarmTask::elapsedTimeInState()
{
    return millis() - stateTimestamp;
}

bool AlarmTask::checkAndSetJustEntered()
{
    bool result = justEntered;
    if (justEntered)
        justEntered = false;
    return result;
}

void AlarmTask::log(const String &msg)
{
    Logger.log("[AlarmTask] " + msg);
}