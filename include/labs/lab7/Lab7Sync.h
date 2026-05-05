#pragma once
#ifdef USE_FREERTOS
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// ── Shared data (protected by xLab7Mutex) ───────────────────────────────────
extern volatile bool    Lab7CommandRaw;        // latest parsed command (true=ON)
extern volatile bool    Lab7CommandNew;        // flag: new command received
extern volatile bool    Lab7ConditionedState;  // debounced/validated state
extern volatile bool    Lab7ActuatorState;     // actual actuator output
extern volatile uint8_t Lab7DebounceCounter;   // current debounce counter
extern volatile bool    Lab7AlertActive;       // rapid-toggle alert

// ── Synchronisation ─────────────────────────────────────────────────────────
extern SemaphoreHandle_t xLab7Mutex;
extern SemaphoreHandle_t xLab7CmdReady;       // binary semaphore: command available

#endif
