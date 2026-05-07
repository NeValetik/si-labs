#include "labs/lab10/Lab10Config.h"
#include "labs/lab10/Lab10Sync.h"
#include "labs/lab10/PidController.h"
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

void TaskControl10Func(void* pvParameters) {
    const TickType_t Interval = pdMS_TO_TICKS(ControlIntervalMs10);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        xSemaphoreTake(xLab10Mutex, portMAX_DELAY);
        int16_t pv      = Lab10TempC10;
        int16_t sp      = Lab10SetPointC10;
        int32_t kp      = Lab10KpQ88;
        int32_t ki      = Lab10KiQ88;
        int32_t kd      = Lab10KdQ88;
        int32_t integ   = Lab10Integral;
        int16_t prevErr = Lab10PrevError;
        xSemaphoreGive(xLab10Mutex);

        int16_t output = PidStep(sp, pv, kp, ki, kd,
                                 &integ, &prevErr,
                                 IntegralLimit10, OutputMax10);

        xSemaphoreTake(xLab10Mutex, portMAX_DELAY);
        Lab10Integral  = integ;
        Lab10PrevError = prevErr;
        Lab10Output    = output;
        xSemaphoreGive(xLab10Mutex);

        vTaskDelayUntil(&xLastWakeTime, Interval);
    }
}
