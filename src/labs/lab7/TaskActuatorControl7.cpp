#include "labs/lab7/Lab7Config.h"
#include "labs/lab7/Lab7Sync.h"
#include "drivers/LedDriver.h"
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

void TaskActuatorControl7Func(void* pvParameters) {
    const TickType_t Interval = pdMS_TO_TICKS(ActuatorControlIntervalMs7);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        xSemaphoreTake(xLab7Mutex, portMAX_DELAY);
        bool target = Lab7ConditionedState;
        xSemaphoreGive(xLab7Mutex);

        // Drive the relay / actuator output
        SetLedState(RelayPin7, target);

        // Status LEDs: green = OFF (safe), red = ON (energised)
        SetLedState(GreenLedPin7, !target);
        SetLedState(RedLedPin7,    target);

        // Activity blink on yellow
        static bool yellowToggle = false;
        yellowToggle = !yellowToggle;
        SetLedState(YellowLedPin7, yellowToggle);

        xSemaphoreTake(xLab7Mutex, portMAX_DELAY);
        Lab7ActuatorState = target;
        xSemaphoreGive(xLab7Mutex);

        vTaskDelayUntil(&xLastWakeTime, Interval);
    }
}
