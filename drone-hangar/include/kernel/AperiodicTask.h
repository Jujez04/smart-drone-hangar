#ifndef __APERIODIC_TASK__
#define __APERIODIC_TASK__

#include "Task.h"

class AperiodicTask : public Task {
public:
    AperiodicTask() : Task(APERIODIC) {}

    virtual void init() {
        active = true;
        completed = false;
    }

    void setCompleted() {
        completed = true;
        active = false;
    }
};

#endif