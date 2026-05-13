#include "labs/lab11/Lab11Config.h"
#include "labs/lab11/Lab11Sync.h"
#include "labs/lab11/ButtonLedFsm.h"
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <stdio.h>

void TaskDisplay11Func(void* pvParameters) {
    const TickType_t Interval = pdMS_TO_TICKS(DisplayIntervalMs11);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        vTaskDelayUntil(&xLastWakeTime, Interval);

        xSemaphoreTake(xLab11Mutex, portMAX_DELAY);
        LedFsmState state = (LedFsmState)Lab11LedState;
        uint32_t    count = Lab11PressCount;
        xSemaphoreGive(xLab11Mutex);

        // Plotter-friendly: STATE plotted as 0/1 against PRESSES.
        printf_P(PSTR("STATE:%u\tPRESSES:%lu\n"),
            (unsigned)(state == LedStateOn ? 1 : 0),
            (unsigned long)count);

        // Human-readable status line.
        printf_P(PSTR("# LED=%s  presses=%lu\n"),
            ButtonLedFsmStateName(state),
            (unsigned long)count);
    }
}
