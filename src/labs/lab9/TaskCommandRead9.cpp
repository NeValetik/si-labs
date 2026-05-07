#include "labs/lab9/Lab9Config.h"
#include "labs/lab9/Lab9Sync.h"
#include "drivers/KeypadDriver.h"
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <stdlib.h>
#include <stdio.h>

static int16_t ClampSetPoint(int16_t value) {
    if (value < SetPointMinC10) return SetPointMinC10;
    if (value > SetPointMaxC10) return SetPointMaxC10;
    return value;
}

static void NudgeSetPoint(int16_t deltaC10) {
    xSemaphoreTake(xLab9Mutex, portMAX_DELAY);
    Lab9SetPointC10 = ClampSetPoint(Lab9SetPointC10 + deltaC10);
    int16_t sp = Lab9SetPointC10;
    xSemaphoreGive(xLab9Mutex);
    printf_P(PSTR("[SP=%d.%d C]\n"), sp / 10, abs(sp) % 10);
}

static void SubmitSetPoint(int16_t newSpC10) {
    int16_t sp = ClampSetPoint(newSpC10);

    xSemaphoreTake(xLab9Mutex, portMAX_DELAY);
    Lab9SetPointC10 = sp;
    xSemaphoreGive(xLab9Mutex);

    printf_P(PSTR("[SP=%d.%d C]\n"), sp / 10, abs(sp) % 10);
}

void TaskCommandRead9Func(void* pvParameters) {
    const TickType_t Interval = pdMS_TO_TICKS(CommandIntervalMs9);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    char serialBuffer[CmdBufferSize9];
    uint8_t serialIdx = 0;

    char keypadBuffer[CmdBufferSize9];
    uint8_t keypadIdx = 0;

    for (;;) {
        // Serial: integer °C terminated by newline. e.g. "27" → 27.0 °C.
        while (Serial.available() > 0) {
            char c = (char)Serial.read();
            if (c == '\n' || c == '\r') {
                if (serialIdx > 0) {
                    serialBuffer[serialIdx] = '\0';
                    char* endPtr;
                    long value = strtol(serialBuffer, &endPtr, 10);
                    if (endPtr != serialBuffer && *endPtr == '\0') {
                        SubmitSetPoint((int16_t)(value * 10));
                    }
                    serialIdx = 0;
                }
            } else if (serialIdx < CmdBufferSize9 - 1) {
                serialBuffer[serialIdx++] = c;
            }
        }

        // Keypad: digit accumulator + nudge keys.
        while (IsKeypadKeyAvailable()) {
            char key = ScanKeypad();

            if (key >= '0' && key <= '9') {
                if (keypadIdx < CmdBufferSize9 - 1) {
                    keypadBuffer[keypadIdx++] = key;
                    putchar(key);
                }
            } else if (key == KeyClear9) {
                keypadIdx = 0;
                printf_P(PSTR(" [cleared]\n"));
            } else if (key == KeyCommit9) {
                if (keypadIdx > 0) {
                    keypadBuffer[keypadIdx] = '\0';
                    long v = strtol(keypadBuffer, nullptr, 10);
                    putchar('\n');
                    SubmitSetPoint((int16_t)(v * 10));
                    keypadIdx = 0;
                }
            } else if (key == KeyIncSmall9) {
                NudgeSetPoint(+10);
            } else if (key == KeyDecSmall9) {
                NudgeSetPoint(-10);
            } else if (key == KeyIncLarge9) {
                NudgeSetPoint(+100);
            } else if (key == KeyDecLarge9) {
                NudgeSetPoint(-100);
            }
        }

        vTaskDelayUntil(&xLastWakeTime, Interval);
    }
}
