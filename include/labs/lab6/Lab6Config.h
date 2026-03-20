#pragma once
#include <Arduino.h>

const uint8_t NtcAnalogPin6  = A0;
const uint8_t GreenLedPin6   = 12;   
const uint8_t RedLedPin6     = 11;   
const uint8_t YellowLedPin6  = 10;   

const uint16_t SaturationMin = 150;
const uint16_t SaturationMax = 900;

const uint8_t MedianWindowSize = 5;

const uint8_t  WeightedAvgSize = 4;
const uint16_t WeightedAvgWeights[WeightedAvgSize] = {50, 25, 15, 10};

const float AlertThresholdC = 30.0f;

const uint16_t SensorReadIntervalMs6  = 50;    
const uint16_t ConditionIntervalMs6   = 50;    
const uint16_t DisplayIntervalMs6     = 500; 
