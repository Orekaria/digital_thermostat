#include <Arduino.h>

#ifndef POWERCONSUMPTION_H
#define POWERCONSUMPTION_H

class PowerConsumption {
    enum class CPUMode {
        high,
        low
    };

private:
    CPUMode powerMode = CPUMode::high;
public:
    PowerConsumption();
    ~PowerConsumption();
    void high();
    void low();
};

#endif
