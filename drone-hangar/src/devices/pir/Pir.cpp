#include "devices/pir/Pir.h"
#include "Arduino.h"

Pir::Pir(int pin){
  this->pin = pin;
  this->detected = false;
  pinMode(pin, INPUT);
}

void Pir::sync(){
  detected = digitalRead(pin) == HIGH;
  updateSyncTime(millis());
}

bool Pir::isDetected(){
  return detected;
}

void Pir::calibrate(){
	// delay(10000); --> Moved into the PresenceDetectorTask
}

void Pir::updateSyncTime(long time){
	lastTimeSync = time;
}

long Pir::getLastSyncTime(){
	return lastTimeSync;
}