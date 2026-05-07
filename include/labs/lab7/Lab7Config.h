#pragma once
#include <Arduino.h>

// ── Pin assignments ─────────────────────────────────────────────────────────
const uint8_t RelayPin7       = 9;    // binary actuator output (relay / LED)
const uint8_t GreenLedPin7    = 12;   // system OK indicator
const uint8_t RedLedPin7      = 11;   // alert indicator
const uint8_t YellowLedPin7   = 10;   // activity indicator

// Keypad 4×4 — rows driven LOW one at a time, columns INPUT_PULLUP
const uint8_t KeypadRow0Pin7  = 2;
const uint8_t KeypadRow1Pin7  = 3;
const uint8_t KeypadRow2Pin7  = 4;
const uint8_t KeypadRow3Pin7  = 5;
const uint8_t KeypadCol0Pin7  = A0;
const uint8_t KeypadCol1Pin7  = A1;
const uint8_t KeypadCol2Pin7  = A2;
const uint8_t KeypadCol3Pin7  = A3;

// Keypad command mapping: 1/A → ON, 0/B → OFF
const char KeyOnPrimary7      = '1';
const char KeyOnAlt7          = 'A';
const char KeyOffPrimary7     = '0';
const char KeyOffAlt7         = 'B';

// ── Debounce configuration ─────────────────────────────────────────────────
const uint8_t DebounceCount7  = 5;    // consecutive identical commands to confirm

// ── Task timing ─────────────────────────────────────────────────────────────
const uint16_t CommandReadIntervalMs7    = 50;    // command polling period
const uint16_t ConditionIntervalMs7      = 50;    // conditioning period
const uint16_t ActuatorControlIntervalMs7 = 100;  // actuator drive period
const uint16_t DisplayIntervalMs7        = 500;   // display/report period
