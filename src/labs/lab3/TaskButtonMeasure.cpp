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
    bool pressed = ReadButtonRaw(ButtonPin);

    if (State == BtnIdle) {
        if (pressed) {
            // Turn off result LEDs at the start of each new press.
            SetLedState(GreenLedPin, false);
            SetLedState(RedLedPin,   false);
            PressStartMs = millis();
            State        = BtnPressed;
        }
    } else {  // BtnPressed
        if (!pressed) {
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
    }
}
