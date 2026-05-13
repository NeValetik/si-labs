#pragma once
#include <Arduino.h>

// ── Pin assignments ─────────────────────────────────────────────────────────
const uint8_t ButtonPin11     = 2;    // INPUT_PULLUP, active-LOW
const uint8_t GreenLedPin11   = 12;   // FSM state LED_OFF
const uint8_t RedLedPin11     = 11;   // FSM state LED_ON
const uint8_t YellowLedPin11  = 10;   // activity blink

// ── Debouncing ──────────────────────────────────────────────────────────────
// Five consecutive identical raw samples are required to confirm a stable
// logic level — at the 50 ms sampling cadence below this gives a 250 ms
// validation window, comfortably under the 100 ms latency target for
// confirming an intentional press and rejecting contact bounce.
const uint8_t  DebounceSamples11 = 5;

// ── Task timing ─────────────────────────────────────────────────────────────
const uint16_t ButtonIntervalMs11   = 50;
const uint16_t ActuatorIntervalMs11 = 50;
const uint16_t DisplayIntervalMs11  = 500;
