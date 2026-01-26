#include "tasks/DistanceDetectorTask.h"
#include "kernel/Logger.h"
#include "config.h"

DistanceDetectorTask::DistanceDetectorTask(Sonar* pSonar, Context* pContext)
    : pSonar(pSonar), pContext(pContext) {
    setState(IDLE);
}

void DistanceDetectorTask::tick() {
    switch (state) {
        case IDLE:
            timeAboveD1 = 0;
            timeBelowD2 = 0;
            if (pContext->isTakeOffCommandReceived() || pContext->isLandingCommandReceived() || pContext->isDoorOpen()) 
                setState(ACTIVE);
            break;

        case ACTIVE:
            if (pContext->isDroneInside() && !pContext->isTakeOffCommandReceived() && pContext->isDoorClosed()) {
                setState(IDLE);
                break;
            }

            float distance = readDistance();
            
            // --- FILTRO ANTIRUMORE FONDAMENTALE ---
            // Se leggiamo 0 o > 4m, ignoriamo la lettura (non resettiamo il timer!)
            if (distance <= 0.01 || distance > 4.0) return; 
            // --------------------------------------

            // Logica Takeoff
            if (distance > D1) {
                if (timeAboveD1 == 0) timeAboveD1 = millis();
                if (millis() - timeAboveD1 >= T1) {
                    if (!pContext->isDroneOut()) {
                        Logger.log("Drone OUT confirmed!"); // Debug
                        pContext->confirmDroneOut();
                        timeAboveD1 = 0;
                    }
                }
            } else {
                timeAboveD1 = 0; // Resetta solo se la lettura è valida ma sotto soglia
            }

            // Logica Landing
            if (distance < D2) {
                if (timeBelowD2 == 0) timeBelowD2 = millis();
                if (millis() - timeBelowD2 >= T2) {
                    if (pContext->isDroneOut()) {
                        Logger.log("Drone INSIDE confirmed!"); // Debug
                        pContext->confirmDroneInside();
                        timeBelowD2 = 0;
                    }
                }
            } else {
                timeBelowD2 = 0;
            }
            break;
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