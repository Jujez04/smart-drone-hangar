#ifndef __SERIAL_COMM_TASK__
#define __SERIAL_COMM_TASK__

#include "kernel/AperiodicTask.h"
#include "model/Context.h"
#include <Arduino.h>

class SerialCommTask : public AperiodicTask {
public:
    SerialCommTask(Context* pContext);
    void init() override;
    void tick() override;

private:
    enum SerialCommState{
        LISTENING,
        PROCESSING,
        SENDING
    } state;

    void setState(SerialCommState state);
    long elapsedTimeInState();
    bool checkAndSetJustEntered();
    void log(const String& msg);

    long stateTimestamp;
    bool justEntered;

    Context* pContext;
    String inputBuffer;

    static const String APP_PREFIX;
    static const String LOG_PREFIX;

    void readSerial();
    void handleIncomingMessage(const String& msg);
    void sendStatusToDRU();
    void sendAlarmToDRU();
    void sendLogMessage(const String& msg);
    bool parseCommand(const String& msg, String& command);
};

#endif