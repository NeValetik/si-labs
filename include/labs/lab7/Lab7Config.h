#pragma once
#include <Arduino.h>

// ── Pin assignments ─────────────────────────────────────────────────────────
const uint8_t RelayPin7       = 9;    // binary actuator output (relay / LED)
const uint8_t GreenLedPin7    = 12;   // system OK indicator
const uint8_t RedLedPin7      = 11;   // alert indicator
const uint8_t YellowLedPin7   = 10;   // activity indicator

// ── Debounce configuration ─────────────────────────────────────────────────
const uint8_t DebounceCount7  = 5;    // consecutive identical commands to confirm

// ── Task timing ─────────────────────────────────────────────────────────────
const uint16_t CommandReadIntervalMs7    = 50;    // command polling period
const uint16_t ConditionIntervalMs7      = 50;    // conditioning period
const uint16_t ActuatorControlIntervalMs7 = 100;  // actuator drive period
const uint16_t DisplayIntervalMs7        = 500;   // display/report period

// ── Command buffer ──────────────────────────────────────────────────────────
const uint8_t CmdBufferSize7  = 8;
