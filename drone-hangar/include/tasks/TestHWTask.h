#ifndef __TEST_HW_TASK__
#define __TEST_HW_TASK__

#include "kernel/PeriodicTask.h"
#include "model/HWPlatform.h"

// For testing purposes only
class TestHWTask: public PeriodicTask {

public:
    TestHWTask(HWPlatform* pHW);
    void tick();

private:
    HWPlatform* pHW;
};

#endif