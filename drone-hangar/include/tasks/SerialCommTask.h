#ifndef __SERIAL_COMM_TASK__
#define __SERIAL_COMM_TASK__

#include "kernel/Task.h"
#include "kernel/MsgService.h"
#include "model/Context.h"
#include <Arduino.h>

class SerialCommTask : public Task {
public:
    SerialCommTask(Context* pContext);
    void tick();

private:
    enum SerialCommState {
        LISTENING,      // Waiting for messages from DRU
        PROCESSING,     // Processing received command
        SENDING         // Sending status/alarm to DRU
    } state;

    void setState(SerialCommState state);
    long elapsedTimeInState();
    bool checkAndSetJustEntered();
    void log(const String& msg);

    long stateTimestamp;
    bool justEntered;

    MsgServiceClass* pMsgService;
    Context* pContext;

    // Helper methods
    void handleIncomingMessage(const String& msg);
    void sendStatusToDRU();
    void sendAlarmToDRU();
    bool parseCommand(const String& msg, String& command);
};

#endif