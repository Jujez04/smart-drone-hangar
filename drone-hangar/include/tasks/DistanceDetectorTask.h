#ifndef __DISTANCE_DETECTOR_TASK__
#define __DISTANCE_DETECTOR_TASK__

#include "kernel/PeriodicTask.h"
#include "model/Context.h"
#include "devices/sonar/Sonar.h"
#include <Arduino.h>

/**
 * @class DistanceDetectorTask
 * @brief A task that detects the distance of the drone using a sonar sensor.
 */
class DistanceDetectorTask : public PeriodicTask {
public:
    DistanceDetectorTask(Sonar* pSonar, Context* pContext);
    void tick();

private:
    long timeAboveD1;
    long timeBelowD2;

    enum DistanceDetectorState {
        IDLE,        // Not monitoring distance
        ACTIVE       // Actively reading sonar
    } state;

    void setState(DistanceDetectorState state);
    long elapsedTimeInState();
    bool checkAndSetJustEntered();
    void log(const String& msg);

    long stateTimestamp;
    bool justEntered;

    Sonar* pSonar;
    Context* pContext;

    // Helper methods
    float readDistance();
    bool isValidReading(float distance);
};

#endif