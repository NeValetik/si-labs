#pragma once
#ifdef USE_FREERTOS
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// ── Shared state (protected by xLab10Mutex) ─────────────────────────────────
extern volatile uint16_t Lab10RawAdc;
extern volatile int16_t  Lab10TempC10;        // PV
extern volatile int16_t  Lab10SetPointC10;    // SP

extern volatile int32_t  Lab10KpQ88;          // PID gains, Q8.8
extern volatile int32_t  Lab10KiQ88;
extern volatile int32_t  Lab10KdQ88;

extern volatile int32_t  Lab10Integral;       // PID internal state
extern volatile int16_t  Lab10PrevError;
extern volatile int16_t  Lab10Output;         // clamped [-OutputMax, +OutputMax]
extern volatile uint8_t  Lab10ServoAngle;     // last commanded angle

extern SemaphoreHandle_t xLab10Mutex;

#endif
