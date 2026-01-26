#ifndef __CONFIG__
#define __CONFIG__

#include "Arduino.h"

// ==========================================
// 1. PIN CONFIGURATION (Wiring)
// ==========================================
#define PIR_PIN 2           // Digital Input
#define BT_PIN 3            // Digital Input (Reset Button)
#define L1_PIN 4            // Digital Output (Green - Inside)
#define L2_PIN 5            // Digital Output (Green - Blinking)
#define L3_PIN 6            // Digital Output (Red - Alarm)
#define SONAR_TRIG_PIN 9    // Digital Output
#define SONAR_ECHO_PIN 10   // Digital Input
#define MOTOR_PIN 11        // PWM Output (Servo)

#define TEMP_PIN A0         // Analog Input (TMP36)
#define SDA_PIN A4          // I2C Data
#define SCL_PIN A5          // I2C Clock

// ==========================================
// 2. HARDWARE PARAMETERS
// ==========================================
// Serial Communication
#define SERIAL_BAUD_RATE 9600

// LCD I2C
#define ADDRESS_LCD 0x27
#define ROW_LCD 4
#define COLUMN_LCD 20

// PIR Sensor
#define PIR_CALIBRATION_TIME 10000 // 10 sec

// Sonar Sensor (HC-SR04)
#define SONAR_TIMEOUT_US 30000     // 30ms
#define SONAR_MIN_DISTANCE 0.02    // 2 cm
#define SONAR_MAX_DISTANCE 4.00    // 4 m

// Servo Motor (Door)
#define DOOR_CLOSED_ANGLE 0
#define DOOR_OPEN_ANGLE 90
#define DOOR_MOVEMENT_TIME 2000

#define D1 0.20 

// D2: Distanza sotto la quale il drone è considerato "ATTERRATO/DENTRO" (es. 15cm)
#define D2 0.15 

// --- TIMEOUT EVENTI (Millisecondi) ---
// T1: Tempo che il drone deve stare > D1 per confermare uscita
#define T1 2000 

// T2: Tempo che il drone deve stare < D2 per confermare atterraggio
#define T2 2000 

// --- TEMPERATURA (Gradi Celsius) ---
// Temp1: Soglia Pre-Allarme
#define TEMP_1 25.0 

// Temp2: Soglia Allarme Critico
#define TEMP_2 30.0 

// T3: Tempo sopra Temp1 per scattare Pre-Allarme (ms)
#define T3 3000

// T4: Tempo sopra Temp2 per scattare Allarme (ms)
#define T4 3000

#define BLINK_PERIOD 500  // Periodo di lampeggio LED (ms)

// ==========================================
// 4. SCHEDULER PERIODS (Millisecondi)
// ==========================================
#define SCHEDULER_BASE_PERIOD 50
#define PER_HANGAR_TASK 100
#define PER_DOOR_TASK 100
#define PER_BLINK_TASK 100
#define PER_SENSORS_TASK 200     // Sonar e PIR un po' più lenti
#define PER_ALARM_TASK 500       // Temp non serve leggerla velocemente

#endif