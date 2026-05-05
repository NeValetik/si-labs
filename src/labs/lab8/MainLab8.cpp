#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "drivers/LedDriver.h"
#include "drivers/SerialStream.h"
#include "services/StdioRedirect.h"
#include "labs/lab8/Lab8Config.h"
#include "labs/lab8/Lab8Sync.h"
#include "labs/lab8/TaskCommandRead8.h"
#include "labs/lab8/TaskCondition8.h"
#include "labs/lab8/TaskActuatorControl8.h"
#include "labs/lab8/TaskDisplay8.h"

// ── Shared data definitions ─────────────────────────────────────────────────
volatile uint8_t Lab8CommandRaw     = 0;
volatile bool    Lab8CommandNew     = false;
volatile uint8_t Lab8Saturated      = 0;
volatile uint8_t Lab8MedianFiltered = 0;
volatile uint8_t Lab8WeightedAvg    = 0;
volatile uint8_t Lab8RampTarget     = 0;
volatile uint8_t Lab8CurrentSpeed   = 0;
volatile uint8_t Lab8PwmValue       = 0;
volatile bool    Lab8AlertActive    = false;

SemaphoreHandle_t xLab8Mutex    = nullptr;
SemaphoreHandle_t xLab8CmdReady = nullptr;

static SerialStream SerialIo;

void SetupLab8() {
    SerialIo.begin(9600);
    initStdio(&SerialIo);

    // Motor driver pins
    pinMode(MotorPwmPin8, OUTPUT);
    pinMode(MotorIn1Pin8, OUTPUT);
    pinMode(MotorIn2Pin8, OUTPUT);

    analogWrite(MotorPwmPin8, 0);
    digitalWrite(MotorIn1Pin8, HIGH);   // forward direction
    digitalWrite(MotorIn2Pin8, LOW);

    // Status LEDs
    InitializeLed(GreenLedPin8);
    InitializeLed(RedLedPin8);
    InitializeLed(YellowLedPin8);

    SetLedState(GreenLedPin8, true);
    SetLedState(RedLedPin8, false);
    SetLedState(YellowLedPin8, false);

    xLab8Mutex    = xSemaphoreCreateMutex();
    xLab8CmdReady = xSemaphoreCreateBinary();

    xTaskCreate(TaskCommandRead8Func,     "CmdRead",  256, nullptr, 3, nullptr);
    xTaskCreate(TaskCondition8Func,       "Cond",     256, nullptr, 2, nullptr);
    xTaskCreate(TaskActuatorControl8Func, "Actuator", 128, nullptr, 2, nullptr);
    xTaskCreate(TaskDisplay8Func,         "Display",  256, nullptr, 1, nullptr);

    printf_P(PSTR("Lab 8: Analog Actuator Control\n"));
    printf_P(PSTR("Motor PWM: D%u  Dir: D%u/D%u\n"),
        MotorPwmPin8, MotorIn1Pin8, MotorIn2Pin8);
    printf_P(PSTR("Pipeline: Sat[%u-%u] -> Med[%u] -> WAvg[%u] -> Ramp[%u/tick]\n"),
        SpeedMin8, SpeedMax8, MedianWindowSize8, WeightedAvgSize8, RampStepPerTick8);
    printf_P(PSTR("Commands: 0-100 (speed %%)\n"));

    vTaskStartScheduler();
}

void LoopLab8() {
}
