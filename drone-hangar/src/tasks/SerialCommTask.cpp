#include "tasks/SerialCommTask.h"
#include "kernel/Logger.h"
#include "config.h"

const String SerialCommTask::APP_PREFIX = "dh:";
const String SerialCommTask::LOG_PREFIX = "lo:";

SerialCommTask::SerialCommTask(Context* pContext)
    : pContext(pContext) {
    setState(LISTENING);
    inputBuffer = "";
    inputBuffer.reserve(256);
}

void SerialCommTask::init() {
    AperiodicTask::init();
    Serial.begin(SERIAL_BAUD_RATE);
    log("Serial initialized");
}

void SerialCommTask::tick() {
    // Always check for serial data
    readSerial();

    switch (state) {
        case LISTENING: {
            if (checkAndSetJustEntered()) {
                // Ready
            }
            // Check if we need to send alarm
            if (pContext->isAlarm() && !pContext->isAlarmSendedToDRU()) {
                setState(SENDING);
            }

            break;
        }
        
        case PROCESSING: {
            if (checkAndSetJustEntered()) {
                // Command processed
            }
            setState(LISTENING);
            break;
        }
        
        case SENDING: {
            if (checkAndSetJustEntered()) {
                if (pContext->isAlarm() && !pContext->isAlarmSendedToDRU()) {
                    sendAlarmToDRU();
                    pContext->confirmAlarmSendedToDRU();
                }
                sendStatusToDRU();
            }
            setState(LISTENING);
            break;
        }
    }
}

void SerialCommTask::readSerial() {
    while (Serial.available() > 0) {
        char ch = Serial.read();
        if (ch == '\n') {
            if (inputBuffer.length() > 0) {
                handleIncomingMessage(inputBuffer);
                inputBuffer = "";
                setState(PROCESSING);
            }
        } else if (ch != '\r') {
            inputBuffer += ch;
        }
    }
}

void SerialCommTask::handleIncomingMessage(const String& msg) {
    String command;
    if (!parseCommand(msg, command)) {
        log("Invalid message: " + msg);
        return;
    }
    log("Received: " + command);

    if (command == "TAKEOFF") {
        pContext->confirmTakeOffCommandReceived();
        sendLogMessage("Takeoff command received");

    } else if (command == "LANDING") {
        pContext->confirmLandingCommandReceived();
        sendLogMessage("Landing command received");

    } else if (command == "STATUS") {
        sendStatusToDRU();

    } else if (command == "RESET") {
        pContext->confirmResetButtonPressed();
        sendLogMessage("Remote reset triggered");

    } else {
        log("Unknown command: " + command);
    }
}

bool SerialCommTask::parseCommand(const String& msg, String& command) {
    int colonIndex = msg.indexOf(':');

    if (colonIndex > 0) {
        String prefix = msg.substring(0, colonIndex);
        if (prefix == "CMD") {
            command = msg.substring(colonIndex + 1);
            command.trim();
            return command.length() > 0;
        }
        return false;
    } else {
        command = msg;
        command.trim();
        return command.length() > 0;
    }
}

void SerialCommTask::sendStatusToDRU() {
    String status = APP_PREFIX + "STATUS:" + pContext->getStatusMessageForDRU();
    Serial.println(status);
}

void SerialCommTask::sendAlarmToDRU() {
    String alarmMsg = APP_PREFIX + "ALARM";
    Serial.println(alarmMsg);
    sendLogMessage("CRITICAL ALARM!");
}

void SerialCommTask::sendLogMessage(const String& msg) {
    Serial.println(LOG_PREFIX + msg);
}

void SerialCommTask::setState(SerialCommState s) {
    state = s;
    stateTimestamp = millis();
    justEntered = true;
}

long SerialCommTask::elapsedTimeInState() {
    return millis() - stateTimestamp;
}

bool SerialCommTask::checkAndSetJustEntered() {
    bool result = justEntered;
    if (justEntered) {
        justEntered = false;
    }
    return result;
}

void SerialCommTask::log(const String& msg) {
    Logger.log("[SerialComm] " + msg);
}