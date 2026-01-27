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
  // pPir->calibrate();
  pMotor->on();
  pMotor->setPosition(0);
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

LcdI2C *HWPlatform::getLcd() {
  return pLcd;
}

void HWPlatform::test() {
  pPir->sync();
  bool pirDetected = pPir->isDetected();
  float temp = pTemp->getTemperature();
  float dist = pSonar->getDistance();
  bool btnPressed = pButton->isPressed();

  if (pirDetected)
    pL1->switchOn();
  else
    pL1->switchOff();

  if (btnPressed)
    pL2->switchOn();
  else
    pL2->switchOff();

  static bool heartbeat = false;
  heartbeat = !heartbeat;
  if (heartbeat)
    pL3->switchOn();
  else
    pL3->switchOff();

  String logMsg = String("Temp:") + temp +
                  " | Dist:" + dist +
                  " | PIR:" + (pirDetected ? "SI" : "NO") +
                  " | BTN:" + (btnPressed ? "SI" : "NO");
  Logger.log(logMsg);
}

void HWPlatform::testServo() {
  enum ServoTestState {
    CLOSED,
    OPENING,
    OPEN,
    CLOSING
  };

  static ServoTestState state = CLOSED;
  static unsigned long stateTime = 0;
  static bool entered = true;
  static bool lastBtnState = false;
  bool currentBtnState = pButton->isPressed();
  bool btnPressedNow = currentBtnState && !lastBtnState;
  lastBtnState = currentBtnState;

  unsigned long elapsed = millis() - stateTime;

  switch (state) {
  case CLOSED:
    if (entered) {
      pMotor->setPosition(0);
      Logger.log("[Servo] CLOSED (0 deg)");
      entered = false;
    }
    if (btnPressedNow) {
      state = OPENING;
      stateTime = millis();
      entered = true;
    }
    break;

  case OPENING:
    if (entered) {
      pMotor->setPosition(180);
      Logger.log("[Servo] OPENING -> 180 deg");
      entered = false;
    }
    if (elapsed >= 2000) {
      state = OPEN;
      stateTime = millis();
      entered = true;
    }
    break;

  case OPEN:
    if (entered) {
      Logger.log("[Servo] OPEN (Waiting...)");
      entered = false;
    }
    if (btnPressedNow || elapsed >= 2000) {
      state = CLOSING;
      stateTime = millis();
      entered = true;
    }
    break;

  case CLOSING:
    if (entered) {
      pMotor->setPosition(0);
      Logger.log("[Servo] CLOSING -> 0 deg");
      entered = false;
    }
    if (elapsed >= 2000) {
      state = CLOSED;
      stateTime = millis();
      entered = true;
    }
    break;
  }
}