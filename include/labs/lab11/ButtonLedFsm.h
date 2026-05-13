#pragma once
#include <Arduino.h>

// Finite state machine for the ButtonLED application (Lab 11, Part 7.2.1).
//
//   ┌──────────┐   EventButtonPress   ┌──────────┐
//   │ LedOff   │ ───────────────────▶ │  LedOn   │
//   │  (init)  │ ◀─────────────────── │          │
//   └──────────┘   EventButtonPress   └──────────┘
//
// The FSM is pure logic (no I/O, no driver calls) so it can be unit-tested or
// reused with a different actuator. Drivers are invoked by TaskActuator11.
enum LedFsmState : uint8_t {
    LedStateOff = 0,
    LedStateOn  = 1
};

enum LedFsmEvent : uint8_t {
    EventButtonPress = 0
};

// Pure transition function. Returns the next state for (current, event).
LedFsmState ButtonLedFsmStep(LedFsmState current, LedFsmEvent event);

// Human-readable label for diagnostics / display tasks.
const char* ButtonLedFsmStateName(LedFsmState state);
