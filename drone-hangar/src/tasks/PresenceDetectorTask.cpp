#include "tasks/PresenceDetectorTask.h"
#include "kernel/Logger.h"
#include "config.h"

PresenceDetectorTask::PresenceDetectorTask(Pir* pPir, Context* pContext)
    : pPir(pPir), pContext(pContext) {
    setState(IDLE);
}

void PresenceDetectorTask::tick() {
    switch (state) {
        case IDLE: {
            if (checkAndSetJustEntered()) {
                log("IDLE - Presence monitoring disabled");
                pContext->confirmDroneFar();
            }

            if (pContext->isLandingCommandReceived()) {
                setState(ACTIVE);
            }
            break;
        }

        case ACTIVE: {
            if (checkAndSetJustEntered()) {
                log("ACTIVE - Presence monitoring enabled");
            }

            // Deactivate when monitoring is not needed
            if (pContext->isDroneInside()) {
                setState(IDLE);
            } else {
                // Read PIR sensor
                bool presence = readPresence();
                if (presence) {
                    pContext->confirmDroneNear();
                } else {
                    pContext->confirmDroneFar();
                }
            }
            break;
        }
    }
}

bool PresenceDetectorTask::readPresence() {
    return pPir->isDetected();
}

void PresenceDetectorTask::setState(PresenceDetectorState s) {
    state = s;
    stateTimestamp = millis();
    justEntered = true;
}

long PresenceDetectorTask::elapsedTimeInState() {
    return millis() - stateTimestamp;
}

bool PresenceDetectorTask::checkAndSetJustEntered() {
    bool result = justEntered;
    if (justEntered) {
        justEntered = false;
    }
    return result;
}

void PresenceDetectorTask::log(const String& msg) {
    Logger.log("[PresenceDetector] " + msg);
}