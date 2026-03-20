#include "labs/lab6/Lab6Config.h"
#include "labs/lab6/Lab6Sync.h"
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <stdio.h>

void TaskDisplay6Func(void* pvParameters) {
    const TickType_t Interval = pdMS_TO_TICKS(DisplayIntervalMs6);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        vTaskDelayUntil(&xLastWakeTime, Interval);

        xSemaphoreTake(xLab6Mutex, portMAX_DELAY);
        uint16_t raw     = Lab6RawAdc;
        uint16_t sat     = Lab6Saturated;
        uint16_t med     = Lab6MedianFiltered;
        uint16_t wavg    = Lab6WeightedAvg;
        int16_t  tempC10 = Lab6TempC10;
        bool     alert   = Lab6AlertActive;
        xSemaphoreGive(xLab6Mutex);

        int16_t whole = tempC10 / 10;
        uint16_t frac = (uint16_t)(tempC10 < 0 ? -(tempC10 % 10) : (tempC10 % 10));

        printf_P(PSTR("RAW:%u SAT:%u MED:%u AVG:%u T:%d.%uC %s\n"),
            raw, sat, med, wavg,
            whole, frac,
            alert ? "ALERT" : "OK");
    }
}
