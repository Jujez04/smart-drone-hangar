#include "kernel/Logger.h"
#include <Arduino.h>
#include "config.h"

void LoggerService::init() {
    // If Serial is already open (from main.cpp), do nothing.
    // If not, open it safely.
    if (!Serial) {
        Serial.begin(9600);
        delay(100);
    }
}

void LoggerService::log(const String& msg){
    Serial.println("lo:" + msg);
}