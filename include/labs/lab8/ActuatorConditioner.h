#pragma once
#include <Arduino.h>

uint8_t ApplySaturationU8(uint8_t value, uint8_t minVal, uint8_t maxVal);

uint8_t ApplyMedianFilterU8(uint8_t* buffer, uint8_t bufferSize);

uint8_t ApplyWeightedAverageU8(const uint8_t* buffer, const uint16_t* weights, uint8_t size);

uint8_t ApplyRamp(uint8_t current, uint8_t target, uint8_t step);
