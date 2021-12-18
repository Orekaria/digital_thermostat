#include <Arduino.h>
#include <Shared.h>
#include <Utils.h>
#include <CircularBuffer.h>

#ifndef VOLTIMETER_H
#define VOLTIMETER_H

class Voltimeter
{
private:
    uint8_t _pin;
    CircularBuffer _vinCircularBuffer;
    float readVin();
public:
    Voltimeter(uint8_t pin);
    ~Voltimeter();
    bool isVoltageSpike();
};

#endif
