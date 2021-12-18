#include <CircularBuffer.h>
#include <Utils.h>

CircularBuffer::CircularBuffer() {
};

CircularBuffer::~CircularBuffer() {
};

void CircularBuffer::push(float value) {
    buffer[pointer] = value;
    pointer += 1;
    if (pointer >= CIRCULAR_BUFFER_SIZE) {
        pointer = 0;
        isFullyInitialized = true;
    }
};

float CircularBuffer::avg() {
    float avg = 0.0;
    for (int i = 0; i < size(); i++) {
        avg += buffer[i] / size();
    }
    return avg;
};

/// <summary>Rounds the avg to .5</summary>
/// <returns>avg rounded to .5</returns>
float CircularBuffer::avg5() {
    return Utils::roundToDot5(this->avg());
};

int CircularBuffer::size() {
    if (!isFullyInitialized) {
        return pointer;
    }
    // return sizeof(buffer) / sizeof(buffer[0]);
    return CIRCULAR_BUFFER_SIZE;
};

float CircularBuffer::minSample() {
    float min = buffer[0];
    for (int i = 0; i < size(); i++) {
        if (buffer[i] < min) {
            min = buffer[i];
        }
    }
    return min;
};

float CircularBuffer::maxSample() {
    float max = buffer[0];
    for (int i = 0; i < size(); i++) {
        if (buffer[i] > max) {
            max = buffer[i];
        }
    }
    return max;
};

void CircularBuffer::reset() {
    buffer[0] = buffer[pointer - 1];
    isFullyInitialized = false;
    pointer = 0;
}
