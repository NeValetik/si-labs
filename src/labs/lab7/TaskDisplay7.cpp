#include "labs/lab7/Lab7Config.h"
#include "labs/lab7/Lab7Sync.h"
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <stdio.h>

void TaskDisplay7Func(void* pvParameters) {
    const TickType_t Interval = pdMS_TO_TICKS(DisplayIntervalMs7);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        vTaskDelayUntil(&xLastWakeTime, Interval);

        xSemaphoreTake(xLab7Mutex, portMAX_DELAY);
        bool rawCmd      = Lab7CommandRaw;
        bool conditioned = Lab7ConditionedState;
        bool actuator    = Lab7ActuatorState;
        uint8_t debounce = Lab7DebounceCounter;
        bool alert       = Lab7AlertActive;
        xSemaphoreGive(xLab7Mutex);

        printf_P(PSTR("CMD:%s COND:%s DEB:%u/%u ACT:%s %s\n"),
            rawCmd      ? "ON"  : "OFF",
            conditioned ? "ON"  : "OFF",
            debounce, DebounceCount7,
            actuator    ? "ON"  : "OFF",
            alert       ? "ALERT" : "OK");
    }
}
