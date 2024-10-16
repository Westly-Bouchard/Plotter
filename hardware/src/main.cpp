#include <Arduino.h>

#include "constants.h"
#include "gantry.h"

// long positions1[2] = {200, 100};
// long positions2[2] = {0, 0};

enum State {
  IDLE,
  DISABLED,
  JOG,
  ENABLED
};

Gantry gantry;

State currentState;
State previousState;

void setup() {

  // Initialize gantry
  gantry = Gantry();

  // Setup state machine
  currentState = IDLE;
  previousState = IDLE;

  Serial.begin(9600);


  pinMode(7, OUTPUT);
  digitalWrite(7, LOW);
}

String input;

void loop() {
  // switch (currentState) {
  //   case IDLE: {

  //   }
  //   break;

  //   case DISABLED: {

  //   }
  //   break;

  //   case JOG: {

  //   }
  //   break;

  //   case ENABLED: {

  //   }
  //   break;
  // }
  // gantry.moveTo(positions1);
  // gantry.runSpeedToPosition();

  // delay(1000);

  // gantry.moveTo(positions2);
  // gantry.runSpeedToPosition();

  // delay(1000);

  if (Serial.available()) {
    input = Serial.readString();
    Serial.write(input.c_str());
    if (input == "e") {

      digitalWrite(7, HIGH);

      delay(2000);

      digitalWrite(7, LOW);

    }

    // if (input == "e") {
    //   digitalWrite(LED_BUILTIN, HIGH); 
    // }
 }
}