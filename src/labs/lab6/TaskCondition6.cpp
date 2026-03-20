#include "labs/lab6/Lab6Config.h"
#include "labs/lab6/Lab6Sync.h"
#include "labs/lab6/SignalConditioner.h"
#include "drivers/NtcSensor.h"
#include "drivers/LedDriver.h"
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

void TaskCondition6Func(void* pvParameters) {
    uint16_t medianBuffer[MedianWindowSize];
    uint16_t avgBuffer[WeightedAvgSize];
    uint8_t medianIdx = 0;
    uint8_t avgIdx    = 0;
    uint8_t medianFill = 0;
    uint8_t avgFill    = 0;

    for (uint8_t i = 0; i < MedianWindowSize; i++) medianBuffer[i] = 0;
    for (uint8_t i = 0; i < WeightedAvgSize; i++)  avgBuffer[i]    = 0;

    for (;;) {
        xSemaphoreTake(xLab6NewSample, portMAX_DELAY);

        xSemaphoreTake(xLab6Mutex, portMAX_DELAY);
        uint16_t raw = Lab6RawAdc;
        xSemaphoreGive(xLab6Mutex);

        uint16_t saturated = ApplySaturation(raw, SaturationMin, SaturationMax);

        medianBuffer[medianIdx] = saturated;
        medianIdx = (medianIdx + 1) % MedianWindowSize;
        if (medianFill < MedianWindowSize) medianFill++;

        uint16_t medianWork[MedianWindowSize];
        for (uint8_t i = 0; i < MedianWindowSize; i++) {
            medianWork[i] = medianBuffer[i];
        }
        uint16_t median = ApplyMedianFilter(medianWork, medianFill);

        avgBuffer[avgIdx] = median;
        avgIdx = (avgIdx + 1) % WeightedAvgSize;
        if (avgFill < WeightedAvgSize) avgFill++;

        uint16_t orderedBuf[WeightedAvgSize];
        for (uint8_t i = 0; i < avgFill; i++) {
            uint8_t idx = (avgIdx + WeightedAvgSize - 1 - i) % WeightedAvgSize;
            orderedBuf[i] = avgBuffer[idx];
        }
        uint16_t weightedAvg = ApplyWeightedAverage(orderedBuf, WeightedAvgWeights, avgFill);

        int16_t tempC10 = NtcAdcToTempC10(weightedAvg);

        bool alert = (tempC10 >= (int16_t)(AlertThresholdC * 10.0f));

        SetLedState(GreenLedPin6, !alert);
        SetLedState(RedLedPin6,    alert);

        static bool yellowToggle = false;
        yellowToggle = !yellowToggle;
        SetLedState(YellowLedPin6, yellowToggle);

        xSemaphoreTake(xLab6Mutex, portMAX_DELAY);
        Lab6Saturated      = saturated;
        Lab6MedianFiltered = median;
        Lab6WeightedAvg    = weightedAvg;
        Lab6TempC10        = tempC10;
        Lab6AlertActive    = alert;
        xSemaphoreGive(xLab6Mutex);
    }
}
