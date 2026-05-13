#include "labs/lab11/Lab11Config.h"
#include "labs/lab11/Lab11Sync.h"
#include "labs/lab11/ButtonLedFsm.h"
#include "drivers/LedDriver.h"
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

// Drives the visual actuators (Green/Red LEDs) from the FSM state and pulses
// the yellow activity LED every tick so the operator can confirm the
// scheduler is alive even when no presses occur.
void TaskActuator11Func(void* pvParameters) {
    const TickType_t Interval = pdMS_TO_TICKS(ActuatorIntervalMs11);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    bool yellowBlink = false;

    for (;;) {
        xSemaphoreTake(xLab11Mutex, portMAX_DELAY);
        LedFsmState state = (LedFsmState)Lab11LedState;
        xSemaphoreGive(xLab11Mutex);

        bool ledOn = (state == LedStateOn);
        SetLedState(GreenLedPin11, !ledOn);
        SetLedState(RedLedPin11,    ledOn);

        yellowBlink = !yellowBlink;
        SetLedState(YellowLedPin11, yellowBlink);

        vTaskDelayUntil(&xLastWakeTime, Interval);
    }
}
