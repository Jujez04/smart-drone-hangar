#ifndef __BLINKING_TASK__
#define __BLINKING_TASK__

#include "kernel/Task.h"
#include "model/Context.h"
#include "devices/led/Led.h"
#include <Arduino.h>



class BlinkingTask: public Task {

public:
  BlinkingTask(Led* pLed, Context* pContext);
  void tick();

private:
  enum BlinkingState { IDLE, OFF, ON };

  void setState(BlinkingState state);
  long elapsedTimeInState();
  void log(const String& msg);

  bool checkAndSetJustEntered();

  BlinkingState state;
  long stateTimestamp;
  bool justEntered;

  Led* pLed;
  Context* pContext;
};

#endif