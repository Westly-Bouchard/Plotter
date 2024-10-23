#include <Arduino.h>

#include "constants.h"
#include "gantry.h"

// long positions1[2] = {200, 100};
// long positions2[2] = {0, 0};

enum State {
  DISABLED,
  ENABLED
};

struct Curve {
    double cp1x;
    double cp1y;

    double cp2x;
    double cp2y;

    double cp3x;
    double cp3y;

    double cp4x;
    double cp4y;
};

const size_t IMAGE_DESC_SIZE = 16;

struct ImageDesc {
  double width;
  double height;
};

ImageDesc img;

bool needImageDesc = false;

Curve currentCurve;

Gantry gantry;

State currentState;
State previousState;

void setup() {

  // Initialize gantry
  gantry = Gantry();

  // Setup state machine
  currentState = DISABLED;
  previousState = DISABLED;

  Serial.begin(9600);


  pinMode(7, OUTPUT);
  digitalWrite(7, LOW);
}

char input;

void loop() {

  // gantry.moveTo(positions1);
  // gantry.runSpeedToPosition();

  // delay(1000);

  // gantry.moveTo(positions2);
  // gantry.runSpeedToPosition();

  // delay(1000);

  switch (currentState) {
    case State::DISABLED: {
      
      // If the plotter has just been disabled, make sure to disable the stepper motors
      if (previousState == State::ENABLED) {
        gantry.disableSteppers();

        digitalWrite(7, LOW);

        previousState = currentState;
      }

      // Check for control word from slicer
      if (Serial.available() == 1) {
        Serial.readBytes(&input, 1);

        if (input == ENABLE_WORD) {
          previousState = currentState;
          currentState = State::ENABLED;
        }
      }
    }
    break;

    case State::ENABLED: {
      // If this is the first run of the enable logic
      if (previousState == State::DISABLED) {
        digitalWrite(7, HIGH);

        gantry.enableSteppers();

        previousState = currentState;

        Serial.write(READY_RESPONSE);

        needImageDesc = true;
      }

      // If we receive a control word
      if (Serial.available() == 1) {
          Serial.readBytes(&input, 1);

          if (input == DISABLE_WORD) {
            previousState = currentState;
            currentState = State::DISABLED;
          }
      }

      if (needImageDesc && Serial.available() >= IMAGE_DESC_SIZE) {
        char buf[IMAGE_DESC_SIZE];

        Serial.readBytes(buf, IMAGE_DESC_SIZE);

        memcpy(&img, &buf, IMAGE_DESC_SIZE);

        needImageDesc = false;
      }

      
    }
    break;
  }
}