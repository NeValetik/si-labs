#include "labs/lab3/SyncObjects.h"
#include "labs/lab3/TaskButtonMeasure.h"
#include "services/PressData.h"
#include <Arduino_FreeRTOS.h>
#include <task.h>

#define REC_MEASURE_MS   20
#define OFFS_MEASURE_MS   0

void TaskButtonMeasureRtosFunc(void* pvParameters) {
    const TickType_t RecTicks = pdMS_TO_TICKS(REC_MEASURE_MS);

    // Apply initial offset (yield at least one tick even if offset is 0).
    vTaskDelay(pdMS_TO_TICKS(OFFS_MEASURE_MS > 0 ? OFFS_MEASURE_MS : 1));

    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        bool hadPending = HasPressResult();

        TaskButtonMeasure();

        // If a new press result appeared, wake Task2 via semaphore.
        if (!hadPending && HasPressResult()) {
            xSemaphoreGive(xPressSemaphore);
        }

        vTaskDelayUntil(&xLastWakeTime, RecTicks);
    }
}
