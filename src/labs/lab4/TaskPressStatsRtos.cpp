#include "labs/lab3/SyncObjects.h"
#include "labs/lab3/TaskPressStats.h"
#include "services/PressData.h"
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#define OFFS_STATS_MS  1
 
void TaskPressStatsRtosFunc(void* pvParameters) {
    vTaskDelay(pdMS_TO_TICKS(OFFS_STATS_MS));

    for (;;) {
        xSemaphoreTake(xPressSemaphore, portMAX_DELAY);

        uint32_t duration;
        if (ConsumePressResult(&duration)) {
            xSemaphoreTake(xStatsMutex, portMAX_DELAY);
            RecordPress(duration);
            xSemaphoreGive(xStatsMutex);
        }

        TaskPressStats();
    }
}
