#include "labs/lab5/Lab5Config.h"
#include "labs/lab5/Lab5Sync.h"
#include "drivers/NtcSensor.h"
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

void TaskSensorRead5Func(void* pvParameters) {
    const TickType_t Interval = pdMS_TO_TICKS(SensorReadIntervalMs);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        uint16_t raw = ReadNtcRaw(NtcAnalogPin);

        int16_t tempC10 = NtcAdcToTempC10(raw);

        xSemaphoreTake(xLab5Mutex, portMAX_DELAY);
        Lab5RawAdc  = raw;
        Lab5TempC10 = tempC10;
        xSemaphoreGive(xLab5Mutex);

        vTaskDelayUntil(&xLastWakeTime, Interval);
    }
}
