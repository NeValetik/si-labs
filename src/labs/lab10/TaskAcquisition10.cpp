#include "labs/lab10/Lab10Config.h"
#include "labs/lab10/Lab10Sync.h"
#include "drivers/NtcSensor.h"
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

void TaskAcquisition10Func(void* pvParameters) {
    const TickType_t Interval = pdMS_TO_TICKS(AcquisitionIntervalMs10);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        uint16_t raw = ReadNtcRaw(NtcAnalogPin10);
        int16_t  pv  = NtcAdcToTempC10(raw);

        xSemaphoreTake(xLab10Mutex, portMAX_DELAY);
        Lab10RawAdc  = raw;
        Lab10TempC10 = pv;
        xSemaphoreGive(xLab10Mutex);

        vTaskDelayUntil(&xLastWakeTime, Interval);
    }
}
