#include "labs/lab11/Lab11Config.h"
#include "labs/lab11/Lab11Sync.h"
#include "drivers/ButtonDriver.h"
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

// Samples the button at ButtonIntervalMs11 and applies temporal debouncing:
// the raw level must be stable for DebounceSamples11 consecutive samples
// before the task accepts it as a new "confirmed" level. A confirmed
// transition from released (HIGH) to pressed (LOW) gives xLab11PressSem
// exactly once per physical press.
void TaskButtonRead11Func(void* pvParameters) {
    const TickType_t Interval = pdMS_TO_TICKS(ButtonIntervalMs11);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    bool    confirmedReleased = true;   // pull-up → idle level is HIGH (released)
    bool    lastRaw           = true;
    uint8_t stableCount       = DebounceSamples11;

    for (;;) {
        bool raw = ReadButtonRaw(ButtonPin11);   // HIGH = released, LOW = pressed

        if (raw == lastRaw) {
            if (stableCount < DebounceSamples11) {
                stableCount++;
            }
        } else {
            lastRaw     = raw;
            stableCount = 1;
        }

        if (stableCount >= DebounceSamples11) {
            bool stableReleased = lastRaw;   // HIGH means released

            if (confirmedReleased && !stableReleased) {
                xSemaphoreGive(xLab11PressSem);
            }
            confirmedReleased = stableReleased;
        }

        vTaskDelayUntil(&xLastWakeTime, Interval);
    }
}
