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
    Serial.begin(SERIAL_BAUD_RATE);
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
    tSerial->init(50);
    sched.addTask(tSerial);

    // B. Logic Orchestrator (100ms)
    tHangar = new HangarTask(
        pHWPlatform->getLcd(),
        pHWPlatform->getL1(),
        pHWPlatform->getL3(),
        pHWPlatform->getButton(),
        pContext);
    tHangar->init(100);
    sched.addTask(tHangar);

    // Presence Detector (PIR - 200ms)
    tPresence = new PresenceDetectorTask(pHWPlatform->getPir(), pContext);
    tPresence->init(200);
    sched.addTask(tPresence);

    // C. Actuators
    // Door Task (100ms movement update)
    tDoor = new DoorTask(pHWPlatform->getMotor(), pContext);
    tDoor->init(100);
    sched.addTask(tDoor);

    // Blinking Task (Standard blink logic)
    tBlink = new BlinkingTask(pHWPlatform->getL2(), pContext);
    tBlink->init(100); // Check period (frequency managed inside)
    sched.addTask(tBlink);

    // D. Sensors
    // Distance Detector (Sonar - 200ms to allow echo)
    tDistance = new DistanceDetectorTask(pHWPlatform->getSonar(), pContext);
    tDistance->init(200);
    sched.addTask(tDistance);


    // E. Alarm Task
    tAlarm = new AlarmTask(pHWPlatform->getTempSensor(), pHWPlatform->getButton(), pContext);
    tAlarm->init(500);
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
