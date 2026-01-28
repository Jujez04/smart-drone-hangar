#include "tasks/SerialCommTask.h"
#include "kernel/Logger.h"
#include "config.h"

SerialCommTask::SerialCommTask(Context *pContext) : pContext(pContext)
{
    inputBuffer = "";
    inputBuffer.reserve(50);
    lastStatusTime = 0;
}

void SerialCommTask::init(int period)
{
    PeriodicTask::init(period);
}

void SerialCommTask::tick()
{
    readSerialData();
    if (millis() - lastStatusTime >= STATUS_PERIOD)
    {
        sendStatusUpdate();
        lastStatusTime = millis();
    }
}

void SerialCommTask::readSerialData()
{
    while (Serial.available() > 0)
    {
        char ch = (char)Serial.read();
        if (ch == '\n') {
            if (inputBuffer.length() > 0) {
                processCommand(inputBuffer);
                inputBuffer = "";
            }
        } else if (ch != '\r') {
            inputBuffer += ch;
        }
    }
}

void SerialCommTask::processCommand(String msg)
{
    msg.trim();
    if (msg.startsWith(CMD_PREFIX)) {
        String cmd = msg.substring(CMD_PREFIX.length());
        if (cmd == "TAKEOFF") {
            pContext->confirmTakeOffCommandReceived();
            Serial.println(F("lo:CMD TAKEOFF"));
        } else if (cmd == "LANDING") {
            pContext->confirmLandingCommandReceived();
            Serial.println(F("lo:CMD LANDING"));
        } else if (cmd == "RESET") {
            pContext->confirmResetButtonPressed();
            Serial.println(F("lo:CMD RESET"));
        } else {
            Serial.print(F("lo:ERR CMD:"));
            Serial.println(cmd);
        }
    }
}

void SerialCommTask::sendStatusUpdate()
{
    if (Serial.availableForWrite() < 32) {
        return;
    }
    Serial.print(F("dh:STATUS:"));
    Serial.println(pContext->getStatusMessageForDRU());
    Serial.print(F("dh:DIST:"));
    Serial.println(pContext->getDistance());
}

void SerialCommTask::log(const String &msg)
{
    Serial.print(F("lo:"));
    Serial.println(msg);
}