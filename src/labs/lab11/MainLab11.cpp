#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "drivers/ButtonDriver.h"
#include "drivers/LedDriver.h"
#include "drivers/SerialStream.h"
#include "services/StdioRedirect.h"
#include "labs/lab11/Lab11Config.h"
#include "labs/lab11/Lab11Sync.h"
#include "labs/lab11/ButtonLedFsm.h"
#include "labs/lab11/TaskButtonRead11.h"
#include "labs/lab11/TaskFsm11.h"
#include "labs/lab11/TaskActuator11.h"
#include "labs/lab11/TaskDisplay11.h"

// ── Shared data definitions ─────────────────────────────────────────────────
volatile LedFsmState Lab11LedState   = LedStateOff;
volatile uint32_t    Lab11PressCount = 0;

SemaphoreHandle_t xLab11Mutex    = nullptr;
SemaphoreHandle_t xLab11PressSem = nullptr;

static SerialStream SerialIo;

void SetupLab11() {
    SerialIo.begin(9600);
    initStdio(&SerialIo);

    InitializeButton(ButtonPin11);
    InitializeLed(GreenLedPin11);
    InitializeLed(RedLedPin11);
    InitializeLed(YellowLedPin11);

    SetLedState(GreenLedPin11, true);
    SetLedState(RedLedPin11, false);
    SetLedState(YellowLedPin11, false);

    xLab11Mutex    = xSemaphoreCreateMutex();
    xLab11PressSem = xSemaphoreCreateBinary();

    xTaskCreate(TaskButtonRead11Func, "Btn",   128, nullptr, 3, nullptr);
    xTaskCreate(TaskFsm11Func,        "Fsm",   128, nullptr, 3, nullptr);
    xTaskCreate(TaskActuator11Func,   "Act",   128, nullptr, 2, nullptr);
    xTaskCreate(TaskDisplay11Func,    "Disp",  192, nullptr, 1, nullptr);

    printf_P(PSTR("Lab 11: FSM ButtonLED (Part 7.2.1)\n"));
    printf_P(PSTR("Button: D%u  GreenLED (OFF): D%u  RedLED (ON): D%u  Yellow: D%u\n"),
        ButtonPin11, GreenLedPin11, RedLedPin11, YellowLedPin11);
    printf_P(PSTR("Debounce: %u consecutive samples @ %u ms\n"),
        (unsigned)DebounceSamples11, (unsigned)ButtonIntervalMs11);

    vTaskStartScheduler();
}

void LoopLab11() {
}
