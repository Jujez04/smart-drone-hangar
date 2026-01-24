#ifndef __PERIODIC_TASK__
#define __PERIODIC_TASK__

#include "Task.h"

class PeriodicTask : public Task {
public:
    PeriodicTask(int period) {
        this->type = PERIODIC;
        this->period = period;
        timeElapsed = 0;
    }

    void init() {
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