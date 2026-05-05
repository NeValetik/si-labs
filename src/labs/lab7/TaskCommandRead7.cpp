#include "labs/lab7/Lab7Config.h"
#include "labs/lab7/Lab7Sync.h"
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <string.h>

void TaskCommandRead7Func(void* pvParameters) {
    const TickType_t Interval = pdMS_TO_TICKS(CommandReadIntervalMs7);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    char cmdBuffer[CmdBufferSize7];
    uint8_t cmdIdx = 0;

    for (;;) {
        while (Serial.available() > 0) {
            char c = (char)Serial.read();

            if (c == '\n' || c == '\r') {
                if (cmdIdx > 0) {
                    cmdBuffer[cmdIdx] = '\0';

                    // Convert to uppercase for case-insensitive matching
                    for (uint8_t i = 0; i < cmdIdx; i++) {
                        if (cmdBuffer[i] >= 'a' && cmdBuffer[i] <= 'z') {
                            cmdBuffer[i] -= 32;
                        }
                    }

                    bool newState = false;
                    bool valid = false;

                    if (strcmp(cmdBuffer, "ON") == 0 || strcmp(cmdBuffer, "1") == 0) {
                        newState = true;
                        valid = true;
                    } else if (strcmp(cmdBuffer, "OFF") == 0 || strcmp(cmdBuffer, "0") == 0) {
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

                    cmdIdx = 0;
                }
            } else {
                if (cmdIdx < CmdBufferSize7 - 1) {
                    cmdBuffer[cmdIdx++] = c;
                }
            }
        }

        vTaskDelayUntil(&xLastWakeTime, Interval);
    }
}
