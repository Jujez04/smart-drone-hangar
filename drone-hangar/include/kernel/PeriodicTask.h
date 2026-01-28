#ifndef __PERIODIC_TASK__
#define __PERIODIC_TASK__

#include "Task.h"

// Periodic implementation
/**
 * @class PeriodicTask
 * @brief A class representing a periodic task in the system.
 */
class PeriodicTask : public Task {
public:
    PeriodicTask() : Task(PERIODIC) {}

    virtual void init(int period) {
        this->period = period;
        timeElapsed = 0;
        active = true;
    }

    bool updateAndCheckTime(int basePeriod) {
        if (!active) return false;
        timeElapsed += basePeriod;
        if (timeElapsed >= period) {
            timeElapsed = 0;
            return true;
        }
        return false;
    }

private:
    unsigned long period;
    unsigned long timeElapsed;
};

#endif