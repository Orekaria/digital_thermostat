#include <Arduino.h>

#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

class CircularBuffer {
private:
    static const uint8_t CIRCULAR_BUFFER_SIZE = 7; // beats of the buffer
    float buffer[CIRCULAR_BUFFER_SIZE];
    int pointer = 0;
    bool isFullyInitialized = false;
    float roundToDot5(float f);
public:
    CircularBuffer();
    ~CircularBuffer();
    void push(float value);
    float avg();
    float avg5();
    int size();
    float minSample();
    float maxSample();
    void reset();
};
#endif /* CIRCULARBUFFER_H */
