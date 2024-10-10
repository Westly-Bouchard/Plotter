#include <Arduino.h>

#include <AccelStepper.h>
#include <MultiStepper.h>

#include "constants.h"
#include "gantry.h"

long positions1[2] = {200, 100};
long positions2[2] = {0, 0};

Gantry gantry;

void setup() {

  // Initialize gantry
  gantry = Gantry();

  Serial.begin(9600);
}

String input;

void loop() {
  // gantry.moveTo(positions1);
  // gantry.runSpeedToPosition();

  // delay(1000);

  // gantry.moveTo(positions2);
  // gantry.runSpeedToPosition();

  // delay(1000);

  // if (Serial.available()) {
  //   input = Serial.readString();

  //   if (input == "e") {

  //     Serial.write(input.c_str());

  //     gantry.moveTo(positions1);
      
  //   } else if (input == "d") {

  //     Serial.write(input.c_str());

  //     gantry.moveTo(positions2);

  //   }
  // }

  // gantry.runSpeedToPosition();

}