#include <Arduino.h>

#include "config.h"

#include "kernel/Scheduler.h"
#include "kernel/Logger.h"
#include "kernel/MsgService.h"

#include "model/HWPlatform.h"
#include "model/Context.h"

#include "tasks/TestHWTask.h"

#define __TESTING_HW__

Scheduler sched;

HWPlatform* pHWPlatform;
Context* context;

void setup() {
    MsgService.init();
    sched.init(50);

    Logger.log("::::: Drone Hangar System :::::");
    pHWPlatform = new HWPlatform();
    pHWPlatform->init();

#ifndef __TESTING_HW__

#endif

#ifdef __TESTING_HW__
    PeriodicTask* pTestHWTask = new TestHWTask(pHWPlatform);
    pTestHWTask->init(2000);
    sched.addTask(pTestHWTask);
#endif

}

void loop() {
    sched.schedule();
}
