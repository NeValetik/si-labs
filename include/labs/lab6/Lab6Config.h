#pragma once
#include <Arduino.h>

// ── Pin assignments ─────────────────────────────────────────────────────────
const uint8_t NtcAnalogPin6  = A0;
const uint8_t GreenLedPin6   = 12;   // normal state
const uint8_t RedLedPin6     = 11;   // alert (temperature high)
const uint8_t YellowLedPin6  = 10;   // conditioning active indicator

// ── Saturation limits (ADC range clamp) ─────────────────────────────────────
const uint16_t SaturationMin = 100;
const uint16_t SaturationMax = 900;

// ── Median filter window size ───────────────────────────────────────────────
const uint8_t MedianWindowSize = 5;

// ── Weighted average filter ─────────────────────────────────────────────────
const uint8_t  WeightedAvgSize = 4;
const uint16_t WeightedAvgWeights[WeightedAvgSize] = {50, 25, 15, 10};

// ── Alert threshold (on conditioned temperature) ────────────────────────────
const float AlertThresholdC = 30.0f;

// ── Task timing ─────────────────────────────────────────────────────────────
const uint16_t SensorReadIntervalMs6  = 50;    // sensor acquisition period
const uint16_t ConditionIntervalMs6   = 50;    // conditioning period
const uint16_t DisplayIntervalMs6     = 500;   // display/report period
