#pragma once
#ifdef USE_FREERTOS
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// ── Shared data (protected by xLab6Mutex) ───────────────────────────────────
extern volatile uint16_t Lab6RawAdc;           // raw ADC reading
extern volatile uint16_t Lab6Saturated;        // after saturation
extern volatile uint16_t Lab6MedianFiltered;   // after median filter
extern volatile uint16_t Lab6WeightedAvg;      // after weighted average
extern volatile int16_t  Lab6TempC10;          // final temperature (tenths of °C)
extern volatile bool     Lab6AlertActive;      // temperature alert state

// ── Synchronisation ─────────────────────────────────────────────────────────
extern SemaphoreHandle_t xLab6Mutex;
extern SemaphoreHandle_t xLab6NewSample;       // binary semaphore: sensor -> conditioner

#endif
