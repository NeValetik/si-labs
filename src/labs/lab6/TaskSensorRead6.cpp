#include "labs/lab6/Lab6Config.h"
#include "labs/lab6/Lab6Sync.h"
#include "drivers/NtcSensor.h"
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

void TaskSensorRead6Func(void* pvParameters) {
    const TickType_t Interval = pdMS_TO_TICKS(SensorReadIntervalMs6);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        uint16_t raw = ReadNtcRaw(NtcAnalogPin6);

        xSemaphoreTake(xLab6Mutex, portMAX_DELAY);
        Lab6RawAdc = raw;
        xSemaphoreGive(xLab6Mutex);

        xSemaphoreGive(xLab6NewSample);

        vTaskDelayUntil(&xLastWakeTime, Interval);
    }
}
