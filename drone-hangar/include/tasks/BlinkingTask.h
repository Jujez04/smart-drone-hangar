#ifndef __BLINKING_TASK__
#define __BLINKING_TASK__

#include "kernel/PeriodicTask.h"
#include "model/Context.h"
#include "devices/led/Led.h"
#include <Arduino.h>

/**
 * @class BlinkingTask
 * @brief A task that manages the blinking of an LED based on the system context.
 */
class BlinkingTask : public PeriodicTask {
public:
  BlinkingTask(Led *pLed, Context *pContext);
  void init(int period) override;
  void tick() override;

private:
  enum BlinkingState {
    IDLE, // Waiting
    OFF, // LED off
    ON // LED on
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