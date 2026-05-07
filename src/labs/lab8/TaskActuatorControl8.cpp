#include "labs/lab8/Lab8Config.h"
#include "labs/lab8/Lab8Sync.h"
#include "labs/lab8/ActuatorConditioner.h"
#include "drivers/LedDriver.h"
#include <Arduino.h>
#include <Servo.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

static Servo Lab8Servo;

void TaskActuatorControl8Func(void* pvParameters) {
    const TickType_t Interval = pdMS_TO_TICKS(ActuatorControlIntervalMs8);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    Lab8Servo.attach(ServoPin8);
    Lab8Servo.write(ServoAngleMin8);

    uint8_t currentSpeed = 0;

    for (;;) {
        xSemaphoreTake(xLab8Mutex, portMAX_DELAY);
        uint8_t target = Lab8RampTarget;
        bool alert     = Lab8AlertActive;
        xSemaphoreGive(xLab8Mutex);

        // Apply ramp (soft start / stop)
        currentSpeed = ApplyRamp(currentSpeed, target, RampStepPerTick8);

        // Map command percentage [0-100] → servo angle [0-180]
        uint8_t angle = (uint8_t)((uint16_t)currentSpeed * ServoAngleMax8 / SpeedMax8);
        Lab8Servo.write(angle);

        // Status LEDs
        SetLedState(GreenLedPin8, !alert);
        SetLedState(RedLedPin8,    alert);

        xSemaphoreTake(xLab8Mutex, portMAX_DELAY);
        Lab8CurrentSpeed = currentSpeed;
        Lab8ServoAngle   = angle;
        xSemaphoreGive(xLab8Mutex);

        vTaskDelayUntil(&xLastWakeTime, Interval);
    }
}
