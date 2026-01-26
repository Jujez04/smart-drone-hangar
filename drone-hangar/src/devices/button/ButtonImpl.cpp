#include "devices/button/ButtonImpl.h"
#include <Arduino.h>

ButtonImpl::ButtonImpl(int pin){
  this->pin = pin;
  pinMode(pin, INPUT);
  
  lastDebounceTime = 0;
  lastButtonState = HIGH;
  buttonState = HIGH;
}

bool ButtonImpl::isPressed(){
  int reading = digitalRead(pin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    if (reading != buttonState) {
      buttonState = reading;
    }
  }

  lastButtonState = reading;
  return buttonState == HIGH;
}