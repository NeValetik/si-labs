#include "labs/lab6/SignalConditioner.h"

uint16_t ApplySaturation(uint16_t value, uint16_t minVal, uint16_t maxVal) {
    if (value < minVal) return minVal;
    if (value > maxVal) return maxVal;
    return value;
}

uint16_t ApplyMedianFilter(uint16_t* buffer, uint8_t bufferSize) {
    uint16_t sorted[bufferSize];
    for (uint8_t i = 0; i < bufferSize; i++) {
        sorted[i] = buffer[i];
    }

    for (uint8_t i = 1; i < bufferSize; i++) {
        uint16_t key = sorted[i];
        int8_t j = i - 1;
        while (j >= 0 && sorted[j] > key) {
            sorted[j + 1] = sorted[j];
            j--;
        }
        sorted[j + 1] = key;
    }

    return sorted[bufferSize / 2];
}

uint16_t ApplyWeightedAverage(const uint16_t* buffer, const uint16_t* weights, uint8_t size) {
    uint32_t weightedSum = 0;
    uint32_t weightTotal = 0;

    for (uint8_t i = 0; i < size; i++) {
        weightedSum += (uint32_t)buffer[i] * (uint32_t)weights[i];
        weightTotal += weights[i];
    }

    if (weightTotal == 0) return 0;
    return (uint16_t)(weightedSum / weightTotal);
}
