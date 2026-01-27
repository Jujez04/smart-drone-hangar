#include "tasks/AlarmTask.h"

#include "kernel/Logger.h"
#include "config.h"

AlarmTask::AlarmTask(TempSensor *pTempSensor, Button *pResetButton, Context *pContext)
    : pTempSensor(pTempSensor), pResetButton(pResetButton), pContext(pContext)
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
            pContext->clearAlarm();
            pContext->enableCommands();
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
            pContext->disableCommands();
        }

        if (currentTemp < TEMP_1) {
            Serial.println(F("lo:[AlarmTask] Temp OK -> IDLE"));
            setState(IDLE);
        } else if (currentTemp >= TEMP_2) {
            if (timeAboveTemp2 == 0) timeAboveTemp2 = millis();
            if (millis() - timeAboveTemp2 >= T4) {
                setState(ALARM);
            }
        }
        else {
            timeAboveTemp2 = 0;
        }
        break;

    case ALARM:
        if (checkAndSetJustEntered())
        {
            Serial.println(F("lo:[Alarm] CRITICAL STATE"));
            pContext->triggerAlarm();
            pContext->disableCommands();
        }

        if (!pContext->isAlarm()) {
            Serial.println(F("lo:[Alarm] Manual Reset -> IDLE"));
            setState(IDLE);
        }
        if (pResetButton->isPressed())
        {
            pContext->clearAlarm();
            setState(IDLE);
        }
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