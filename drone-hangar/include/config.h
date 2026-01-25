#ifndef __CONFIG__
#define __CONFIG__

#include "Arduino.h"

#define PIR_PIN 2
#define BT_PIN 3
#define L1_PIN 4
#define L2_PIN 5
#define L3_PIN 6
#define SONAR_TRIG_PIN 9
#define SONAR_ECHO_PIN 10
#define MOTOR_PIN 11

#define TEMP_PIN A0
#define SDA_PIN A4
#define SCL_PIN A5

#define PIR_CALIBRATION_TIME 10000

#define SONAR_TIMEOUT_US 30000
#define ADDRESS_LCD 0x27
#define ROW_LCD 4
#define COLUMN_LCD 20

#define DOOR_CLOSED_ANGLE 0
#define DOOR_OPEN_ANGLE 180
#define DOOR_MOVEMENT_TIME 1000

#define SONAR_MIN_DISTANCE 3 // 3 cm
#define SONAR_MAX_DISTANCE 150 // 1 m

#define D1 80
#define T1 1000 // 10 sec
#define D2 10
#define T2 1000 // 10 sec

#endif