#include "tasks/DistanceDetectorTask.h"
#include "kernel/Logger.h"
#include "config.h"

DistanceDetectorTask::DistanceDetectorTask(Sonar *pSonar, Context *pContext)
    : pSonar(pSonar), pContext(pContext)
{
    setState(IDLE);
    timeAboveD1 = 0;
    timeBelowD2 = 0;
}

void DistanceDetectorTask::tick()
{
    switch (state)
    {
    case IDLE:
        // For GUI --> "--"
        pContext->setDistance(0.0);
        timeAboveD1 = 0;
        timeBelowD2 = 0;
        if (pContext->isTakeOffCommandReceived() || pContext->isLandingCommandReceived() || pContext->isDoorOpen())
            setState(ACTIVE);
        break;

    case ACTIVE:
        if (pContext->isDoorClosed() && !pContext->isTakeOffCommandReceived() && !pContext->isLandingCommandReceived())
        {
            setState(IDLE);
            break;
        }

        float distance = readDistance();

        if (isValidReading(distance))
            pContext->setDistance(distance);

        // Take-off detection
        if (distance > D1)
        {
            if (timeAboveD1 == 0) timeAboveD1 = millis();
            if (millis() - timeAboveD1 >= T1)
            {
                if (!pContext->isDroneOut())
                {
                    Logger.log("Drone OUT confirmed!");
                    pContext->confirmDroneOut();
                    timeAboveD1 = 0;
                }
            }
        }
        else
        {
            timeAboveD1 = 0;
        }

        // Landing detection
        if (distance < D2)
        {
            if (timeBelowD2 == 0) timeBelowD2 = millis();

            if (millis() - timeBelowD2 >= T2)
            {
                if (pContext->isDoorOpen())
                {
                    if (pContext->isDroneOut())
                    {
                        Logger.log("Drone INSIDE confirmed!");
                        pContext->confirmDroneInside();
                        timeBelowD2 = 0;
                    }
                }
                else
                {
                    timeBelowD2 = 0;
                }
            }
        }
        else
        {
            timeBelowD2 = 0;
        }
        break;
    }
}

float DistanceDetectorTask::readDistance()
{
    return pSonar->getDistance();
}

bool DistanceDetectorTask::isValidReading(float distance)
{
    return distance >= SONAR_MIN_DISTANCE && distance <= SONAR_MAX_DISTANCE && distance > 0;
}

void DistanceDetectorTask::setState(DistanceDetectorState s)
{
    state = s;
    stateTimestamp = millis();
    justEntered = true;
}

long DistanceDetectorTask::elapsedTimeInState() { return millis() - stateTimestamp; }

bool DistanceDetectorTask::checkAndSetJustEntered()
{
    bool r = justEntered;
    if (justEntered)
        justEntered = false;
    return r;
}

void DistanceDetectorTask::log(const String &msg)
{
    Logger.log("[DistanceDetector] " + msg);
}