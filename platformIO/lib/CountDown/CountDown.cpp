#include <CountDown.h>

CountDown::CountDown(uint32_t maxValue) {
    _maxCount = maxValue;
    CountDown(maxValue, false);
};

CountDown::CountDown(uint32_t maxValue, bool isLoop) {
    _maxCount = maxValue;
    _isLoop = isLoop;
};

CountDown::~CountDown() {
};

/// <summary>Decreases count by 1. If the countdown is finished, the count resets to the maxValue</summary>
/// <returns>true if the countdown has finished</returns>
bool CountDown::isFinished() {
    if (_count > 0) {
        _count--;
    } else if (_count == 0) {
        if (_isLoop) {
            _count = _maxCount;
        }
        return true;
    }
    return false;
};

bool CountDown::peek() {
    return _count == 0;
};

void CountDown::reset() {
    _count = _maxCount;
};
