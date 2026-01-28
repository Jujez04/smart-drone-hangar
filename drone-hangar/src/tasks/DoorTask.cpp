#include "tasks/DoorTask.h"
#include "kernel/Logger.h"
#include "config.h"

DoorTask::DoorTask(ServoMotor *pServo, Context *pContext)
    : pServo(pServo), pContext(pContext)
{
    setState(CLOSED);
    currentPosition = DOOR_CLOSED_ANGLE;
    pServo->on();
    pServo->setPosition(DOOR_CLOSED_ANGLE);
}

void DoorTask::tick()
{
    switch (state)
    {
    case CLOSED:
    {
        if (checkAndSetJustEntered())
        {
            log("CLOSED");
            pServo->setPosition(DOOR_CLOSED_ANGLE);
            currentPosition = DOOR_CLOSED_ANGLE;
            pContext->confirmDoorClosed();
        }

        bool validTakeoff = pContext->isTakeOffCommandReceived() && pContext->isDroneInside();
        bool validLanding = pContext->isLandingCommandReceived() && pContext->isDroneNear();

        if ((validTakeoff || validLanding) && pContext->isCommandsEnabled())
        {
            setState(OPENING);
        }
        break;
    }

    case OPENING:
    {
        if (checkAndSetJustEntered())
        {
            log("OPENING...");
            pContext->openDoor();
        }

        static long lastStepTime = 0;
        if (millis() - lastStepTime >= SERVO_STEP_DELAY) {
            lastStepTime = millis();
            if (currentPosition < DOOR_OPEN_ANGLE) {
                currentPosition++;
                pServo->setPosition(currentPosition);
            } else {
                setState(OPEN);
            }
        }
        break;
    }

    case OPEN:
    {
        if (checkAndSetJustEntered())
        {
            log("OPEN");
            pContext->confirmDoorOpened();
        }

        if (pContext->isDroneOut() && pContext->isTakeOffCommandReceived())
        {
            pContext->clearTakingOffCommand();
            setState(CLOSING);
        }
        if (pContext->isAlarm() || pContext->isPreAlarm()) {
            setState(CLOSING);
        }
        else if (pContext->isDroneInside() && pContext->isLandingCommandReceived())
        {
            pContext->clearLandingCommand();
            setState(CLOSING);
        }
        break;
    }

    case CLOSING:
    {
        if (checkAndSetJustEntered())
        {
            log("CLOSING...");
            pContext->closeDoor();
        }

        static long lastStepTime = 0;
        if (millis() - lastStepTime >= SERVO_STEP_DELAY) {
            lastStepTime = millis();
            if (currentPosition > DOOR_CLOSED_ANGLE) {
                currentPosition--;
                pServo->setPosition(currentPosition);
            } else {
                setState(CLOSED);
            }
        }
        break;
    }
    }
}

void DoorTask::setState(DoorState s)
{
    state = s;
    stateTimestamp = millis();
    justEntered = true;
}

long DoorTask::elapsedTimeInState()
{
    return millis() - stateTimestamp;
}

bool DoorTask::checkAndSetJustEntered()
{
    bool result = justEntered;
    if (justEntered)
        justEntered = false;
    return result;
}

void DoorTask::log(const String &msg)
{
    Logger.log("[DoorTask] " + msg);
}