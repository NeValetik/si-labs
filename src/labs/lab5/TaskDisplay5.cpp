#include "labs/lab5/Lab5Config.h"
#include "labs/lab5/Lab5Sync.h"
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <stdio.h>

void TaskDisplay5Func(void* pvParameters) {
    const TickType_t Interval = pdMS_TO_TICKS(DisplayIntervalMs);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        vTaskDelayUntil(&xLastWakeTime, Interval);

        // Snapshot shared data.
        xSemaphoreTake(xLab5Mutex, portMAX_DELAY);
        uint16_t raw        = Lab5RawAdc;
        int16_t  tempC10    = Lab5TempC10;
        bool     alert      = Lab5AlertActive;
        bool     rawThresh  = Lab5RawThreshold;
        uint8_t  debounce   = Lab5DebounceCounter;
        xSemaphoreGive(xLab5Mutex);

        // Format temperature: e.g. 253 -> "25.3"
        int16_t whole = tempC10 / 10;
        uint16_t frac = (uint16_t)(tempC10 < 0 ? -(tempC10 % 10) : (tempC10 % 10));

        printf_P(PSTR("RAW:%u T:%d.%uC Thr:%c Deb:%u/%u Alert:%s\n"),
            raw,
            whole, frac,
            rawThresh ? 'H' : 'L',
            debounce, DebounceMaxCount,
            alert ? "ON" : "OFF");
    }
}
