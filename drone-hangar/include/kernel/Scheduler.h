#ifndef __SCHEDULER__
#define __SCHEDULER__

#include "Task.h"

#define MAX_TASKS 30

class Scheduler {

private:
  unsigned long basePeriod;
  int nTasks;
  Task* taskList[MAX_TASKS];

public:
  void init(unsigned long basePeriod);
  virtual bool addTask(Task* task);
  virtual void schedule();
};

#endif