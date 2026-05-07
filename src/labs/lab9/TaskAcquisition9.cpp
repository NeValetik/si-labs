#include "labs/lab9/Lab9Config.h"
#include "labs/lab9/Lab9Sync.h"
#include "drivers/NtcSensor.h"
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

void TaskAcquisition9Func(void* pvParameters) {
    const TickType_t Interval = pdMS_TO_TICKS(AcquisitionIntervalMs9);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        uint16_t raw = ReadNtcRaw(NtcAnalogPin9);
        int16_t  pv  = NtcAdcToTempC10(raw);

        xSemaphoreTake(xLab9Mutex, portMAX_DELAY);
        Lab9RawAdc  = raw;
        Lab9TempC10 = pv;
        xSemaphoreGive(xLab9Mutex);

        vTaskDelayUntil(&xLastWakeTime, Interval);
    }
}
