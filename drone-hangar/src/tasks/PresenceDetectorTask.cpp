#include "tasks/PresenceDetectorTask.h"
#include "kernel/Logger.h"
#include "config.h"

PresenceDetectorTask::PresenceDetectorTask(Pir *pPir, Context *pContext)
    : pPir(pPir), pContext(pContext)
{
    setState(IDLE);
}

void PresenceDetectorTask::init(int period)
{
    PeriodicTask::init(period);
}

void PresenceDetectorTask::tick()
{
    switch (state)
    {
    case IDLE:
    {
        if (checkAndSetJustEntered())
        {
            log("IDLE - Waiting for Landing...");
        }

        if (pContext->isLandingCommandReceived())
        {
            setState(ACTIVE);
        }
        break;
    }

    case ACTIVE:
    {
        if (checkAndSetJustEntered())
        {
            log("ACTIVE - Monitoring PIR...");
        }
        if (pContext->isDroneInside() && pContext->isDoorClosed())
        {
            setState(IDLE);
            break;
        }

        bool presence = readPresence();
        if (presence)
        {
            log("PRESENCE DETECTED!");
            pContext->confirmDroneNear();
        }
        else
        {
            log("NO PRESENCE.");
            pContext->confirmDroneFar();
        }
        break;
    }
    }
}

bool PresenceDetectorTask::readPresence()
{
    pPir->sync();
    return pPir->isDetected();
}

void PresenceDetectorTask::setState(PresenceDetectorState s)
{
    state = s;
    stateTimestamp = millis();
    justEntered = true;
}

long PresenceDetectorTask::elapsedTimeInState()
{
    return millis() - stateTimestamp;
}

bool PresenceDetectorTask::checkAndSetJustEntered()
{
    bool result = justEntered;
    if (justEntered)
        justEntered = false;
    return result;
}

void PresenceDetectorTask::log(const String &msg)
{
    Logger.log("[PIR] " + msg);
}