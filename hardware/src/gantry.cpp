
#include "constants.h"
#include "gantry.h"

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