#pragma once
#include <Arduino.h>

uint16_t ApplySaturation(uint16_t value, uint16_t minVal, uint16_t maxVal);

uint16_t ApplyMedianFilter(uint16_t* buffer, uint8_t bufferSize);

uint16_t ApplyWeightedAverage(const uint16_t* buffer, const uint16_t* weights, uint8_t size);
