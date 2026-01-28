#ifndef __CONFIG__
#define __CONFIG__

#include "Arduino.h"

// PIN CONFIGURATION
#define PIR_PIN 2         // Digital Input
#define BT_PIN 3          // Digital Input (Reset Button)
#define L1_PIN 4          // Digital Output (Green - Inside)
#define L2_PIN 5          // Digital Output (Green - Blinking)
#define L3_PIN 6          // Digital Output (Red - Alarm)
#define SONAR_TRIG_PIN 9  // Digital Output
#define SONAR_ECHO_PIN 10 // Digital Input
#define MOTOR_PIN 11      // PWM Output (Servo)

#define TEMP_PIN A0 // Analog Input (TMP36)
#define SDA_PIN A4  // I2C Data
#define SCL_PIN A5  // I2C Clock

// SYSTEM CONFIGURATION

// Serial Communication
#define SERIAL_BAUD_RATE 9600

// LCD I2C
#define ADDRESS_LCD 0x27
#define ROW_LCD 4
#define COLUMN_LCD 20

// PIR Sensor
#define PIR_CALIBRATION_TIME 10000 // 10 sec

// Sonar Sensor (HC-SR04)
#define SONAR_TIMEOUT_US 30000  // 30ms
#define SONAR_MIN_DISTANCE 0.02 // 2 cm
#define SONAR_MAX_DISTANCE 4.00 // 4 m

// Servo Motor (Door)
#define DOOR_CLOSED_ANGLE 0
#define DOOR_OPEN_ANGLE 90
#define DOOR_MOVEMENT_TIME 2000

#define D1 0.20 // 20 cm
#define D2 0.15 // 15 cm

#define T1 2000 // ms
#define T2 2000 // ms

// Alarm Factors
#define TEMP_1 2.0
#define TEMP_2 3.0
#define T3 10000
#define T4 10000

#define BLINK_PERIOD 500

#define SCHEDULER_BASE_PERIOD 50
#define PER_HANGAR_TASK 100
#define PER_DOOR_TASK 100
#define PER_SENSORS_TASK 200
#define PER_ALARM_TASK 500

#endif