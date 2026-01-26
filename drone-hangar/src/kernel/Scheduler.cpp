#include "kernel/Scheduler.h"
#include <Arduino.h>
#include "kernel/PeriodicTask.h"
#include "kernel/AperiodicTask.h"

void Scheduler::init(unsigned long basePeriod){
  this->basePeriod = basePeriod;
  nTasks = 0;
}

bool Scheduler::addTask(Task* task){
  if (nTasks < MAX_TASKS - 1){
    taskList[nTasks] = task;
    nTasks++;
    return true;
  } else {
    return false;
  }
}

void Scheduler::schedule() {
    while (true) {
        for (int i = 0; i < nTasks; i++) {
            Task* task = taskList[i];
            if (task->getType() == PERIODIC) {
                PeriodicTask* periodicTask = (PeriodicTask*)task;
                if (periodicTask->isActive() &&
                    periodicTask->updateAndCheckTime(basePeriod)) {
                    periodicTask->tick();
                }
            } else if (task->getType() == APERIODIC) {
                AperiodicTask* aperiodicTask = (AperiodicTask*)task;
                if (aperiodicTask->isActive() && !aperiodicTask->isCompleted()) {
                    aperiodicTask->tick();
                }
            }
        }
        delay(basePeriod);
    }
}