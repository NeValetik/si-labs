#include "labs/lab3/SyncObjects.h"
#include "labs/lab3/TaskPressStats.h"
#include "services/PressData.h"
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#define OFFS_STATS_MS  1
 
void TaskPressStatsRtosFunc(void* pvParameters) {
    // Offset: start slightly after Task1 to preserve producer/consumer ordering.
    vTaskDelay(pdMS_TO_TICKS(OFFS_STATS_MS));

    for (;;) {
        // Block until Task1 signals a new press.
        xSemaphoreTake(xPressSemaphore, portMAX_DELAY);

        // Consume the press result and update stats under mutex protection.
        uint32_t duration;
        if (ConsumePressResult(&duration)) {
            xSemaphoreTake(xStatsMutex, portMAX_DELAY);
            RecordPress(duration);
            xSemaphoreGive(xStatsMutex);
        }

        // Drive yellow LED blink — no mutex needed (blink state is task-local).
        TaskPressStats();
    }
}
