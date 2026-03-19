#include "labs/lab6/Lab6Config.h"
#include "labs/lab6/Lab6Sync.h"
#include "labs/lab6/SignalConditioner.h"
#include "drivers/NtcSensor.h"
#include "drivers/LedDriver.h"
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

void TaskCondition6Func(void* pvParameters) {
    // Circular buffers for filtering.
    uint16_t medianBuffer[MedianWindowSize];
    uint16_t avgBuffer[WeightedAvgSize];
    uint8_t medianIdx = 0;
    uint8_t avgIdx    = 0;
    uint8_t medianFill = 0;
    uint8_t avgFill    = 0;

    // Initialize buffers to zero.
    for (uint8_t i = 0; i < MedianWindowSize; i++) medianBuffer[i] = 0;
    for (uint8_t i = 0; i < WeightedAvgSize; i++)  avgBuffer[i]    = 0;

    for (;;) {
        // Wait for new sample from sensor task.
        xSemaphoreTake(xLab6NewSample, portMAX_DELAY);

        // Read raw value.
        xSemaphoreTake(xLab6Mutex, portMAX_DELAY);
        uint16_t raw = Lab6RawAdc;
        xSemaphoreGive(xLab6Mutex);

        // Stage 1: Saturation (clamp to valid range).
        uint16_t saturated = ApplySaturation(raw, SaturationMin, SaturationMax);

        // Stage 2: Median filter (salt & pepper noise removal).
        medianBuffer[medianIdx] = saturated;
        medianIdx = (medianIdx + 1) % MedianWindowSize;
        if (medianFill < MedianWindowSize) medianFill++;

        // Create working copy for median calculation.
        uint16_t medianWork[MedianWindowSize];
        for (uint8_t i = 0; i < MedianWindowSize; i++) {
            medianWork[i] = medianBuffer[i];
        }
        uint16_t median = ApplyMedianFilter(medianWork, medianFill);

        // Stage 3: Weighted average (uniform noise smoothing).
        avgBuffer[avgIdx] = median;
        avgIdx = (avgIdx + 1) % WeightedAvgSize;
        if (avgFill < WeightedAvgSize) avgFill++;

        // Build ordered buffer (newest first) for weighted average.
        uint16_t orderedBuf[WeightedAvgSize];
        for (uint8_t i = 0; i < avgFill; i++) {
            uint8_t idx = (avgIdx + WeightedAvgSize - 1 - i) % WeightedAvgSize;
            orderedBuf[i] = avgBuffer[idx];
        }
        uint16_t weightedAvg = ApplyWeightedAverage(orderedBuf, WeightedAvgWeights, avgFill);

        // Stage 4: Convert conditioned ADC to temperature.
        // LinearAdcToTempC10 for potentiometer simulation,
        // NtcAdcToTempC10 for real NTC thermistor hardware.
        int16_t tempC10 = LinearAdcToTempC10(weightedAvg);

        // Alert check on final conditioned temperature.
        bool alert = (tempC10 >= (int16_t)(AlertThresholdC * 10.0f));

        // Drive LEDs.
        SetLedState(GreenLedPin6, !alert);
        SetLedState(RedLedPin6,    alert);

        // Toggle yellow LED each sample — visible blink at ~10 Hz.
        static bool yellowToggle = false;
        yellowToggle = !yellowToggle;
        SetLedState(YellowLedPin6, yellowToggle);

        // Store all intermediate and final values.
        xSemaphoreTake(xLab6Mutex, portMAX_DELAY);
        Lab6Saturated      = saturated;
        Lab6MedianFiltered = median;
        Lab6WeightedAvg    = weightedAvg;
        Lab6TempC10        = tempC10;
        Lab6AlertActive    = alert;
        xSemaphoreGive(xLab6Mutex);
    }
}
