
double motorToCore_X(double deltaA, double deltaB) {
    return 0.5 * (deltaA + deltaB);
}

double motorToCore_Y(double deltaA, double deltaB) {
    return 0.5 * (deltaA - deltaB);
}

double coreToMotor_A(double deltaX, double deltaY) {
    return deltaX + deltaY;
}

double coreToMotor_B(double deltaX, double deltaY) {
    return deltaX - deltaY;
}