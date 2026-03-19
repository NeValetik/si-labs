#pragma once
#ifdef USE_FREERTOS
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// ── Shared data (protected by xLab5Mutex) ───────────────────────────────────
extern volatile uint16_t Lab5RawAdc;          // raw ADC reading
extern volatile int16_t  Lab5TempC10;         // temperature in tenths of °C
extern volatile bool     Lab5AlertActive;     // debounced alert state
extern volatile bool     Lab5RawThreshold;    // raw threshold comparison (before debounce)
extern volatile uint8_t  Lab5DebounceCounter; // current debounce counter

// ── Synchronisation ─────────────────────────────────────────────────────────
extern SemaphoreHandle_t xLab5Mutex;

#endif
