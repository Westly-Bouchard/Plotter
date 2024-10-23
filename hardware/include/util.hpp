
double coreToCart_X(double deltaA, double deltaB) {
    return 0.5 * (deltaA + deltaB);
}

double coreToCart_Y(double deltaA, double deltaB) {
    return 0.5 * (deltaA - deltaB);
}

double cartToCore_A(double deltaX, double deltaY) {
    return deltaX + deltaY;
}

double cartToCore_B(double deltaX, double deltaY) {
    return deltaX - deltaY;
}