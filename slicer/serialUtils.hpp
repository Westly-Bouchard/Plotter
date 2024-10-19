//
// Created by Westly Bouchard on 10/15/24.
//

#ifndef SLICER_SERIALUTILS_H
#define SLICER_SERIALUTILS_H

#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <termios.h>
#include <unistd.h>

int openSerialPort(const char* portName) {
    int fd = open(portName, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 1) {
        std::cerr << "Error opening serial connection: " << portName << std::endl;
        return -1;
    }

    return fd;
}

bool configureSerialPort(int fd, int speed) {
    struct termios tty;
    if (tcgetattr(fd, &tty) != 0) {
        std::cerr << "Error from tcgetattr" << std::endl;
        return false;
    }

    // Set baud rate
    cfsetospeed(&tty, speed);
    cfsetispeed(&tty, speed);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // Use 8-bit characters
    tty.c_iflag &= ~IGNBRK; // Disable break processing
    tty.c_lflag = 0; // No signaling chars, no echo, no canonical processing
    tty.c_oflag = 0; // No remapping, no delays
    tty.c_cc[VMIN] = 0; // Read doesn't block
    tty.c_cc[VTIME] = 5; // 0.5 seconds for read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Shut off xon/xoff control

    tty.c_cflag |= (CLOCAL | CREAD); // Ignore modem controls, enable reading

    tty.c_cflag &= ~(PARENB | PARODD); // Shut off parity
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        std::cerr << "Error from tcsetattr" << std::endl;
        return false;
    }

    return true;
}

int readFromSerialPort(int fd, char *buffer, size_t size) {
    return read(fd, buffer, size);
}

int writeToSerialPort(int fd, const char *buffer, size_t size) {
    return write(fd, buffer, size);
}

void closeSerialPort(int fd) { close(fd); }

// Example reference for how to read from the serial port
//char buffer[100];
//    int n = readFromSerialPort(fd, buffer, sizeof(buffer));
//    if (n < 0) {
//        cerr << "Error reading from serial port" << endl;
//    } else {
//        cout << "Read form serial port: " << string(buffer, n) << endl;
//    }

class SerialConnection {
private:
    int fd;
    const char* portName;

public:
    explicit SerialConnection(const char* portName) {
        this->portName = portName;

        fd = -1;
    }

    bool open() {
        fd = openSerialPort(portName);

        if (fd < 0 || !configureSerialPort(fd, B9600)) {
            return false;
        }

        return true;
    }

    int writeChar(char ctrl) {
        return writeToSerialPort(fd, &ctrl, 1);
    }

    int getFD() const { return fd; }
};

#endif //SLICER_SERIALUTILS_H
