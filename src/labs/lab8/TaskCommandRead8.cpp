#include "labs/lab8/Lab8Config.h"
#include "labs/lab8/Lab8Sync.h"
#include "drivers/KeypadDriver.h"
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <stdlib.h>

static void SubmitSpeed(uint8_t value) {
    if (value > SpeedMax8) value = SpeedMax8;

    xSemaphoreTake(xLab8Mutex, portMAX_DELAY);
    Lab8CommandRaw = value;
    Lab8CommandNew = true;
    xSemaphoreGive(xLab8Mutex);

    xSemaphoreGive(xLab8CmdReady);
}

void TaskCommandRead8Func(void* pvParameters) {
    const TickType_t Interval = pdMS_TO_TICKS(CommandReadIntervalMs8);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    char serialBuffer[CmdBufferSize8];
    uint8_t serialIdx = 0;

    char keypadBuffer[CmdBufferSize8];
    uint8_t keypadIdx = 0;

    for (;;) {
        // ── Serial input ────────────────────────────────────────────────────
        while (Serial.available() > 0) {
            char c = (char)Serial.read();

            if (c == '\n' || c == '\r') {
                if (serialIdx > 0) {
                    serialBuffer[serialIdx] = '\0';

                    char* endPtr;
                    long value = strtol(serialBuffer, &endPtr, 10);

                    if (endPtr != serialBuffer && *endPtr == '\0' && value >= 0) {
                        SubmitSpeed((uint8_t)(value > 255 ? 255 : value));
                    }
                    serialIdx = 0;
                }
            } else if (serialIdx < CmdBufferSize8 - 1) {
                serialBuffer[serialIdx++] = c;
            }
        }

        // ── Keypad input ────────────────────────────────────────────────────
        while (IsKeypadKeyAvailable()) {
            char key = ScanKeypad();

            if (key >= '0' && key <= '9') {
                if (keypadIdx < CmdBufferSize8 - 1) {
                    keypadBuffer[keypadIdx++] = key;
                    putchar(key);   // echo
                }
            } else if (key == KeyClear8) {
                keypadIdx = 0;
                printf_P(PSTR(" [cleared]\n"));
            } else if (key == KeyEmergency8) {
                keypadIdx = 0;
                printf_P(PSTR(" [STOP]\n"));
                SubmitSpeed(0);
            } else if (key == KeyCommit8) {
                if (keypadIdx > 0) {
                    keypadBuffer[keypadIdx] = '\0';
                    long value = strtol(keypadBuffer, nullptr, 10);
                    putchar('\n');
                    SubmitSpeed((uint8_t)(value > 255 ? 255 : value));
                    keypadIdx = 0;
                }
            }
        }

        vTaskDelayUntil(&xLastWakeTime, Interval);
    }
}
