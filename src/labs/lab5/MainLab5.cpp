#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "drivers/LedDriver.h"
#include "drivers/NtcSensor.h"
#include "drivers/SerialStream.h"
#include "services/StdioRedirect.h"
#include "labs/lab5/Lab5Config.h"
#include "labs/lab5/Lab5Sync.h"
#include "labs/lab5/TaskSensorRead5.h"
#include "labs/lab5/TaskThresholdAlert5.h"
#include "labs/lab5/TaskDisplay5.h"

// ── Shared data definitions ─────────────────────────────────────────────────
volatile uint16_t Lab5RawAdc          = 0;
volatile int16_t  Lab5TempC10         = 0;
volatile bool     Lab5AlertActive     = false;
volatile bool     Lab5RawThreshold    = false;
volatile uint8_t  Lab5DebounceCounter = 0;

SemaphoreHandle_t xLab5Mutex = nullptr;

// ── STDIO ───────────────────────────────────────────────────────────────────
static SerialStream SerialIo;

// ── Lab entry points ────────────────────────────────────────────────────────

void SetupLab5() {
    SerialIo.begin(9600);
    initStdio(&SerialIo);

    InitializeNtcSensor(NtcAnalogPin);
    InitializeLed(GreenLedPin5);
    InitializeLed(RedLedPin5);

    // LEDs off at start.
    SetLedState(GreenLedPin5, true);
    SetLedState(RedLedPin5, false);

    // Create mutex for shared data.
    xLab5Mutex = xSemaphoreCreateMutex();

    // Create tasks with descending priorities.
    xTaskCreate(TaskSensorRead5Func,      "Sensor",    128, nullptr, 3, nullptr);
    xTaskCreate(TaskThresholdAlert5Func,   "Threshold", 128, nullptr, 2, nullptr);
    xTaskCreate(TaskDisplay5Func,          "Display",   256, nullptr, 1, nullptr);

    printf_P(PSTR("Lab 5: Binary Signal - Threshold Alerting\n"));
    printf_P(PSTR("Sensor: NTC on A0\n"));
    printf_P(PSTR("Alert: >%d C (hyst: %d C)\n"), (int)ThresholdHighC, (int)ThresholdLowC);
    printf_P(PSTR("Green=normal, Red=alert\n"));

    vTaskStartScheduler();
}

void LoopLab5() {
    // Never reached: vTaskStartScheduler() does not return.
}
