#ifndef __BLINKING_TASK__
#define __BLINKING_TASK__

#include "kernel/PeriodicTask.h"
#include "model/Context.h"
#include "devices/led/Led.h"
#include <Arduino.h>

class BlinkingTask : public PeriodicTask {
public:
  BlinkingTask(Led *pLed, Context *pContext);
  void init(int period) override;
  void tick() override;

private:
  enum BlinkingState {
    IDLE,
    OFF,
    ON
  };

  void setState(BlinkingState state);
  long elapsedTimeInState();
  bool checkAndSetJustEntered();
  void log(const String &msg);

  BlinkingState state;

  long stateTimestamp;
  bool justEntered;

  Led *pLed;
  Context *pContext;
};

#endif