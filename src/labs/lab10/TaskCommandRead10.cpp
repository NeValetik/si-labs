#include "labs/lab10/Lab10Config.h"
#include "labs/lab10/Lab10Sync.h"
#include "drivers/KeypadDriver.h"
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

static int16_t ClampSetPoint10(int16_t value) {
    if (value < SetPointMinC10_10) return SetPointMinC10_10;
    if (value > SetPointMaxC10_10) return SetPointMaxC10_10;
    return value;
}

static void NudgeSetPoint10(int16_t deltaC10) {
    xSemaphoreTake(xLab10Mutex, portMAX_DELAY);
    Lab10SetPointC10 = ClampSetPoint10(Lab10SetPointC10 + deltaC10);
    int16_t sp = Lab10SetPointC10;
    xSemaphoreGive(xLab10Mutex);
    printf_P(PSTR("[SP=%d.%d C]\n"), sp / 10, abs(sp) % 10);
}

static void SubmitSetPoint10(int16_t spC10) {
    int16_t sp = ClampSetPoint10(spC10);
    xSemaphoreTake(xLab10Mutex, portMAX_DELAY);
    Lab10SetPointC10 = sp;
    xSemaphoreGive(xLab10Mutex);
    printf_P(PSTR("[SP=%d.%d C]\n"), sp / 10, abs(sp) % 10);
}

// Parse a Q8.8 fixed-point gain from a decimal string ("1.25" → 1.25 * 256).
// Accepts a leading sign and up to 4 fractional digits.
static bool ParseQ88(const char* s, int32_t* outQ88) {
    if (!s || !*s) return false;

    int sign = 1;
    if (*s == '-') { sign = -1; ++s; }
    else if (*s == '+') ++s;

    int32_t intPart = 0;
    bool sawDigit = false;
    while (*s >= '0' && *s <= '9') {
        intPart = intPart * 10 + (*s - '0');
        sawDigit = true;
        ++s;
    }

    int32_t fracNum = 0, fracDen = 1;
    if (*s == '.') {
        ++s;
        for (uint8_t i = 0; i < 4 && *s >= '0' && *s <= '9'; ++i) {
            fracNum = fracNum * 10 + (*s - '0');
            fracDen *= 10;
            sawDigit = true;
            ++s;
        }
    }

    if (!sawDigit || *s != '\0') return false;

    int32_t q88 = intPart * 256 + (fracNum * 256 + fracDen / 2) / fracDen;
    *outQ88 = sign * q88;
    return true;
}

// Process a complete serial line. Recognises:
//   "NN"      → set setpoint to NN °C
//   "P 1.5"   → Kp = 1.5
//   "I 0.05"  → Ki
//   "D 0.5"   → Kd
//   "RESET"   → clear PID state
static void ProcessSerialLine(char* line) {
    // Strip leading whitespace
    while (*line == ' ' || *line == '\t') ++line;
    if (*line == '\0') return;

    char head = (char)toupper((unsigned char)*line);
    if (head == 'P' || head == 'I' || head == 'D') {
        char* arg = line + 1;
        while (*arg == ' ' || *arg == '\t' || *arg == '=') ++arg;

        int32_t q88;
        if (!ParseQ88(arg, &q88)) {
            printf_P(PSTR("[bad gain: %s]\n"), line);
            return;
        }

        xSemaphoreTake(xLab10Mutex, portMAX_DELAY);
        if (head == 'P') Lab10KpQ88 = q88;
        else if (head == 'I') Lab10KiQ88 = q88;
        else                  Lab10KdQ88 = q88;
        // Resetting integrator on gain change avoids surprise windup kicks.
        Lab10Integral  = 0;
        Lab10PrevError = 0;
        xSemaphoreGive(xLab10Mutex);

        printf_P(PSTR("[%c=%ld.%04ld]\n"),
            head, q88 * 10000L / 256,
            labs(q88) * 10000L / 256 % 10000);
        return;
    }

    if (head == 'R') {
        xSemaphoreTake(xLab10Mutex, portMAX_DELAY);
        Lab10Integral  = 0;
        Lab10PrevError = 0;
        Lab10Output    = 0;
        xSemaphoreGive(xLab10Mutex);
        printf_P(PSTR("[PID reset]\n"));
        return;
    }

    // Plain integer → setpoint in whole °C.
    char* endPtr;
    long value = strtol(line, &endPtr, 10);
    if (endPtr != line && (*endPtr == '\0' || *endPtr == ' ')) {
        SubmitSetPoint10((int16_t)(value * 10));
    }
}

void TaskCommandRead10Func(void* pvParameters) {
    const TickType_t Interval = pdMS_TO_TICKS(CommandIntervalMs10);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    char serialBuffer[CmdBufferSize10];
    uint8_t serialIdx = 0;

    char keypadBuffer[CmdBufferSize10];
    uint8_t keypadIdx = 0;

    for (;;) {
        while (Serial.available() > 0) {
            char c = (char)Serial.read();
            if (c == '\n' || c == '\r') {
                if (serialIdx > 0) {
                    serialBuffer[serialIdx] = '\0';
                    ProcessSerialLine(serialBuffer);
                    serialIdx = 0;
                }
            } else if (serialIdx < CmdBufferSize10 - 1) {
                serialBuffer[serialIdx++] = c;
            }
        }

        while (IsKeypadKeyAvailable()) {
            char key = ScanKeypad();

            if (key >= '0' && key <= '9') {
                if (keypadIdx < CmdBufferSize10 - 1) {
                    keypadBuffer[keypadIdx++] = key;
                    putchar(key);
                }
            } else if (key == KeyClear10) {
                keypadIdx = 0;
                printf_P(PSTR(" [cleared]\n"));
            } else if (key == KeyCommit10) {
                if (keypadIdx > 0) {
                    keypadBuffer[keypadIdx] = '\0';
                    long v = strtol(keypadBuffer, nullptr, 10);
                    putchar('\n');
                    SubmitSetPoint10((int16_t)(v * 10));
                    keypadIdx = 0;
                }
            } else if (key == KeyIncSmall10) {
                NudgeSetPoint10(+10);
            } else if (key == KeyDecSmall10) {
                NudgeSetPoint10(-10);
            } else if (key == KeyIncLarge10) {
                NudgeSetPoint10(+100);
            } else if (key == KeyDecLarge10) {
                NudgeSetPoint10(-100);
            }
        }

        vTaskDelayUntil(&xLastWakeTime, Interval);
    }
}
