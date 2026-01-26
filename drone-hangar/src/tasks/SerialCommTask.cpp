#include "tasks/SerialCommTask.h"
#include "kernel/Logger.h"
#include "config.h"

// Definizione Protocollo
const String SerialCommTask::CMD_PREFIX = "ru:";
const String SerialCommTask::APP_PREFIX = "dh:";
const String SerialCommTask::LOG_PREFIX = "lo:";

SerialCommTask::SerialCommTask(Context* pContext) : pContext(pContext) {
    inputBuffer = "";
    inputBuffer.reserve(50); // Prevenzione frammentazione heap
    lastStatusTime = 0;
}

void SerialCommTask::init(int period) {
    PeriodicTask::init(period);
}

void SerialCommTask::tick() {
    // 1. Lettura Input
    readSerialData();

    // 2. Invio Stato Periodico
    if (millis() - lastStatusTime >= STATUS_PERIOD) {
        sendStatusUpdate();
        lastStatusTime = millis();
    }
}

void SerialCommTask::readSerialData() {
    while (Serial.available() > 0) {
        char ch = (char) Serial.read();

        if (ch == '\n') {
            if (inputBuffer.length() > 0) {
                processCommand(inputBuffer);
                inputBuffer = "";
            }
        } 
        else if (ch != '\r') { 
            inputBuffer += ch;
        }
    }
}

void SerialCommTask::processCommand(String msg) {
    msg.trim(); // Rimuove \r, \n e spazi

    // 1. Controllo Prefisso Standard "ru:"
    if (msg.startsWith(CMD_PREFIX)) {
        // Estrai il comando pulito (rimuovi "ru:")
        String cmd = msg.substring(CMD_PREFIX.length());
        
        // 2. Switch sui comandi
        if (cmd == "TAKEOFF") {
            pContext->confirmTakeOffCommandReceived();
            sendLog("CMD TAKEOFF ACCEPTED");
        } 
        else if (cmd == "LANDING") {
            pContext->confirmLandingCommandReceived();
            sendLog("CMD LANDING ACCEPTED");
        } 
        else if (cmd == "RESET") {
            pContext->confirmResetButtonPressed();
            sendLog("CMD RESET ACCEPTED");
        }
        else {
            sendLog("ERROR: Unknown command -> " + cmd);
        }
    } 
    else {
        // Messaggio ignorato (rumore o formato errato)
        // sendLog("Ignored raw msg: " + msg);
    }
}

void SerialCommTask::sendStatusUpdate() {
    // Standard: dh:STATUS:<VALORE>
    String msg = APP_PREFIX + "STATUS:" + pContext->getStatusMessageForDRU();
    Serial.println(msg);
}

void SerialCommTask::sendLog(const String& msg) {
    // Standard: lo:<MESSAGGIO>
    Serial.println(LOG_PREFIX + msg);
}