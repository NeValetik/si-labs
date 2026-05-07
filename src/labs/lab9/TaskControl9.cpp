#include "labs/lab9/Lab9Config.h"
#include "labs/lab9/Lab9Sync.h"
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

// ON-OFF with hysteresis:
//   PV < SP - h  → output ON  (drive servo to "heat" angle)
//   PV > SP + h  → output OFF (drive servo to "cool" angle)
//   inside band  → hold previous output (this *is* the hysteresis)
void TaskControl9Func(void* pvParameters) {
    const TickType_t Interval = pdMS_TO_TICKS(ControlIntervalMs9);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        xSemaphoreTake(xLab9Mutex, portMAX_DELAY);
        int16_t pv   = Lab9TempC10;
        int16_t sp   = Lab9SetPointC10;
        int16_t hyst = Lab9HysteresisC10;
        bool    out  = Lab9OutputState;
        xSemaphoreGive(xLab9Mutex);

        if (pv < sp - hyst) {
            out = true;
        } else if (pv > sp + hyst) {
            out = false;
        }

        xSemaphoreTake(xLab9Mutex, portMAX_DELAY);
        Lab9OutputState = out;
        xSemaphoreGive(xLab9Mutex);

        vTaskDelayUntil(&xLastWakeTime, Interval);
    }
}
