#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "drivers/LedDriver.h"
#include "drivers/SerialStream.h"
#include "services/StdioRedirect.h"
#include "labs/lab7/Lab7Config.h"
#include "labs/lab7/Lab7Sync.h"
#include "labs/lab7/TaskCommandRead7.h"
#include "labs/lab7/TaskCondition7.h"
#include "labs/lab7/TaskActuatorControl7.h"
#include "labs/lab7/TaskDisplay7.h"

// ── Shared data definitions ─────────────────────────────────────────────────
volatile bool    Lab7CommandRaw       = false;
volatile bool    Lab7CommandNew       = false;
volatile bool    Lab7ConditionedState = false;
volatile bool    Lab7ActuatorState    = false;
volatile uint8_t Lab7DebounceCounter  = 0;
volatile bool    Lab7AlertActive      = false;

SemaphoreHandle_t xLab7Mutex    = nullptr;
SemaphoreHandle_t xLab7CmdReady = nullptr;

static SerialStream SerialIo;

void SetupLab7() {
    SerialIo.begin(9600);
    initStdio(&SerialIo);

    InitializeLed(RelayPin7);
    InitializeLed(GreenLedPin7);
    InitializeLed(RedLedPin7);
    InitializeLed(YellowLedPin7);

    SetLedState(RelayPin7, false);
    SetLedState(GreenLedPin7, true);
    SetLedState(RedLedPin7, false);
    SetLedState(YellowLedPin7, false);

    xLab7Mutex    = xSemaphoreCreateMutex();
    xLab7CmdReady = xSemaphoreCreateBinary();

    xTaskCreate(TaskCommandRead7Func,     "CmdRead",  256, nullptr, 3, nullptr);
    xTaskCreate(TaskCondition7Func,       "Cond",     128, nullptr, 2, nullptr);
    xTaskCreate(TaskActuatorControl7Func, "Actuator", 128, nullptr, 2, nullptr);
    xTaskCreate(TaskDisplay7Func,         "Display",  256, nullptr, 1, nullptr);

    printf_P(PSTR("Lab 7: Binary Actuator Control\n"));
    printf_P(PSTR("Actuator: Relay on D%u\n"), RelayPin7);
    printf_P(PSTR("Commands: ON/1 OFF/0\n"));
    printf_P(PSTR("Debounce: %u samples\n"), DebounceCount7);

    vTaskStartScheduler();
}

void LoopLab7() {
}
