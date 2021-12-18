#include <Voltimeter.h>

Voltimeter::Voltimeter(uint8_t pin) {
    _pin = pin;
    _vinCircularBuffer = CircularBuffer();
    pinMode(_pin, OUTPUT);
}

Voltimeter::~Voltimeter() {
}

float Voltimeter::readVin() {
    int pinRead = analogRead(_pin); // Probe Input
    float vin = pinRead * 5.0 / 1024.0; // Pin to Vin
    return vin;
}

bool Voltimeter::isVoltageSpike() {
    // voltage
    float vin = readVin();
    _vinCircularBuffer.push(vin);
    float vinDiff = _vinCircularBuffer.maxSample() - _vinCircularBuffer.minSample();
#if DEBUG == true
    Utils::log("Vin: " + String(vin));
    Utils::log(" (" + String(_vinCircularBuffer.size()) + "/7");
    Utils::log(" " + String(_vinCircularBuffer.minSample(), 2));
    Utils::log(" - " + String(_vinCircularBuffer.maxSample(), 2));
    Utils::log(" = " + String(_vinCircularBuffer.maxSample() - _vinCircularBuffer.minSample(), 2));
    Utils::log(") ");
#endif
    if (vinDiff > 0.22) {
        // seen increases of 0.28V e.g: 3.96 (3.95 - 4.22 - 0.28) = there is an outside source power (engine, 220V) => set heater to OFF / retract the step
        // external source of power detected. The caravan is either moving of has been connected to 220V.
        // set the heater to OFF regardless of the POT, until the panel is touched
        return true;
#if DEBUG == true
        Utils::log("VIN buffer RESET: " + String(vinDiff) + " ");
#endif
        _vinCircularBuffer.reset();
    }
    return false;
}
