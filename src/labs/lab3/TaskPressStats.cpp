#include "labs/lab3/TaskPressStats.h"
#include "drivers/LedDriver.h"
#include "services/PressData.h"
#include <Arduino.h>

static uint8_t YellowLedPin = 0;

// Blink parameters: each phase is 5 task calls × 20 ms = 100 ms.
const uint8_t BlinkOnTicks  = 5;
const uint8_t BlinkOffTicks = 5;

static uint8_t BlinkRemaining = 0;
static uint8_t PhaseTick      = 0;
static bool    LedIsOn        = false;

static void StartBlink(uint8_t count) {
    BlinkRemaining = count;
    PhaseTick      = 0;
    LedIsOn        = false;
    SetLedState(YellowLedPin, false);
}

// Advances the blink state machine by one step (called every task activation).
static void UpdateBlink() {
    if (BlinkRemaining == 0) return;

    if (PhaseTick > 0) {
        PhaseTick--;
        return;
    }

    if (!LedIsOn) {
        // Begin ON phase
        SetLedState(YellowLedPin, true);
        LedIsOn   = true;
        PhaseTick = BlinkOnTicks - 1;
    } else {
        // Begin OFF phase — one blink complete
        SetLedState(YellowLedPin, false);
        LedIsOn        = false;
        BlinkRemaining--;
        PhaseTick      = BlinkOffTicks - 1;
    }
}

void TaskPressStatsInit(uint8_t yellowLedPin) {
    YellowLedPin = yellowLedPin;
    StartBlink(0);
}

void TaskPressStats() {
    uint32_t duration;
    if (ConsumePressResult(&duration)) {
        RecordPress(duration);
        // 5 blinks for short press, 10 for long press
        StartBlink(duration < ShortPressThresholdMs ? 5 : 10);
    }
    UpdateBlink();
}
