#ifndef __SERIAL_COMM_TASK__
#define __SERIAL_COMM_TASK__

#include "kernel/PeriodicTask.h"
#include "model/Context.h"
#include <Arduino.h>

class SerialCommTask : public PeriodicTask {

public:
    SerialCommTask(Context* pContext);
    void init(int period) override;
    void tick() override;

private:
    Context* pContext;
    String inputBuffer;
    
    // Timer per l'invio dello stato (non vogliamo inviare ad ogni tick)
    unsigned long lastStatusTime;
    const unsigned long STATUS_PERIOD = 200; // Invia stato ogni 200ms

    // Prefissi protocollo
    static const String CMD_PREFIX;  // "ru:" (Input)
    static const String APP_PREFIX;  // "dh:" (Output)
    static const String LOG_PREFIX;  // "lo:" (Output)

    // Metodi interni
    void readSerialData();
    void processCommand(String command);
    void sendStatusUpdate();
    void sendLog(const String& msg);
};

#endif