#include "tasks/TestHWTask.h"

#include <Arduino.h>

TestHWTask::TestHWTask(HWPlatform* pHW) : pHW(pHW) {}

void TestHWTask::tick() {
    pHW->test();
}