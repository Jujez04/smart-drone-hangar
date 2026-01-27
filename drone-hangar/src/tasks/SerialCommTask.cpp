#include "tasks/SerialCommTask.h"
#include "kernel/Logger.h"
#include "config.h"

SerialCommTask::SerialCommTask(Context *pContext) : pContext(pContext)
{
    inputBuffer = "";
    inputBuffer.reserve(50); // Riserva memoria una volta sola
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
        if (ch == '\n')
        {
            if (inputBuffer.length() > 0)
            {
                processCommand(inputBuffer);
                inputBuffer = "";
            }
        }
        else if (ch != '\r')
        {
            inputBuffer += ch;
        }
    }
}

void SerialCommTask::processCommand(String msg)
{
    msg.trim();
    if (msg.startsWith(CMD_PREFIX))
    {
        String cmd = msg.substring(CMD_PREFIX.length());
        if (cmd == "TAKEOFF")
        {
            pContext->confirmTakeOffCommandReceived();

            Serial.println(F("lo:CMD TAKEOFF ACCEPTED"));
        }
        else if (cmd == "LANDING")
        {
            pContext->confirmLandingCommandReceived();
            Serial.println(F("lo:CMD LANDING ACCEPTED"));
        }
        else if (cmd == "RESET")
        {
            pContext->confirmResetButtonPressed();
            Serial.println(F("lo:CMD RESET ACCEPTED"));
        }
        else
        {
            Serial.print(F("lo:ERROR: Unknown command -> "));
            Serial.println(cmd);
        }
    }
}

void SerialCommTask::sendStatusUpdate()
{

    Serial.print(F("dh:STATUS:"));
    Serial.println(pContext->getStatusMessageForDRU());
}

void SerialCommTask::log(const String &msg)
{
    // Versione ottimizzata per messaggi dinamici
    Serial.print(F("lo:"));
    Serial.println(msg);
}