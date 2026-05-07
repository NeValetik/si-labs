#pragma once
#ifdef USE_FREERTOS
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// ── Shared data (all protected by xLab9Mutex) ───────────────────────────────
extern volatile uint16_t Lab9RawAdc;          // last raw ADC reading
extern volatile int16_t  Lab9TempC10;         // measured PV [tenths of °C]
extern volatile int16_t  Lab9SetPointC10;     // operator-configured setpoint [tenths °C]
extern volatile int16_t  Lab9HysteresisC10;   // configured hysteresis [tenths °C]
extern volatile bool     Lab9OutputState;     // true = heating, false = cooling
extern volatile uint8_t  Lab9ServoAngle;      // last commanded angle [°]

// ── Synchronisation ─────────────────────────────────────────────────────────
extern SemaphoreHandle_t xLab9Mutex;

#endif
