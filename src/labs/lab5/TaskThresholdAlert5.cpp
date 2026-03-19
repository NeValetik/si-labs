#include "labs/lab5/Lab5Config.h"
#include "labs/lab5/Lab5Sync.h"
#include "drivers/LedDriver.h"
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

void TaskThresholdAlert5Func(void* pvParameters) {
    const TickType_t Interval = pdMS_TO_TICKS(ThresholdCheckMs);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    // Threshold values in tenths of °C for integer comparison.
    const int16_t ThreshHighC10 = (int16_t)(ThresholdHighC * 10.0f);
    const int16_t ThreshLowC10  = (int16_t)(ThresholdLowC * 10.0f);

    bool alertState = false;
    uint8_t counter = 0;

    for (;;) {
        // Read current temperature from shared data.
        xSemaphoreTake(xLab5Mutex, portMAX_DELAY);
        int16_t tempC10 = Lab5TempC10;
        xSemaphoreGive(xLab5Mutex);

        // Determine raw threshold state with hysteresis.
        bool rawState;
        if (!alertState) {
            // Not in alert: activate only if above high threshold.
            rawState = (tempC10 >= ThreshHighC10);
        } else {
            // In alert: deactivate only if below low threshold.
            rawState = (tempC10 > ThreshLowC10);
        }

        // Debouncing counter.
        if (rawState) {
            if (counter < DebounceMaxCount) counter++;
        } else {
            if (counter > 0) counter--;
        }

        // Update alert state based on counter limits.
        if (counter >= DebounceMaxCount) {
            alertState = true;
        } else if (counter == 0) {
            alertState = false;
        }
        // Intermediate values: keep previous alertState.

        // Drive LEDs.
        SetLedState(GreenLedPin5, !alertState);
        SetLedState(RedLedPin5,    alertState);

        // Store threshold results in shared data.
        xSemaphoreTake(xLab5Mutex, portMAX_DELAY);
        Lab5AlertActive     = alertState;
        Lab5RawThreshold    = rawState;
        Lab5DebounceCounter = counter;
        xSemaphoreGive(xLab5Mutex);

        vTaskDelayUntil(&xLastWakeTime, Interval);
    }
}
