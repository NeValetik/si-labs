#include "labs/lab10/Lab10Config.h"
#include "labs/lab10/Lab10Sync.h"
#include "drivers/LedDriver.h"
#include <Arduino.h>
#include <Servo.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

static Servo Lab10Servo;

void TaskActuator10Func(void* pvParameters) {
    const TickType_t Interval = pdMS_TO_TICKS(ActuatorIntervalMs10);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    Lab10Servo.attach(ServoPin10);
    Lab10Servo.write(ServoCenterAngle10);

    bool yellowBlink = false;

    for (;;) {
        xSemaphoreTake(xLab10Mutex, portMAX_DELAY);
        int16_t out = Lab10Output;
        int16_t sp  = Lab10SetPointC10;
        int16_t pv  = Lab10TempC10;
        xSemaphoreGive(xLab10Mutex);

        // Map control output [-OutputMax, +OutputMax] → servo [0, 180]
        int16_t scaled = (int16_t)((int32_t)out * ServoSwingAngle10 / OutputMax10);
        int16_t angle  = (int16_t)ServoCenterAngle10 + scaled;
        if (angle < 0)   angle = 0;
        if (angle > 180) angle = 180;

        Lab10Servo.write((uint8_t)angle);

        // Status LEDs
        bool saturated = (out >= OutputMax10) || (out <= -OutputMax10);
        bool inBand    = (sp - pv >= -10 && sp - pv <= 10);
        SetLedState(GreenLedPin10, inBand);
        SetLedState(RedLedPin10,   saturated);

        yellowBlink = !yellowBlink;
        SetLedState(YellowLedPin10, yellowBlink);

        xSemaphoreTake(xLab10Mutex, portMAX_DELAY);
        Lab10ServoAngle = (uint8_t)angle;
        xSemaphoreGive(xLab10Mutex);

        vTaskDelayUntil(&xLastWakeTime, Interval);
    }
}
