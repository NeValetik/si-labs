#include "labs/lab8/Lab8Config.h"
#include "labs/lab8/Lab8Sync.h"
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <stdio.h>

void TaskDisplay8Func(void* pvParameters) {
    const TickType_t Interval = pdMS_TO_TICKS(DisplayIntervalMs8);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        vTaskDelayUntil(&xLastWakeTime, Interval);

        xSemaphoreTake(xLab8Mutex, portMAX_DELAY);
        uint8_t cmd   = Lab8CommandRaw;
        uint8_t sat   = Lab8Saturated;
        uint8_t med   = Lab8MedianFiltered;
        uint8_t wavg  = Lab8WeightedAvg;
        uint8_t tgt   = Lab8RampTarget;
        uint8_t spd   = Lab8CurrentSpeed;
        uint8_t pwm   = Lab8PwmValue;
        bool    alert = Lab8AlertActive;
        xSemaphoreGive(xLab8Mutex);

        printf_P(PSTR("CMD:%u SAT:%u MED:%u AVG:%u TGT:%u SPD:%u%% PWM:%u %s\n"),
            cmd, sat, med, wavg, tgt, spd, pwm,
            alert ? "LIMIT" : "OK");
    }
}
