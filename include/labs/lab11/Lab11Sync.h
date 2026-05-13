#pragma once
#ifdef USE_FREERTOS
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include "labs/lab11/ButtonLedFsm.h"

// ── Shared data (protected by xLab11Mutex) ──────────────────────────────────
extern volatile LedFsmState Lab11LedState;     // current FSM state
extern volatile uint32_t    Lab11PressCount;   // confirmed presses since boot

// ── Synchronisation ─────────────────────────────────────────────────────────
extern SemaphoreHandle_t xLab11Mutex;       // protects Lab11LedState/PressCount
extern SemaphoreHandle_t xLab11PressSem;    // binary: button task gives, FSM takes
#endif
