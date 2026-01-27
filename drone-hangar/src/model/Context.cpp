#include "model/Context.h"

Context::Context()
{
    doorOpen = false;
    doorMoving = false;
    droneNear = false;
    droneIsInsideFlag = true;
    takeOffCommandReceived = false;
    landingCommandReceived = false;
    preAlarm = false;
    alarm = false;
    resetButtonPressed = false;
    commandsFlag = true;
}

void Context::closeDoor()
{
    doorMoving = true;
}

void Context::clearPreAlarm() {
    preAlarm = false;
}

void Context::clearAlarm() {
    alarm = false;
}

void Context::disableCommands() {
    commandsFlag = false;
}

void Context::enableCommands() {
    commandsFlag = true;
}

void Context::openDoor()
{
    doorMoving = true;
}

void Context::confirmDoorClosed()
{
    doorMoving = false;
    doorOpen = false;
}
void Context::confirmDoorOpened()
{
    doorMoving = false;
    doorOpen = true;
}

bool Context::isDoorClosed()
{
    return !doorOpen && !doorMoving;
}

bool Context::isDoorOpen()
{
    return doorOpen && !doorMoving;
}

bool Context::isStopped()
{
    return !doorMoving;
}

void Context::confirmDroneInside()
{
    droneIsInsideFlag = true;
}

void Context::confirmDroneOut()
{
    droneIsInsideFlag = false;
    droneNear = false;
}

void Context::clearTakingOffCommand(){
    takeOffCommandReceived = false;
}

void Context::clearLandingCommand(){
    landingCommandReceived = false;
}

bool Context::isDroneInside()
{
    return droneIsInsideFlag;
}

bool Context::isDroneOut()
{
    return !droneIsInsideFlag;
}

void Context::confirmDroneNear()
{
    droneNear = true;
}

void Context::confirmDroneFar()
{
    droneNear = false;
}

bool Context::isDroneNear()
{
    return droneNear;
}

void Context::confirmTakeOffCommandReceived()
{
    takeOffCommandReceived = true;
}

bool Context::isCommandsEnabled() {
    return commandsFlag;
}

bool Context::isTakeOffCommandReceived()
{
    return takeOffCommandReceived;
}

void Context::confirmLandingCommandReceived()
{
    landingCommandReceived = true;
    droneNear = false;
}

bool Context::isLandingCommandReceived()
{
    return landingCommandReceived;
}

void Context::triggerPreAlarm()
{
    preAlarm = true;
}

bool Context::isPreAlarm()
{
    return preAlarm;
}

void Context::triggerAlarm()
{
    alarm = true;
    preAlarm = false;
}

bool Context::isAlarm()
{
    return alarm;
}

void Context::confirmResetButtonPressed()
{
    resetButtonPressed = true;
    alarm = false;
    preAlarm = false;
}

void Context::clearResetButtonPressed()
{
    resetButtonPressed = false;
}

bool Context::isResetButtonPressed()
{
    return resetButtonPressed;
}

String Context::getStatusMessageForDRU()
{
    if (alarm)
        return "ALARM";
    if (takeOffCommandReceived)
        return "TAKING_OFF";
    if (landingCommandReceived)
        return "LANDING";
    if (!droneIsInsideFlag)
        return "DRONE_OUT";
    return "DRONE_INSIDE";
}