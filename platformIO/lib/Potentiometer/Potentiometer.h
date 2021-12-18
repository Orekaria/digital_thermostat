#include <Arduino.h>
#include <Shared.h>
#include <CircularBuffer.h>

#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

class Potentiometer
{
private:
    uint8_t _pin;
    CircularBuffer _circularBuffer;
public:
    Potentiometer(uint8_t pin);
    ~Potentiometer();
    float readPotentiometer();
};

#endif

Potentiometer::Potentiometer(uint8_t pin) {
    _pin = pin;
    _circularBuffer = CircularBuffer();
    pinMode(_pin, INPUT);
}

Potentiometer::~Potentiometer() {
}

float Potentiometer::readPotentiometer() {
    long potReading = analogRead(_pin);
    // Utils::log("pot: " + String(potReading) + ", ");
    long potDegreesMapped = map(potReading, 1023, 0, (MIN_TEMPERATURE * 10), MAX_TEMPERATURE * 10);
    // average the last readings to reduce noise
    _circularBuffer.push(potDegreesMapped / 10.0);
    return _circularBuffer.avg5();
}

