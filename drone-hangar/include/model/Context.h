#ifndef __CONTEXT__
#define __CONTEXT__

class Context {

public:
    void closeDoor();
    void openDoor();
    void confirmDoorClosed();
    void confirmDoorOpened();
    void confirmDroneInside();
    void confirmDroneOut();
    void confirmDroneNear();
    void confirmDroneFar();
    void confirmTakeOffCommandReceived();
    void confirmLandingCommandReceived();

    bool isStopped(); // Blink: Think about it
    bool isDoorClosed();
    bool isDoorOpen();
    bool isDroneOut();
    bool isDroneInside();
    bool isDroneNear();
    bool isTakeOffCommandReceived();
    bool isLandingCommandReceived();
private:
    bool doorOpen = true;
    bool doorMoving;

    //PIR
    bool droneNear = false;

    bool takeOffCommandReceived = false;
    bool landingCommandReceived = false;

    bool isPreAlarm;
    bool isAlarm;
};

#endif