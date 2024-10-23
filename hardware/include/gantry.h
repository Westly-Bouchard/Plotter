#include <AccelStepper.h>
#include <MultiStepper.h>

class Gantry {
    private:
        AccelStepper m_A;
        AccelStepper m_B;

        MultiStepper coreXY;

        float xPos;
        float yPos;

        bool enabled;

        int enablePin;

    public:
        Gantry();

        void enableSteppers();

        void disableSteppers();

        void setTargetPosition(double x, double y);

        bool takeMovementStep();
};