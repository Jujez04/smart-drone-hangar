#include "kernel/Logger.h"
#include <Arduino.h>
#include "config.h"

void LoggerService::init() {
    if (!Serial) {
        Serial.begin(9600);
        delay(100);
    }
}

void LoggerService::log(const String& msg){
    Serial.println("lo:" + msg);
}