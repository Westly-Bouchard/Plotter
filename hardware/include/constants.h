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

constexpr unsigned short int MOTOR_ENABLE_PIN = PIN7;

/*** CONTROL WORDS ***/
constexpr unsigned char DISABLED = 0x44;

constexpr unsigned char JOG = 0x4A;

constexpr unsigned char ENABLE = 0x45;

#endif