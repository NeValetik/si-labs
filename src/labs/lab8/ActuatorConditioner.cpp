#include "labs/lab8/ActuatorConditioner.h"

uint8_t ApplySaturationU8(uint8_t value, uint8_t minVal, uint8_t maxVal) {
    if (value < minVal) return minVal;
    if (value > maxVal) return maxVal;
    return value;
}

uint8_t ApplyMedianFilterU8(uint8_t* buffer, uint8_t bufferSize) {
    uint8_t sorted[bufferSize];
    for (uint8_t i = 0; i < bufferSize; i++) {
        sorted[i] = buffer[i];
    }

    // Insertion sort
    for (uint8_t i = 1; i < bufferSize; i++) {
        uint8_t key = sorted[i];
        int8_t j = i - 1;
        while (j >= 0 && sorted[j] > key) {
            sorted[j + 1] = sorted[j];
            j--;
        }
        sorted[j + 1] = key;
    }

    return sorted[bufferSize / 2];
}

uint8_t ApplyWeightedAverageU8(const uint8_t* buffer, const uint16_t* weights, uint8_t size) {
    uint32_t weightedSum = 0;
    uint32_t weightTotal = 0;

    for (uint8_t i = 0; i < size; i++) {
        weightedSum += (uint32_t)buffer[i] * (uint32_t)weights[i];
        weightTotal += weights[i];
    }

    if (weightTotal == 0) return 0;
    return (uint8_t)(weightedSum / weightTotal);
}

uint8_t ApplyRamp(uint8_t current, uint8_t target, uint8_t step) {
    if (current < target) {
        uint8_t diff = target - current;
        return current + (diff < step ? diff : step);
    } else if (current > target) {
        uint8_t diff = current - target;
        return current - (diff < step ? diff : step);
    }
    return current;
}
