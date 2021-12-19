#include <Arduino.h>
#include <Shared.h>
#include <Utils.h>
#include <CircularBuffer.h>

#ifndef THERMOSTAT_H
#define THERMOSTAT_H

class Thermostat
{
private:
    uint8_t _pin;
    CircularBuffer _circularBuffer;
public:
    Thermostat(uint8_t pin);
    ~Thermostat();
    float readLM35();
};

#endif

Thermostat::Thermostat(uint8_t pin) {
    _pin = pin;
    _circularBuffer = CircularBuffer();
    pinMode(_pin, INPUT);
}

Thermostat::~Thermostat() {
}

float Thermostat::readLM35() {
    float adcValue = analogRead(_pin); // accuracy: 0.5 celsius steps
    float voltage = adcValue * AREF / 1024.0;
    float ambientTempFix = Utils::roundToDot5(voltage * 100.0);
    // Utils::log(", adcValue : " + String(adcValue ));
    // Utils::log(", voltage: " + String(voltage, 4));
    // Utils::log(", tempRaw: " + String(ambientTempFix, 2));

    // return the avg
    _circularBuffer.push(ambientTempFix);
    return _circularBuffer.avg5();
}
