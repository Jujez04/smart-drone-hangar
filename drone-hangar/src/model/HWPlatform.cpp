#include "model/HWPlatform.h"
#include "Arduino.h"
#include "devices/button/ButtonImpl.h"
#include "kernel/MsgService.h"
#include "config.h"
#include "devices/led/Led.h"
#include "devices/servo/ServoMotorImpl.h"
#include "kernel/Logger.h"

void wakeUp(){}

HWPlatform::HWPlatform(){
  pButton = new ButtonImpl(BT_PIN);
  pMotor = new ServoMotorImpl(MOTOR_PIN);
}


void HWPlatform::init(){
}

Button* HWPlatform::getButton(){
  return this->pButton;
}


Led*  HWPlatform::getLed(){
  return this->pLed;
}

ServoMotor* HWPlatform::getMotor(){
  return this->pMotor;
}

void HWPlatform::test(){
  bool btPressed = pButton->isPressed();
  pLed->switchOn();
  pMotor->on();
  pMotor->setPosition(90);
  Logger.log("Button: " + String(btPressed ? "pressed" : " not pressed"));
  delay(1000);
  pMotor->setPosition(0);
  delay(1000);
  pMotor->off();
  pLed->switchOff();
}