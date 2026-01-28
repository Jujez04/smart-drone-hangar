#include "kernel/Logger.h"
#include <Arduino.h>
#include "config.h"

void LoggerService::init() {
    Serial.begin(SERIAL_BAUD_RATE);
    delay(100);
}

void LoggerService::log(const String& msg){
    Serial.println("lo:" + msg);
}