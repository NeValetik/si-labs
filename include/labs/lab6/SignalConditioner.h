#pragma once
#include <Arduino.h>

// Clamp value to [minVal, maxVal].
uint16_t ApplySaturation(uint16_t value, uint16_t minVal, uint16_t maxVal);

// Median filter: sorts the provided buffer and returns the median.
// bufferSize must be odd.
uint16_t ApplyMedianFilter(uint16_t* buffer, uint8_t bufferSize);

// Weighted average: applies weights to buffer values.
// buffer[0] is the most recent sample.
// weights[0] is the weight for the most recent sample.
uint16_t ApplyWeightedAverage(const uint16_t* buffer, const uint16_t* weights, uint8_t size);
