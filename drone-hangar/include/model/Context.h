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
    void disableCommands();
    void enableCommands();
    void triggerPreAlarm();
    void triggerAlarm();
    void confirmResetButtonPressed();
    void clearResetButtonPressed();
    void clearPreAlarm();
    void clearAlarm();
    bool isDoorClosed();
    bool isDoorOpen();
    bool isCommandsEnabled();
    bool isDroneOut();
    bool isDroneInside();
    bool isDroneNear();

    bool isTakeOffCommandReceived();
    bool isLandingCommandReceived();
    bool isPreAlarm();
    bool isAlarm();
    void setDistance(float d);
    float getDistance();
    const char* getStatusMessageForDRU();

private:
    bool commandsFlag;
    bool doorOpen;
    bool doorMoving;
    bool droneNear;

    bool droneIsInsideFlag;

    bool takeOffCommandReceived;
    bool landingCommandReceived;
    float currentDistance;
    bool preAlarm;
    bool alarm;
    bool resetButtonPressed;
};

#endif