#include "labs/lab8/Lab8Config.h"
#include "labs/lab8/Lab8Sync.h"
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <stdlib.h>

void TaskCommandRead8Func(void* pvParameters) {
    const TickType_t Interval = pdMS_TO_TICKS(CommandReadIntervalMs8);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    char cmdBuffer[CmdBufferSize8];
    uint8_t cmdIdx = 0;

    for (;;) {
        while (Serial.available() > 0) {
            char c = (char)Serial.read();

            if (c == '\n' || c == '\r') {
                if (cmdIdx > 0) {
                    cmdBuffer[cmdIdx] = '\0';

                    // Parse numeric speed value
                    char* endPtr;
                    long value = strtol(cmdBuffer, &endPtr, 10);

                    if (endPtr != cmdBuffer && *endPtr == '\0') {
                        // Valid number — clamp to uint8_t range before storing
                        if (value < 0)   value = 0;
                        if (value > 255) value = 255;

                        xSemaphoreTake(xLab8Mutex, portMAX_DELAY);
                        Lab8CommandRaw = (uint8_t)value;
                        Lab8CommandNew = true;
                        xSemaphoreGive(xLab8Mutex);

                        xSemaphoreGive(xLab8CmdReady);
                    }

                    cmdIdx = 0;
                }
            } else {
                if (cmdIdx < CmdBufferSize8 - 1) {
                    cmdBuffer[cmdIdx++] = c;
                }
            }
        }

        vTaskDelayUntil(&xLastWakeTime, Interval);
    }
}
