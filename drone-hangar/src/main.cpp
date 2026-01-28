#include <Arduino.h>
#include "config.h"

// Kernel
#include "kernel/Scheduler.h"
#include "kernel/Logger.h"

// Model & HW
#include "model/HWPlatform.h"
#include "model/Context.h"

// Tasks
#include "tasks/BlinkingTask.h"
#include "tasks/DoorTask.h"
#include "tasks/DistanceDetectorTask.h"
#include "tasks/PresenceDetectorTask.h"
#include "tasks/HangarTask.h"
#include "tasks/SerialCommTask.h"
#include "tasks/TestHWTask.h"
#include "tasks/AlarmTask.h"

Scheduler sched;
HWPlatform *pHWPlatform;
Context *pContext;

// Task Pointers
SerialCommTask *tSerial;
HangarTask *tHangar;
DoorTask *tDoor;
DistanceDetectorTask *tDistance;
PresenceDetectorTask *tPresence;
BlinkingTask *tBlink;
AlarmTask* tAlarm;

// #define __TESTING_HW__

void setup() {
    Logger.init();
    delay(100);
    Logger.log("::::: Drone Hangar System Booting :::::");

    sched.init(SCHEDULER_BASE_PERIOD);

    pHWPlatform = new HWPlatform();
    pHWPlatform->init();

    pContext = new Context();
#ifndef __TESTING_HW__

    // Serial Communication Task
    tSerial = new SerialCommTask(pContext);
    tSerial->init(SCHEDULER_BASE_PERIOD);
    sched.addTask(tSerial);

    // Hangar Task
    tHangar = new HangarTask(
        pHWPlatform->getLcd(),
        pHWPlatform->getL1(),
        pHWPlatform->getL3(),
        pHWPlatform->getButton(),
        pContext);
    tHangar->init(HANGAR_TASK_PERIOD);
    sched.addTask(tHangar);

    // Presence Detector
    tPresence = new PresenceDetectorTask(pHWPlatform->getPir(), pContext);
    tPresence->init(DPD_TASK_PERIOD);
    sched.addTask(tPresence);

    // Door Task
    tDoor = new DoorTask(pHWPlatform->getMotor(), pContext);
    tDoor->init(DOOR_TASK_PERIOD);
    sched.addTask(tDoor);

    // Blinking Task
    tBlink = new BlinkingTask(pHWPlatform->getL2(), pContext);
    tBlink->init(SENSORS_TASK_PERIOD);
    sched.addTask(tBlink);

    // Distance Detector Task
    tDistance = new DistanceDetectorTask(pHWPlatform->getSonar(), pContext);
    tDistance->init(SENSORS_TASK_PERIOD);
    sched.addTask(tDistance);

    // Alarm Task
    tAlarm = new AlarmTask(pHWPlatform->getTempSensor(), pHWPlatform->getButton(), pContext);
    tAlarm->init(ALARM_TASK_PERIOD);
    sched.addTask(tAlarm);

#endif

#ifdef __TESTING_HW__
    PeriodicTask *pTestHWTask = new TestHWTask(pHWPlatform);
    pTestHWTask->init(50);
    sched.addTask(pTestHWTask);
#endif
    Logger.log("System Ready. Waiting for commands...");
}

void loop() {
    sched.schedule();
}
