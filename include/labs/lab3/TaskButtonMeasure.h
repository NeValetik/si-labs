#pragma once
#include <Arduino.h>

// Must be called once in setup before the scheduler starts.
void TaskButtonMeasureInit(uint8_t buttonPin,
                           uint8_t greenLedPin,
                           uint8_t redLedPin);

// Scheduler task function — non-blocking state machine.
// Rec = 20 ms, Offset = 0 ms. Register before TaskPressStats.
void TaskButtonMeasure();
