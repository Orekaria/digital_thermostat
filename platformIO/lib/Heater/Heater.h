#include <Arduino.h>
#include <Shared.h>
#include <Utils.h>

#ifndef HEATER_H
#define HEATER_H

class Heater {
private:
    uint8_t _pin = 13;
    bool _isHeaterLocked = false;
    int8_t _lastHeaterState = -1;
public:
    Heater(uint8_t pin);
    ~Heater();
    bool isHeaterLocked();
    bool lastHeaterState();
    bool setOnOff(bool unlock, float ambientTemp, float desiredTemp);
    void lock();
};

#endif
