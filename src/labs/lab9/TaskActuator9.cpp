#include "labs/lab9/Lab9Config.h"
#include "labs/lab9/Lab9Sync.h"
#include "drivers/LedDriver.h"
#include <Arduino.h>
#include <Servo.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

static Servo Lab9Servo;

void TaskActuator9Func(void* pvParameters) {
    const TickType_t Interval = pdMS_TO_TICKS(ActuatorIntervalMs9);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    Lab9Servo.attach(ServoPin9);
    Lab9Servo.write(ServoAngleNeutral9);

    bool yellowBlink = false;

    for (;;) {
        xSemaphoreTake(xLab9Mutex, portMAX_DELAY);
        bool out = Lab9OutputState;
        xSemaphoreGive(xLab9Mutex);

        uint8_t angle = out ? ServoAngleHeat9 : ServoAngleCool9;
        Lab9Servo.write(angle);

        SetLedState(GreenLedPin9, !out);
        SetLedState(RedLedPin9,    out);

        yellowBlink = !yellowBlink;
        SetLedState(YellowLedPin9, yellowBlink);

        xSemaphoreTake(xLab9Mutex, portMAX_DELAY);
        Lab9ServoAngle = angle;
        xSemaphoreGive(xLab9Mutex);

        vTaskDelayUntil(&xLastWakeTime, Interval);
    }
}
