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
    // 1. Init System
    Logger.init();
    delay(100); // Attendi un attimo per stabilizzare la serialeZ
    Logger.log("::::: Drone Hangar System Booting :::::");

    sched.init(50); // Base period 50ms (MCD)

    // 2. Init Hardware
    pHWPlatform = new HWPlatform();
    pHWPlatform->init();

    // 3. Init Context (Blackboard)
    pContext = new Context();
#ifndef __TESTING_HW__

    // 4. Instantiate Tasks (Dependency Injection)

    // A. Serial Comm (Aperiodic - runs every cycle)
    tSerial = new SerialCommTask(pContext);
    tSerial->init(SCHEDULER_BASE_PERIOD);
    sched.addTask(tSerial);

    // B. Logic Orchestrator (100ms)
    tHangar = new HangarTask(
        pHWPlatform->getLcd(),
        pHWPlatform->getL1(),
        pHWPlatform->getL3(),
        pHWPlatform->getButton(),
        pContext);
    tHangar->init(HANGAR_TASK_PERIOD);
    sched.addTask(tHangar);

    // Presence Detector (PIR - 1000ms)
    tPresence = new PresenceDetectorTask(pHWPlatform->getPir(), pContext);
    tPresence->init(DPD_TASK_PERIOD);
    sched.addTask(tPresence);

    // C. Actuators
    // Door Task (100ms movement update)
    tDoor = new DoorTask(pHWPlatform->getMotor(), pContext);
    tDoor->init(DOOR_TASK_PERIOD);
    sched.addTask(tDoor);

    // Blinking Task
    tBlink = new BlinkingTask(pHWPlatform->getL2(), pContext);
    tBlink->init(SENSORS_TASK_PERIOD);
    sched.addTask(tBlink);

    // D. Sensors
    // Distance Detector (Sonar - 200ms to allow echo)
    tDistance = new DistanceDetectorTask(pHWPlatform->getSonar(), pContext);
    tDistance->init(SENSORS_TASK_PERIOD);
    sched.addTask(tDistance);


    // E. Alarm Task
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
