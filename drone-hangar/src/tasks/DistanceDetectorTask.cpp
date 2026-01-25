#include "tasks/DistanceDetectorTask.h"
#include "kernel/Logger.h"
#include "config.h"

DistanceDetectorTask::DistanceDetectorTask(Sonar* pSonar, Context* pContext)
    : pSonar(pSonar), pContext(pContext) {
    setState(IDLE);
}

void DistanceDetectorTask::tick() {
    switch (state) {
        case IDLE: {
            if (checkAndSetJustEntered()) {
                log("IDLE - Distance monitoring disabled");
            }

            // Activate when monitoring is needed
            if (pContext->isTakeOffCommandReceived()) {
                setState(ACTIVE);
            }
            break;
        }

        case ACTIVE: {
            if (checkAndSetJustEntered()) {
                log("ACTIVE - Distance monitoring enabled");
            }

            // Deactivate when monitoring is not needed
            if (!pContext->isDroneInside()) {
                setState(IDLE);
            } else {
                float distance = readDistance();
                if (isValidReading(distance)) {
                    if(distance > D1) {
                        pContext->confirmDroneOut();
                    }
                    if (distance < D2)
                    {
                        pContext->confirmDroneInside();
                    }
                } else {
                    log("WARNING: Invalid sonar reading ignored");
                }
            }
            break;
        }
    }
}

float DistanceDetectorTask::readDistance() {
    return pSonar->getDistance();
}

bool DistanceDetectorTask::isValidReading(float distance) {
    return distance >= SONAR_MIN_DISTANCE &&
           distance <= SONAR_MAX_DISTANCE &&
           distance > 0;  // Sonar returns 0 on error
}

void DistanceDetectorTask::setState(DistanceDetectorState s) {
    state = s;
    stateTimestamp = millis();
    justEntered = true;
}

long DistanceDetectorTask::elapsedTimeInState() {
    return millis() - stateTimestamp;
}

bool DistanceDetectorTask::checkAndSetJustEntered() {
    bool result = justEntered;
    if (justEntered) {
        justEntered = false;
    }
    return result;
}

void DistanceDetectorTask::log(const String& msg) {
    Logger.log("[DistanceDetector] " + msg);
}