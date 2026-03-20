#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "drivers/ButtonDriver.h"
#include "drivers/LedDriver.h"
#include "drivers/SerialStream.h"
#include "services/StdioRedirect.h"
#include "services/PressData.h"
#include "labs/lab3/SyncObjects.h"
#include "labs/lab3/TaskButtonMeasure.h"
#include "labs/lab3/TaskPressStats.h"

const uint8_t ButtonPin    =  2;
const uint8_t GreenLedPin  = 12;
const uint8_t RedLedPin    = 11;
const uint8_t YellowLedPin = 10;

SemaphoreHandle_t xPressSemaphore = nullptr;
SemaphoreHandle_t xStatsMutex     = nullptr;

static SerialStream SerialIo;

void TaskButtonMeasureRtosFunc(void* pvParameters);
void TaskPressStatsRtosFunc(void* pvParameters);
void TaskReportRtosFunc(void* pvParameters);

void SetupLab4() {
    SerialIo.begin(9600);
    initStdio(&SerialIo);

    InitializeButton(ButtonPin);
    InitializeLed(GreenLedPin);
    InitializeLed(RedLedPin);
    InitializeLed(YellowLedPin);

    TaskButtonMeasureInit(ButtonPin, GreenLedPin, RedLedPin);
    TaskPressStatsInit(YellowLedPin);

    xPressSemaphore = xSemaphoreCreateBinary();
    xStatsMutex     = xSemaphoreCreateMutex();

    xTaskCreate(TaskButtonMeasureRtosFunc, "Measure", 128, nullptr, 1, nullptr);
    xTaskCreate(TaskPressStatsRtosFunc,    "Stats",   128, nullptr, 1, nullptr);
    xTaskCreate(TaskReportRtosFunc,        "Report",  200, nullptr, 1, nullptr);

    printf("Lab 3 RTOS: Button Monitor\n");
    printf("Short < 500 ms -> green\n");
    printf("Long >= 500 ms -> red\n");
    printf("Report every 10 s\n");

    vTaskStartScheduler();
}

void LoopLab4() {
}
