#include <Arduino.h>

#ifndef COUNTDOWN_H
#define COUNTDOWN_H

class CountDown
{
private:
    uint32_t _maxCount;
    bool _isLoop;
    uint32_t _count = 0;
public:
    CountDown(uint32_t maxValue);
    CountDown(uint32_t maxValue, bool isLoop);
    ~CountDown();
    bool isFinished();
    bool peek();
    void reset();
};

#endif
