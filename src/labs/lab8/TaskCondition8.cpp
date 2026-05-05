#include "labs/lab8/Lab8Config.h"
#include "labs/lab8/Lab8Sync.h"
#include "labs/lab8/ActuatorConditioner.h"
#include "drivers/LedDriver.h"
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

void TaskCondition8Func(void* pvParameters) {
    uint8_t medianBuffer[MedianWindowSize8];
    uint8_t avgBuffer[WeightedAvgSize8];
    uint8_t medianIdx  = 0;
    uint8_t avgIdx     = 0;
    uint8_t medianFill = 0;
    uint8_t avgFill    = 0;

    for (uint8_t i = 0; i < MedianWindowSize8; i++) medianBuffer[i] = 0;
    for (uint8_t i = 0; i < WeightedAvgSize8; i++)  avgBuffer[i]    = 0;

    for (;;) {
        // Wait for a new command from the read task
        xSemaphoreTake(xLab8CmdReady, portMAX_DELAY);

        xSemaphoreTake(xLab8Mutex, portMAX_DELAY);
        uint8_t raw = Lab8CommandRaw;
        xSemaphoreGive(xLab8Mutex);

        // 1. Saturation — clamp to valid speed range [0-100%]
        uint8_t saturated = ApplySaturationU8(raw, SpeedMin8, SpeedMax8);

        // 2. Median filter — remove impulse noise
        medianBuffer[medianIdx] = saturated;
        medianIdx = (medianIdx + 1) % MedianWindowSize8;
        if (medianFill < MedianWindowSize8) medianFill++;

        uint8_t medianWork[MedianWindowSize8];
        for (uint8_t i = 0; i < MedianWindowSize8; i++) {
            medianWork[i] = medianBuffer[i];
        }
        uint8_t median = ApplyMedianFilterU8(medianWork, medianFill);

        // 3. Weighted average — smooth fluctuations
        avgBuffer[avgIdx] = median;
        avgIdx = (avgIdx + 1) % WeightedAvgSize8;
        if (avgFill < WeightedAvgSize8) avgFill++;

        uint8_t orderedBuf[WeightedAvgSize8];
        for (uint8_t i = 0; i < avgFill; i++) {
            uint8_t idx = (avgIdx + WeightedAvgSize8 - 1 - i) % WeightedAvgSize8;
            orderedBuf[i] = avgBuffer[idx];
        }
        uint8_t weightedAvg = ApplyWeightedAverageU8(orderedBuf, WeightedAvgWeights8, avgFill);

        // Alert when command at saturation limit
        bool alert = (raw > SpeedMax8);

        // Activity blink
        static bool yellowToggle = false;
        yellowToggle = !yellowToggle;
        SetLedState(YellowLedPin8, yellowToggle);

        xSemaphoreTake(xLab8Mutex, portMAX_DELAY);
        Lab8Saturated      = saturated;
        Lab8MedianFiltered = median;
        Lab8WeightedAvg    = weightedAvg;
        Lab8RampTarget     = weightedAvg;
        Lab8AlertActive    = alert;
        xSemaphoreGive(xLab8Mutex);
    }
}
