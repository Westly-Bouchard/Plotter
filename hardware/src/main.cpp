#include <Arduino.h>

#include <AccelStepper.h>
#include <MultiStepper.h>

#include "constants.h"

AccelStepper xAxis(
  1,
  X_AXIS_STEP,
  X_AXIS_DIR
);

AccelStepper yAxis(
  1,
  Y_AXIS_STEP,
  Y_AXIS_DIR
);

MultiStepper gantry;

long positions1[2] = {200, 100};
long positions2[2] = {0, 0};

void setup() {
  xAxis.setMaxSpeed(X_AXIS_MAX_SPEED);

  yAxis.setMaxSpeed(Y_AXIS_MAX_SPEED);

  gantry.addStepper(xAxis);
  gantry.addStepper(yAxis);
}

void loop() {
  gantry.moveTo(positions1);
  gantry.runSpeedToPosition();

  delay(1000);

  gantry.moveTo(positions2);
  gantry.runSpeedToPosition();

  delay(1000);
}