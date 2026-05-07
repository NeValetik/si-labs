#include "labs/lab10/Lab10Config.h"
#include "labs/lab10/Lab10Sync.h"
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <stdio.h>

void TaskDisplay10Func(void* pvParameters) {
    const TickType_t Interval = pdMS_TO_TICKS(DisplayIntervalMs10);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        vTaskDelayUntil(&xLastWakeTime, Interval);

        xSemaphoreTake(xLab10Mutex, portMAX_DELAY);
        int16_t pv     = Lab10TempC10;
        int16_t sp     = Lab10SetPointC10;
        int16_t out    = Lab10Output;
        uint8_t angle  = Lab10ServoAngle;
        int32_t kpQ88  = Lab10KpQ88;
        int32_t kiQ88  = Lab10KiQ88;
        int32_t kdQ88  = Lab10KdQ88;
        xSemaphoreGive(xLab10Mutex);

        // Plotter line: SP, PV (tenths °C), OUT (-100..+100), ANG (0..180)
        printf_P(PSTR("SP:%d\tPV:%d\tOUT:%d\tANG:%u\n"), sp, pv, out, angle);

        // Human-readable status with PID gains (Q8.8 → ×10000 for 4-decimal print).
        long kpx = (long)kpQ88 * 10000 / 256;
        long kix = (long)kiQ88 * 10000 / 256;
        long kdx = (long)kdQ88 * 10000 / 256;
        printf_P(PSTR("# SP=%d.%d C  PV=%d.%d C  OUT=%d  Kp=%ld.%04ld Ki=%ld.%04ld Kd=%ld.%04ld\n"),
            sp / 10, abs(sp) % 10,
            pv / 10, abs(pv) % 10,
            out,
            kpx / 10000, labs(kpx) % 10000,
            kix / 10000, labs(kix) % 10000,
            kdx / 10000, labs(kdx) % 10000);
    }
}
