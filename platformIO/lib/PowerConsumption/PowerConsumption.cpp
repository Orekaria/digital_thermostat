#include <PowerConsumption.h>
#include <Shared.h>

PowerConsumption::PowerConsumption() {

};

PowerConsumption::~PowerConsumption() {
};

void PowerConsumption::high() {
    if (powerMode == 1) {
        return;
    }
    powerMode = 1;
    delay(10);
    CLKPR = 0x80; // enable change in clock frequency
    CLKPR = SPEED_FULL;
#if DEBUG == true
    Serial.begin(SERIAL_SPEED * pow(2, (uint8_t)SPEED_FULL));
    Serial.println("Power mode: HIGH (" + String(SPEED_FULL, HEX) + ")");
#endif
};

void PowerConsumption::low() {
    if (powerMode == 2) {
        return;
    }
    powerMode = 2;
    delay(10);
    CLKPR = 0x80; // enable change in clock frequency
    CLKPR = SPEED_REDUCED;
#if DEBUG == true
    Serial.begin(SERIAL_SPEED * pow(2, (uint8_t)SPEED_REDUCED));
    Serial.println("Power mode: LOW (" + String(SPEED_REDUCED, HEX) + ")");
#endif
};
