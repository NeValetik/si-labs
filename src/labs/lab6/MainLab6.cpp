#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "drivers/LedDriver.h"
#include "drivers/NtcSensor.h"
#include "drivers/SerialStream.h"
#include "services/StdioRedirect.h"
#include "labs/lab6/Lab6Config.h"
#include "labs/lab6/Lab6Sync.h"
#include "labs/lab6/TaskSensorRead6.h"
#include "labs/lab6/TaskCondition6.h"
#include "labs/lab6/TaskDisplay6.h"

// ── Shared data definitions ─────────────────────────────────────────────────
volatile uint16_t Lab6RawAdc         = 0;
volatile uint16_t Lab6Saturated      = 0;
volatile uint16_t Lab6MedianFiltered = 0;
volatile uint16_t Lab6WeightedAvg    = 0;
volatile int16_t  Lab6TempC10        = 0;
volatile bool     Lab6AlertActive    = false;

SemaphoreHandle_t xLab6Mutex     = nullptr;
SemaphoreHandle_t xLab6NewSample = nullptr;

// ── STDIO ───────────────────────────────────────────────────────────────────
static SerialStream SerialIo;

// ── Lab entry points ────────────────────────────────────────────────────────

void SetupLab6() {
    SerialIo.begin(9600);
    initStdio(&SerialIo);

    InitializeNtcSensor(NtcAnalogPin6);
    InitializeLed(GreenLedPin6);
    InitializeLed(RedLedPin6);
    InitializeLed(YellowLedPin6);

    // LEDs off at start.
    SetLedState(GreenLedPin6, true);
    SetLedState(RedLedPin6, false);
    SetLedState(YellowLedPin6, false);

    // Create sync objects.
    xLab6Mutex     = xSemaphoreCreateMutex();
    xLab6NewSample = xSemaphoreCreateBinary();

    // Create tasks with descending priorities.
    xTaskCreate(TaskSensorRead6Func,  "Sensor",    128, nullptr, 3, nullptr);
    xTaskCreate(TaskCondition6Func,   "Condition",  256, nullptr, 2, nullptr);
    xTaskCreate(TaskDisplay6Func,     "Display",   256, nullptr, 1, nullptr);

    printf_P(PSTR("Lab 6: Analog Signal Conditioning\n"));
    printf_P(PSTR("Sensor: NTC on A0\n"));
    printf_P(PSTR("Pipeline: Sat[%u-%u] -> Median[%u] -> WAvg[%u]\n"),
        SaturationMin, SaturationMax, MedianWindowSize, WeightedAvgSize);

    vTaskStartScheduler();
}

void LoopLab6() {
    // Never reached: vTaskStartScheduler() does not return.
}
