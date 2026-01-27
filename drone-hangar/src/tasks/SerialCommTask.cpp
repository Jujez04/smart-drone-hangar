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
            log("CMD TAKEOFF ACCEPTED");
        }
        else if (cmd == "LANDING")
        {
            pContext->confirmLandingCommandReceived();
            log("CMD LANDING ACCEPTED");
        }
        else if (cmd == "RESET")
        {
            pContext->confirmResetButtonPressed();
            log("CMD RESET ACCEPTED");
        }
        else
        {
            log("ERROR: Unknown command -> " + cmd);
        }
    }
    else
    {
        // Messaggio ignorato (rumore o formato errato)
        log("Ignored raw msg: " + msg);
    }
}

void SerialCommTask::sendStatusUpdate()
{
    // Standard: dh:STATUS:<VALORE>
    String msg = APP_PREFIX + "STATUS:" + pContext->getStatusMessageForDRU();
    Serial.println(msg);
}

void SerialCommTask::log(const String &msg)
{
    Serial.println(LOG_PREFIX + msg);
}