#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <Arduino.h>

/*** MOTOR CONTROL PINS ***/
constexpr unsigned int X_AXIS_STEP = 12;
constexpr unsigned int X_AXIS_DIR = 11;

constexpr unsigned int Y_AXIS_STEP = 3;
constexpr unsigned int Y_AXIS_DIR = 2;

/*** MOTOR PARAMETERS ***/
constexpr unsigned int X_AXIS_MAX_SPEED = 1000;
constexpr unsigned int Y_AXIS_MAX_SPEED = 1000;

constexpr unsigned int STEPS_PER_REVOLUTION = 200;

constexpr unsigned short int MOTOR_ENABLE_PIN = PIN7;

/*** CONTROL WORDS ***/
constexpr char ENABLE_WORD = 'e';

constexpr char DISABLE_WORD = 'd';

constexpr char READY_RESPONSE = 'r';

constexpr char NEXT_CURVE = 'n';

/*** GANTRY CONSTANTS ***/
constexpr unsigned long A_AXIS_ZERO_OFFSET = 100; // Steps

constexpr unsigned long B_AXIS_ZERO_OFFSET = 0; // Steps

constexpr double PULLEY_DIAMETER = 0.481102; // Inches

/*** MISC ***/

// $0 <= t <= 1$ so each curve will be split up into 100 individual lines here.
constexpr double dT = 0.01;

#endif