#include <Arduino.h>
#include <Shared.h>
#include <Utils.h>
#include <CountDown.h>

#ifndef DISPLAY_H
#define DISPLAY_H


class Display
{
    struct digit {
        char character;
        bool seg0;
        bool seg1;
        bool seg2;
        bool seg3;
        bool seg4;
        bool seg5;
        bool seg6;
    };
#define TOTAL_DIGITS (uint16_t) 16
#define C_7447_A (uint8_t) 11
#define C_7447_B (uint8_t) 2
#define C_7447_C (uint8_t) 6
#define C_7447_D (uint8_t) 8
#define C_7447_E (uint8_t) 9
#define C_7447_F (uint8_t) 10
#define C_7447_G (uint8_t) 5
#define C_7447_DP (uint8_t) 7
#define C_7447_1 (uint8_t) 12
#define C_7447_2 (uint8_t) 4
#define C_7447_3 (uint8_t) 3

    // A -> hor sup
    // B -> ver sup der
    // C -> ver inf der
    // D -> hor inf
    // E -> ver inf izq
    // F -> ver sup izq
    // G -> hor central '-'
    const digit _digits[TOTAL_DIGITS] = {
        { 'E',HIGH,LOW ,LOW ,HIGH,HIGH,HIGH,HIGH }, // E must be the first one
        { ' ',LOW ,LOW ,LOW ,LOW ,LOW ,LOW ,LOW  },
        { '0',HIGH,HIGH,HIGH,HIGH,HIGH,HIGH,LOW  },
        { '1',LOW ,HIGH,HIGH,LOW ,LOW ,LOW ,LOW  },
        { '2',HIGH,HIGH,LOW ,HIGH,HIGH,LOW ,HIGH },
        { '3',HIGH,HIGH,HIGH,HIGH,LOW ,LOW ,HIGH },
        { '4',LOW ,HIGH,HIGH,LOW ,LOW ,HIGH,HIGH },
        { '5',HIGH,LOW ,HIGH,HIGH,LOW ,HIGH,HIGH },
        { '6',HIGH,LOW ,HIGH,HIGH,HIGH,HIGH,HIGH },
        { '7',HIGH,HIGH,HIGH,LOW ,LOW ,LOW ,LOW  },
        { '8',HIGH,HIGH,HIGH,HIGH,HIGH,HIGH,HIGH },
        { '9',HIGH,HIGH,HIGH,HIGH,LOW ,HIGH,HIGH },
        { 'O',HIGH,HIGH,HIGH,HIGH,HIGH,HIGH,LOW  },
        { 'F',HIGH,LOW ,LOW ,LOW ,HIGH,HIGH,HIGH },
        { 'N',HIGH,HIGH,HIGH,LOW ,HIGH,HIGH,LOW  },
        { '-',LOW ,LOW ,LOW ,LOW ,LOW ,LOW ,HIGH },
    };

private:
    bool _isDisplayOn;
    CountDown _autoSwitchOffTimer = CountDown(0);
    uint16_t _activeDigit;
    String _content;
    String convertFloatToDisplayContent(float f);
    void activateDot(uint8_t dotPosition);
public:
    Display(uint32_t autoOff);
    ~Display();
    void switchOn();
    void heartbeat(bool lastHeaterState);
    void setContent(bool isHeaterLocked, float ambientTemp, float desiredTemp, bool showAmbientOrDesired);
    bool isDisplayOn();
    void switchOff(bool lastHeaterState);
    void updateDisplay();
    String getContent();
};

#endif
