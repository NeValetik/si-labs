#pragma once
#include <Arduino.h>

// Must be called once in setup before the scheduler starts.
void TaskPressStatsInit(uint8_t yellowLedPin);

// Scheduler task function — consumes press results and drives blink.
// Rec = 20 ms, Offset = 0 ms. Register after TaskButtonMeasure.
void TaskPressStats();
