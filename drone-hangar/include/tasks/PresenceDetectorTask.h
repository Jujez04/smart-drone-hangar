#ifndef __PRESENCE_DETECTOR_TASK__
#define __PRESENCE_DETECTOR_TASK__

#include "kernel/PeriodicTask.h"
#include "model/Context.h"
#include "devices/pir/Pir.h"
#include <Arduino.h>

class PresenceDetectorTask : public PeriodicTask {
public:
    PresenceDetectorTask(Pir* pPir, Context* pContext);
    void tick();

private:
    enum PresenceDetectorState {
        IDLE,        // Not monitoring presence
        ACTIVE       // Actively reading PIR
    } state;

    void setState(PresenceDetectorState state);
    long elapsedTimeInState();
    bool checkAndSetJustEntered();
    void log(const String& msg);

    long stateTimestamp;
    bool justEntered;

    Pir* pPir;
    Context* pContext;


    // Helper methods
    bool readPresence();
    bool isDebouncedPresence();
};

#endif