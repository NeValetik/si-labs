#include "labs/lab5/Lab5Config.h"
#include "labs/lab5/Lab5Sync.h"
#include "drivers/LedDriver.h"
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

void TaskThresholdAlert5Func(void* pvParameters) {
    const TickType_t Interval = pdMS_TO_TICKS(ThresholdCheckMs);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    const int16_t ThreshHighC10 = (int16_t)(ThresholdHighC * 10.0f);
    const int16_t ThreshLowC10  = (int16_t)(ThresholdLowC * 10.0f);

    bool alertState = false;
    uint8_t counter = 0;

    for (;;) {
        xSemaphoreTake(xLab5Mutex, portMAX_DELAY);
        int16_t tempC10 = Lab5TempC10;
        xSemaphoreGive(xLab5Mutex);

        bool rawState;
        if (!alertState) {
            rawState = (tempC10 >= ThreshHighC10);
        } else {
            rawState = (tempC10 > ThreshLowC10);
        }

        if (rawState) {
            if (counter < DebounceMaxCount) counter++;
        } else {
            if (counter > 0) counter--;
        }

        if (counter >= DebounceMaxCount) {
            alertState = true;
        } else if (counter == 0) {
            alertState = false;
        }

        SetLedState(GreenLedPin5, !alertState);
        SetLedState(RedLedPin5,    alertState);

        xSemaphoreTake(xLab5Mutex, portMAX_DELAY);
        Lab5AlertActive     = alertState;
        Lab5RawThreshold    = rawState;
        Lab5DebounceCounter = counter;
        xSemaphoreGive(xLab5Mutex);

        vTaskDelayUntil(&xLastWakeTime, Interval);
    }
}
