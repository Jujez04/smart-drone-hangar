#include "model/HWPlatform.h"

#include <Arduino.h>

#include "config.h"

#include "devices/button/ButtonImpl.h"
#include "devices/servo/ServoMotorImpl.h"
#include "devices/temperature/TempSensorTMP36.h"
#include "devices/lcd/LcdI2C.h"

#include "kernel/Logger.h"

HWPlatform::HWPlatform() {
  pButton = new ButtonImpl(BT_PIN);
  pMotor = new ServoMotorImpl(MOTOR_PIN);

  pL1 = new Led(L1_PIN);
  pL2 = new Led(L2_PIN);
  pL3 = new Led(L3_PIN);

  pPir = new Pir(PIR_PIN);
  pSonar = new Sonar(SONAR_ECHO_PIN, SONAR_TRIG_PIN, SONAR_TIMEOUT_US);
  pTemp = new TempSensorTMP36(TEMP_PIN);

  pLcd = new LcdI2C(ADDRESS_LCD, COLUMN_LCD, ROW_LCD);
}

void HWPlatform::init() {
  pLcd->init();
  pLcd->turnOn();
  pPir->calibrate();
  pMotor->on();
  pMotor->setPosition(0);
  pMotor->off();
}

Button *HWPlatform::getButton() {
  return pButton;
}

ServoMotor *HWPlatform::getMotor() {
  return pMotor;
}

Led *HWPlatform::getL1() {
  return pL1;
}

Led *HWPlatform::getL2() {
  return pL2;
}

Led *HWPlatform::getL3() {
  return pL3;
}

Pir *HWPlatform::getPir() {
  return pPir;
}

Sonar *HWPlatform::getSonar() {
  return pSonar;
}

TempSensor *HWPlatform::getTempSensor() {
  return pTemp;
}

Display *HWPlatform::getLcd() {
  return pLcd;
}

void HWPlatform::test() {

  pLcd->setCursor(0, 0);
  pLcd->print("TEST");

  pMotor->on();
  pMotor->setPosition(180);
  delay(500);
  pMotor->setPosition(0);

  pL1->switchOn();
  delay(500);
  pL1->switchOff();

  pL2->switchOn();
  delay(500);
  pL2->switchOff();

  pL3->switchOn();
  delay(500);
  pL3->switchOff();

  float t = pTemp->getTemperature();
  float s = pSonar->getDistance();
  pPir->sync();
  const char *p = (pPir->isDetected() ? "RILEVATO" : "NON RILEVATO");
  const char *b = (pButton->isPressed() ? "PREMUTO" : "NON PREMUTO");
  Logger.log("temp: " + String(t) + " | dist: " + String(s) + " | pir: " + p + " | btn: " + b);
}