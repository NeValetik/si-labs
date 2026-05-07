#include "labs/lab9/Lab9Config.h"
#include "labs/lab9/Lab9Sync.h"
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <stdio.h>

void TaskDisplay9Func(void* pvParameters) {
    const TickType_t Interval = pdMS_TO_TICKS(DisplayIntervalMs9);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        vTaskDelayUntil(&xLastWakeTime, Interval);

        xSemaphoreTake(xLab9Mutex, portMAX_DELAY);
        int16_t pv    = Lab9TempC10;
        int16_t sp    = Lab9SetPointC10;
        int16_t hyst  = Lab9HysteresisC10;
        bool    out   = Lab9OutputState;
        uint8_t angle = Lab9ServoAngle;
        xSemaphoreGive(xLab9Mutex);

        // Plotter-friendly line — Arduino IDE Serial Plotter splits on whitespace.
        // SP and PV in tenths-of-degree are scaled back to floats by the plotter
        // when printed as integers (it just plots the number). So we send °C×10.
        printf_P(PSTR("SP:%d\tPV:%d\tOUT:%d\tANG:%u\n"),
            sp, pv, out ? 100 : 0, angle);

        // Human-readable status (one line per cycle).
        printf_P(PSTR("# SP=%d.%d C  PV=%d.%d C  H=%d.%d C  OUT=%s\n"),
            sp / 10, abs(sp) % 10,
            pv / 10, abs(pv) % 10,
            hyst / 10, abs(hyst) % 10,
            out ? "ON" : "OFF");
    }
}
