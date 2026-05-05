#include "labs/lab8/Lab8Config.h"
#include "labs/lab8/Lab8Sync.h"
#include "labs/lab8/ActuatorConditioner.h"
#include "drivers/LedDriver.h"
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

void TaskActuatorControl8Func(void* pvParameters) {
    const TickType_t Interval = pdMS_TO_TICKS(ActuatorControlIntervalMs8);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    uint8_t currentSpeed = 0;

    for (;;) {
        xSemaphoreTake(xLab8Mutex, portMAX_DELAY);
        uint8_t target = Lab8RampTarget;
        bool alert     = Lab8AlertActive;
        xSemaphoreGive(xLab8Mutex);

        // Apply ramp (soft start / stop)
        currentSpeed = ApplyRamp(currentSpeed, target, RampStepPerTick8);

        // Convert speed percentage [0-100] → PWM duty [0-255]
        uint8_t pwm = (uint8_t)((uint16_t)currentSpeed * 255 / 100);
        analogWrite(MotorPwmPin8, pwm);

        // Status LEDs
        SetLedState(GreenLedPin8, !alert);
        SetLedState(RedLedPin8,    alert);

        xSemaphoreTake(xLab8Mutex, portMAX_DELAY);
        Lab8CurrentSpeed = currentSpeed;
        Lab8PwmValue     = pwm;
        xSemaphoreGive(xLab8Mutex);

        vTaskDelayUntil(&xLastWakeTime, Interval);
    }
}
