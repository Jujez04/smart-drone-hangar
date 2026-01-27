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
            pContext->confirmDoorClosed();
        }
        // Open on Takeoff OR (Landing AND PIR detected)
        if (pContext->isTakeOffCommandReceived() ||
            (pContext->isLandingCommandReceived() && pContext->isDroneNear()))
        {
            setState(OPENING);
        }
        break;
    }

    case OPENING:
    {
        if (checkAndSetJustEntered())
        {
            log("OPENING");
            pServo->setPosition(DOOR_OPEN_ANGLE);
            pContext->openDoor();
        }
        if (elapsedTimeInState() >= DOOR_MOVEMENT_TIME)
            setState(OPEN);
        break;
    }

    case OPEN:
    {
        if (checkAndSetJustEntered())
        {
            log("OPEN");
            pContext->confirmDoorOpened();
        }
        // Close logic:
        // 1. Drone is OUT and NOT landing (Takeoff done)
        if (pContext->isDroneOut() && !pContext->isLandingCommandReceived())
        {
            setState(CLOSING);
        }
        // 2. Drone is INSIDE (Landing done)
        else if (pContext->isDroneInside())
        {
            setState(CLOSING);
        }
        break;
    }

    case CLOSING:
    {
        if (checkAndSetJustEntered())
        {
            log("CLOSING");
            pServo->setPosition(DOOR_CLOSED_ANGLE);
            pContext->closeDoor();
        }
        if (elapsedTimeInState() >= DOOR_MOVEMENT_TIME)
            setState(CLOSED);
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