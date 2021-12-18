#include <Heater.h>

Heater::Heater(uint8_t pin) {
    _pin = pin;
    pinMode(_pin, OUTPUT);
};

Heater::~Heater() {
};

bool Heater::setOnOff(bool unlock, float ambientTemp, float desiredTemp) {
    if (unlock) {
        _isHeaterLocked = false;
        _lastHeaterState = -1;
    }

    bool onOff = LOW;
    if (!_isHeaterLocked) {
        if (desiredTemp <= MIN_TEMPERATURE) { // heater always OFF
            onOff = LOW;
        } else if (desiredTemp >= MAX_TEMPERATURE) { // heater always ON
            onOff = HIGH;
        } else if (ambientTemp <= desiredTemp) { // if the temperature drops, start the heater
            onOff = HIGH;
        }
    }

    if (_lastHeaterState == onOff) {
        return false;
    }
    _lastHeaterState = onOff;
    digitalWrite(_pin, onOff);
    return true;
}

void Heater::lock() {
    if (_isHeaterLocked) {
        return;
    }
    _isHeaterLocked = true;
    digitalWrite(_pin, LOW);
}

bool Heater::isHeaterLocked() {
    return _isHeaterLocked;
}

bool Heater::lastHeaterState() {
    return _lastHeaterState;
}
