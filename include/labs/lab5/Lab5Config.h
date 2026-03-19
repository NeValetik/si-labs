#pragma once
#include <Arduino.h>

// ── Pin assignments ─────────────────────────────────────────────────────────
const uint8_t NtcAnalogPin  = A0;
const uint8_t GreenLedPin5  = 12;   // normal state
const uint8_t RedLedPin5    = 11;   // alert state

// ── Threshold alerting configuration ────────────────────────────────────────
// Hysteresis: alert activates above ThresholdHighC, deactivates below ThresholdLowC.
const float ThresholdHighC  = 30.0f;   // upper threshold [°C]
const float ThresholdLowC   = 28.0f;   // lower threshold [°C]

// Debounce: number of consecutive samples needed to confirm a state change.
const uint8_t DebounceMaxCount = 5;

// ── Task timing ─────────────────────────────────────────────────────────────
const uint16_t SensorReadIntervalMs   = 50;    // sensor acquisition period
const uint16_t ThresholdCheckMs       = 50;    // threshold check period
const uint16_t DisplayIntervalMs      = 500;   // display/report period
