#include "kernel/Scheduler.h"
#include <Arduino.h>
#include "kernel/PeriodicTask.h"

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

void Scheduler::schedule(){
  for (int i = 0; i < nTasks; i++){
    if (taskList[i]->isActive()){
      if (taskList[i]->isPeriodic()){
        //Downcast
        PeriodicTask* pTask = (PeriodicTask*) taskList[i];
        if (pTask->updateAndCheckTime(basePeriod)){
          pTask->tick();
        }
      } else {
        taskList[i]->tick();
      }
    }
  }
}