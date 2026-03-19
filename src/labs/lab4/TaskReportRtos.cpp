#include "labs/lab3/SyncObjects.h"
#include "services/PressData.h"
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <stdio.h>

#define REC_REPORT_MS   10000
#define OFFS_REPORT_MS     50


void TaskReportRtosFunc(void* pvParameters) {
    const TickType_t RecTicks = pdMS_TO_TICKS(REC_REPORT_MS);

    vTaskDelay(pdMS_TO_TICKS(OFFS_REPORT_MS));

    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        xSemaphoreTake(xStatsMutex, portMAX_DELAY);
        PressStats Stats = GetStats();
        ResetStats();
        xSemaphoreGive(xStatsMutex);

        uint32_t TotalCount = Stats.ShortPresses + Stats.LongPresses;

        if (TotalCount > 0) {
            uint32_t AvgMs = Stats.TotalDurationMs / TotalCount;
            uint32_t WholeSeconds = AvgMs / 1000;
            uint32_t Decimals = (AvgMs % 1000) / 10;

            printf_P(PSTR("L: %lu, S: %lu, Avg: %lu.%02lus\n\r"),
                Stats.LongPresses,
                Stats.ShortPresses,
                WholeSeconds,
                Decimals);
        } else {
            printf_P(PSTR("L: %lu, S: %lu, Avg: 0.00s\n\r"),
                Stats.LongPresses,
                Stats.ShortPresses);
        }

        vTaskDelayUntil(&xLastWakeTime, RecTicks);
    }
}
