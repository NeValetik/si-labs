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
        // Atomically snapshot and reset stats.
        xSemaphoreTake(xStatsMutex, portMAX_DELAY);
        PressStats Stats = GetStats();
        ResetStats();
        xSemaphoreGive(xStatsMutex);

        uint32_t Avg = (Stats.TotalPresses > 0)
                       ? (Stats.TotalDurationMs / Stats.TotalPresses)
                       : 0;

        printf("\n--- Report (10s) ---\n");
        printf("Total : %lu\n", Stats.TotalPresses);
        printf("Short : %lu\n", Stats.ShortPresses);
        printf("Long  : %lu\n", Stats.LongPresses);
        printf("Avg   : %lu ms\n", Avg);
        printf("--------------------\n");

        vTaskDelayUntil(&xLastWakeTime, RecTicks);
    }
}
