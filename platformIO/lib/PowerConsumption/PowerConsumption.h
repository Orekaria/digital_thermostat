#include <Arduino.h>

#ifndef POWERCONSUMPTION_H
#define POWERCONSUMPTION_H

class PowerConsumption {
private:
    uint8_t powerMode = -1;
public:
    PowerConsumption();
    ~PowerConsumption();
    void high();
    void low();
};

#endif
