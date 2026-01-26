#ifndef __DOOR_TASK__
#define __DOOR_TASK__

#include "kernel/PeriodicTask.h"
#include "model/Context.h"
#include "devices/servo/ServoMotor.h"
#include <Arduino.h>

class DoorTask : public PeriodicTask {
public:
    DoorTask(ServoMotor* pServo, Context* pContext);
    void tick();

private:
    enum DoorState {
        CLOSED,
        OPENING,
        OPEN,
        CLOSING
    } state;

    void setState(DoorState state);
    long elapsedTimeInState();
    bool checkAndSetJustEntered();
    void log(const String& msg);

    long stateTimestamp;
    bool justEntered;

    ServoMotor* pServo;
    Context* pContext;

    int currentPosition;

    void moveToPosition(int targetPosition);
    bool isFullyOpen();
    bool isFullyClosed();
};

#endif