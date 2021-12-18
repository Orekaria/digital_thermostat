#include <Display.h>

Display::Display(uint32_t autoOff) {
    _autoSwitchOffTimer = CountDown(autoOff);

    _isDisplayOn = false;
    _activeDigit = 1;
    _content = "   ";
    pinMode(C_7447_A, OUTPUT);
    pinMode(C_7447_B, OUTPUT);
    pinMode(C_7447_C, OUTPUT);
    pinMode(C_7447_D, OUTPUT);
    pinMode(C_7447_E, OUTPUT);
    pinMode(C_7447_F, OUTPUT);
    pinMode(C_7447_G, OUTPUT);
    pinMode(C_7447_DP, OUTPUT);
    pinMode(C_7447_1, OUTPUT);
    pinMode(C_7447_2, OUTPUT);
    pinMode(C_7447_3, OUTPUT);
}

Display::~Display() {
}

void Display::heartbeat(bool lastHeaterState) {
#if DEBuG == true
    if (_autoSwitchOffTimer.peek()) {
        Utils::log(", display: ''");
    } else {
        Utils::log(", display: '" + getContent() + "'");
    }
#endif

    // display OFF
    if (_autoSwitchOffTimer.isFinished()) {
        switchOff(lastHeaterState);
    }
}

void Display::activateDot(uint8_t dotPosition) {
    if (_isDisplayOn) {
        _content = "E03";
        return;
    }
    digitalWrite(C_7447_1, HIGH);
    digitalWrite(C_7447_2, HIGH);
    digitalWrite(C_7447_3, HIGH);
    switch (dotPosition) {
    case 0:
        break;
    case 1:
        digitalWrite(C_7447_1, LOW);
        break;
    case 2:
        digitalWrite(C_7447_2, LOW);
        break;
    case 3:
        digitalWrite(C_7447_3, LOW);
        break;
    default:
        _content = "E04";
        break;
    }
    digitalWrite(C_7447_DP, HIGH);
}

void Display::updateDisplay() {
    if (!_isDisplayOn) {
        return;
    }

    // deactivate updates until the digit is set
    digitalWrite(C_7447_1, HIGH);
    digitalWrite(C_7447_2, HIGH);
    digitalWrite(C_7447_3, HIGH);

    char c = _content.charAt(_activeDigit - 1);
    uint16_t digitIndex = 0;
    bool isFound = false;
    for (digitIndex = 0; digitIndex < TOTAL_DIGITS; digitIndex++) {
        if (_digits[digitIndex].character == c) {
            isFound = true;
            break;
        }
    }
    if (!isFound) {
        digitIndex = 0;
    }

    // Utils::log("digit: " + String(_displayContent) + " - " + String(_activeDigit - 1) + " = " + String(digitIndex) + ", " + c + carriageReturn);

    digitalWrite(C_7447_A, _digits[digitIndex].seg0); // hor sup
    digitalWrite(C_7447_B, _digits[digitIndex].seg1); // ver sup der
    digitalWrite(C_7447_C, _digits[digitIndex].seg2); // ver inf der
    digitalWrite(C_7447_D, _digits[digitIndex].seg3); // hor inf
    digitalWrite(C_7447_E, _digits[digitIndex].seg4); // ver inf
    digitalWrite(C_7447_F, _digits[digitIndex].seg5); // ver sup izq
    digitalWrite(C_7447_G, _digits[digitIndex].seg6); // hor central '-'

    switch (_activeDigit) {
    case 1:
        digitalWrite(C_7447_1, LOW);
        break;
    case 2:
        digitalWrite(C_7447_2, LOW);
        break;
    case 3:
    case 4:
        digitalWrite(C_7447_3, LOW);
        break;
    default:
        _content = "E10";
    }

    if (_content.charAt(_activeDigit) == '.') {
        _activeDigit++;
        digitalWrite(C_7447_DP, HIGH);
    } else {
        digitalWrite(C_7447_DP, LOW);
    }

    _activeDigit++;
    if (_activeDigit > _content.length()) {
        _activeDigit = 1;
    }
}

void Display::switchOff(bool lastHeaterState) {
    digitalWrite(C_7447_1, HIGH);
    digitalWrite(C_7447_2, HIGH);
    digitalWrite(C_7447_3, HIGH);
    digitalWrite(C_7447_A, LOW);
    digitalWrite(C_7447_B, LOW);
    digitalWrite(C_7447_C, LOW);
    digitalWrite(C_7447_D, LOW);
    digitalWrite(C_7447_E, LOW);
    digitalWrite(C_7447_F, LOW);
    digitalWrite(C_7447_G, LOW);
    digitalWrite(C_7447_DP, LOW);
    _isDisplayOn = false;
    if (lastHeaterState == true) {
        activateDot(1);
    } else {
        activateDot(3);
    }
}

String Display::convertFloatToDisplayContent(float f) {
    String s = String(f, 1);

    if (s.indexOf('.') >= 00) {
        if (s.length() == 3) {
            s = " " + s;
        }
        if (s.length() != 4) {
            return "E01";
        }
    } else {
        if (s.length() != 3) {
            return "E02";
        }
    }

    return s;
}

void Display::setContent(bool isHeaterLocked, float ambientTemp, float desiredTemp, bool showAmbientOrDesired) {
    if (!_isDisplayOn) {
        return;
    }

    String newDisplayContent = "";
    if (isHeaterLocked) {
        newDisplayContent = "OFF";
    } else {
        if (showAmbientOrDesired) {
            newDisplayContent = convertFloatToDisplayContent(ambientTemp);
        } else {
            if (desiredTemp <= MIN_TEMPERATURE) {
                newDisplayContent = "OFF";
            } else if (desiredTemp >= MAX_TEMPERATURE) {
                newDisplayContent = "ON ";
            } else {
                newDisplayContent = convertFloatToDisplayContent(desiredTemp);
            }
        }
    }

    if (_content == newDisplayContent) {
        return;
    }
    _content = newDisplayContent;
    _activeDigit = 1;
};

bool Display::isDisplayOn() {
    return _isDisplayOn;
};

void Display::switchOn() {
    _autoSwitchOffTimer.reset();
    _isDisplayOn = true;
};

String Display::getContent() {
    return _content;
}