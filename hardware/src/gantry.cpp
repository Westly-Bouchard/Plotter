#include <math.h>

#include "constants.h"
#include "gantry.h"
#include "util.hpp"

Gantry::Gantry() : xPos{0.0}, yPos{0.0}, enabled{false}, enablePin{MOTOR_ENABLE_PIN} {

    /* Initialize Stepper Motors */
    m_A = AccelStepper(
        1,
        X_AXIS_STEP,
        X_AXIS_DIR
    );

    m_B = AccelStepper(
        1,
        Y_AXIS_STEP,
        Y_AXIS_DIR
    );

    /* Create Gantry Object */

    coreXY = MultiStepper();

    coreXY.addStepper(m_A);
    coreXY.addStepper(m_B);

    /* Setup Driver Enable Control */
    pinMode(enablePin, OUTPUT);
    
    disableSteppers();

}

void Gantry::disableSteppers() {
    digitalWrite(enablePin, HIGH);
}

void Gantry::enableSteppers() {
    digitalWrite(enablePin, LOW);
}

bool Gantry::takeMovementStep() {
    return coreXY.run();
}

// This logic needs to be condensed
void Gantry::setTargetPosition(double x, double y) {
    /*
    * Positions are given in inches
    * So we first have to find the delta in cartesian inches
    * Then convert to a delta in coreXY inches
    * Then convert those linear inch deltas to stepper motor positions
    */

   // Current motor positions in steps
   long currentA = m_A.currentPosition() - A_AXIS_ZERO_OFFSET;
   long currentB = m_B.currentPosition() - B_AXIS_ZERO_OFFSET;

   // Convert to linear inches
   // First find number of revolutions
   long revsA = currentA / STEPS_PER_REVOLUTION;
   long revsB = currentB / STEPS_PER_REVOLUTION;

   // Every revolution is PI*Diameter inches
   double inchesA = revsA * PI * PULLEY_DIAMETER;
   double inchesB = revsB * PI * PULLEY_DIAMETER;

   // Now convert these inch coreXY deltas to cartesian deltas
   double inchesX = coreToCart_X(inchesA, inchesB);
   double inchesY = coreToCart_Y(inchesA, inchesB);

   // Next find difference between target position and current position
   double deltaX = x - inchesX;
   double deltaY = y - inchesY;

   // Now we have to convert that back to coreXY
   // These are the desired number of linear inches that we want each belt to move
   double deltaA = cartToCore_A(deltaX, deltaY);
   double deltaB = cartToCore_B(deltaX, deltaY);

   // Next we have to convert these from inches to steps
   revsA = deltaA / (PI * PULLEY_DIAMETER);
   revsB = deltaB / (PI * PULLEY_DIAMETER);

   long stepsA = round(revsA * STEPS_PER_REVOLUTION);
   long stepsB = round(revsB * STEPS_PER_REVOLUTION);

   // Now we need to convert these delta steps to absolute step positions
   // We do this by adding them to the current positions
   long targetA = currentA + stepsA;
   long targetB = currentB + stepsB;

   // Now update the target position of the gantry
   long target[2] = {targetA, targetB};
   coreXY.moveTo(target);

}