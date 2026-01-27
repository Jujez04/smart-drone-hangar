#include "tasks/AlarmTask.h"

#include "kernel/Logger.h"
#include "config.h"

AlarmTask::AlarmTask(TempSensorTMP36 *pTempSensor, Context *pContext)
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