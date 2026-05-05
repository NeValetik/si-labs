#pragma once
#ifdef USE_FREERTOS
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// ── Shared data (protected by xLab8Mutex) ───────────────────────────────────
extern volatile uint8_t Lab8CommandRaw;        // raw speed command [0-100%]
extern volatile bool    Lab8CommandNew;        // flag: new command received
extern volatile uint8_t Lab8Saturated;         // after saturation
extern volatile uint8_t Lab8MedianFiltered;    // after median filter
extern volatile uint8_t Lab8WeightedAvg;       // after weighted average
extern volatile uint8_t Lab8RampTarget;        // conditioning output → ramp target
extern volatile uint8_t Lab8CurrentSpeed;      // actual speed after ramp [0-100%]
extern volatile uint8_t Lab8PwmValue;          // PWM duty [0-255]
extern volatile bool    Lab8AlertActive;       // limit / overload alert

// ── Synchronisation ─────────────────────────────────────────────────────────
extern SemaphoreHandle_t xLab8Mutex;
extern SemaphoreHandle_t xLab8CmdReady;        // binary semaphore: new command

#endif
