#include "labs/lab7/Lab7Config.h"
#include "labs/lab7/Lab7Sync.h"
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

void TaskCondition7Func(void* pvParameters) {
    const TickType_t Interval = pdMS_TO_TICKS(ConditionIntervalMs7);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    bool conditionedState = false;
    bool lastRawState     = false;
    uint8_t debounceCount = 0;

    for (;;) {
        xSemaphoreTake(xLab7Mutex, portMAX_DELAY);
        bool rawCmd = Lab7CommandRaw;
        xSemaphoreGive(xLab7Mutex);

        // Temporal debouncing: require DebounceCount7 consecutive identical
        // raw values that differ from current conditioned state to confirm change
        if (rawCmd != conditionedState) {
            if (rawCmd == lastRawState) {
                debounceCount++;
            } else {
                debounceCount = 1;
            }

            if (debounceCount >= DebounceCount7) {
                conditionedState = rawCmd;
                debounceCount = 0;
            }
        } else {
            debounceCount = 0;
        }

        lastRawState = rawCmd;

        xSemaphoreTake(xLab7Mutex, portMAX_DELAY);
        Lab7ConditionedState = conditionedState;
        Lab7DebounceCounter  = debounceCount;
        xSemaphoreGive(xLab7Mutex);

        vTaskDelayUntil(&xLastWakeTime, Interval);
    }
}
