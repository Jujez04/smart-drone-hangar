#ifndef __SERIAL_COMM_TASK__
#define __SERIAL_COMM_TASK__

#include "kernel/PeriodicTask.h"
#include "model/Context.h"
#include <Arduino.h>

/**
 * @class SerialCommTask
 * @brief A task that handles serial communication with the drone hangar system.
 * It processes incoming commands and sends status updates periodically.
 */

class SerialCommTask : public PeriodicTask {

public:
    SerialCommTask(Context* pContext);
    void init(int period) override;
    void tick() override;

private:
    Context* pContext;
    String inputBuffer;

    unsigned long lastStatusTime;
    const unsigned long STATUS_PERIOD = 200; // Send status every 200ms

    // Communication protocol
    const String CMD_PREFIX = "ru:";
    const String APP_PREFIX = "dh:";
    const String LOG_PREFIX = "lo:";

    // Metodi interni
    void readSerialData();
    void processCommand(String command);
    void sendStatusUpdate();
    void log(const String& msg);
};

#endif