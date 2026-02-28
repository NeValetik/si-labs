#include "labs/lab3/TaskButtonMeasure.h"
#include "drivers/ButtonDriver.h"
#include "drivers/LedDriver.h"
#include "services/PressData.h"
#include <Arduino.h>

static uint8_t ButtonPin   = 0;
static uint8_t GreenLedPin = 0;
static uint8_t RedLedPin   = 0;

enum ButtonState { BtnIdle, BtnPressed };
static ButtonState State        = BtnIdle;
static uint32_t    PressStartMs = 0;

void TaskButtonMeasureInit(uint8_t buttonPin,
                           uint8_t greenLedPin,
                           uint8_t redLedPin) {
    ButtonPin   = buttonPin;
    GreenLedPin = greenLedPin;
    RedLedPin   = redLedPin;
    State       = BtnIdle;
}

// Non-blocking state machine — returns immediately every call.
void TaskButtonMeasure() {
    if (State == BtnIdle) {
        // Only read button signal when idle to detect a new press
        bool pressed = ReadButtonRaw(ButtonPin);
        if (pressed) {
            // Turn off result LEDs at the start of each new press.
            SetLedState(GreenLedPin, false);
            SetLedState(RedLedPin,   false);
            PressStartMs = millis();
            State        = BtnPressed;
            // Don't read button signal while processing - wait for release
        }
    } else {  // BtnPressed
        // Only read button signal after press is complete to detect release
        // (not while processing, only to check if button was released)
        bool pressed = ReadButtonRaw(ButtonPin);
        if (!pressed) {
            // Button was released - now process the result
            uint32_t duration = millis() - PressStartMs;
            SetPressResult(duration);

            // Signal duration type via LED.
            if (duration < ShortPressThresholdMs) {
                SetLedState(GreenLedPin, true);
            } else {
                SetLedState(RedLedPin, true);
            }
            State = BtnIdle;
        }
        // If still pressed, don't process - just wait for release on next call
    }
}
