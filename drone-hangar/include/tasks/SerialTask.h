#ifndef __SERIAL_TASK__
#define __SERIAL_TASK__

#include "kernel/PeriodicTask.h"
#include "kernel/MsgService.h"

class SerialTask : public PeriodicTask {
public:
    SerialTask() : PeriodicTask(50) {
    }

    void tick() override {
        MsgService.checkSerial();
    }
};

#endif