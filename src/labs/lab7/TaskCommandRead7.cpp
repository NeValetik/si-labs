#include "labs/lab7/Lab7Config.h"
#include "labs/lab7/Lab7Sync.h"
#include "drivers/KeypadDriver.h"
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

void TaskCommandRead7Func(void* pvParameters) {
    const TickType_t Interval = pdMS_TO_TICKS(CommandReadIntervalMs7);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        if (IsKeypadKeyAvailable()) {
            char key = ScanKeypad();

            bool newState = false;
            bool valid = false;

            if (key == KeyOnPrimary7 || key == KeyOnAlt7) {
                newState = true;
                valid = true;
            } else if (key == KeyOffPrimary7 || key == KeyOffAlt7) {
                newState = false;
                valid = true;
            }

            if (valid) {
                xSemaphoreTake(xLab7Mutex, portMAX_DELAY);
                Lab7CommandRaw = newState;
                Lab7CommandNew = true;
                xSemaphoreGive(xLab7Mutex);

                xSemaphoreGive(xLab7CmdReady);
            }
        }

        vTaskDelayUntil(&xLastWakeTime, Interval);
    }
}
