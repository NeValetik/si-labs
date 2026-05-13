#include "labs/lab11/Lab11Config.h"
#include "labs/lab11/Lab11Sync.h"
#include "labs/lab11/ButtonLedFsm.h"
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

// Event-driven FSM driver. Blocks on the binary semaphore that the button
// reader gives on every confirmed press; on wake-up it runs one transition
// step and publishes the new state under the shared mutex.
void TaskFsm11Func(void* pvParameters) {
    for (;;) {
        if (xSemaphoreTake(xLab11PressSem, portMAX_DELAY) != pdTRUE) {
            continue;
        }

        xSemaphoreTake(xLab11Mutex, portMAX_DELAY);
        LedFsmState next = ButtonLedFsmStep((LedFsmState)Lab11LedState, EventButtonPress);
        Lab11LedState = next;
        Lab11PressCount++;
        xSemaphoreGive(xLab11Mutex);
    }
}
