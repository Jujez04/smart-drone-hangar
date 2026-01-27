#ifndef __CONTEXT__
#define __CONTEXT__

#include <Arduino.h>

class Context{
public:
    Context();

    void closeDoor();
    void openDoor();
    void confirmDoorClosed();
    void confirmDoorOpened();

    void confirmDroneInside();
    void confirmDroneOut();
    void confirmDroneNear();
    void confirmDroneFar();

    void confirmTakeOffCommandReceived();
    void clearTakingOffCommand();
    void confirmLandingCommandReceived();
    void clearLandingCommand();

    void triggerPreAlarm();
    void triggerAlarm();
    void confirmResetButtonPressed();
    void clearResetButtonPressed();
    void confirmAlarmSendedToDRU();
    void clearPreAlarm();
    bool isStopped();
    bool isDoorClosed();
    bool isDoorOpen();

    bool isDroneOut();
    bool isDroneInside();
    bool isDroneNear();
    void turnCommandsOff();

    bool isTakeOffCommandReceived();
    bool isLandingCommandReceived();
    bool isPreAlarm();
    bool isAlarm();
    bool isResetButtonPressed();
    bool isAlarmSendedToDRU();
    bool isPreAlarmSendedToDRU();

    String getStatusMessageForDRU();

private:
    bool commandsFlag;
    bool doorOpen;
    bool doorMoving;
    bool droneNear;

    bool droneIsInsideFlag;

    bool takeOffCommandReceived;
    bool landingCommandReceived;

    bool preAlarm;
    bool alarm;
    bool resetButtonPressed;
};

#endif