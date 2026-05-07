#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "drivers/LedDriver.h"
#include "drivers/SerialStream.h"
#include "drivers/KeypadDriver.h"
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
volatile uint8_t Lab8ServoAngle     = 0;
volatile bool    Lab8AlertActive    = false;

SemaphoreHandle_t xLab8Mutex    = nullptr;
SemaphoreHandle_t xLab8CmdReady = nullptr;

static SerialStream SerialIo;

void SetupLab8() {
    SerialIo.begin(9600);
    initStdio(&SerialIo);

    // Status LEDs
    InitializeLed(GreenLedPin8);
    InitializeLed(RedLedPin8);
    InitializeLed(YellowLedPin8);

    const uint8_t RowPins[KeypadRowCount] = {
        KeypadRow0Pin8, KeypadRow1Pin8, KeypadRow2Pin8, KeypadRow3Pin8
    };
    const uint8_t ColPins[KeypadColCount] = {
        KeypadCol0Pin8, KeypadCol1Pin8, KeypadCol2Pin8, KeypadCol3Pin8
    };
    InitializeKeypad(RowPins, ColPins);

    SetLedState(GreenLedPin8, true);
    SetLedState(RedLedPin8, false);
    SetLedState(YellowLedPin8, false);

    xLab8Mutex    = xSemaphoreCreateMutex();
    xLab8CmdReady = xSemaphoreCreateBinary();

    xTaskCreate(TaskCommandRead8Func,     "CmdRead",  256, nullptr, 3, nullptr);
    xTaskCreate(TaskCondition8Func,       "Cond",     256, nullptr, 2, nullptr);
    xTaskCreate(TaskActuatorControl8Func, "Actuator", 192, nullptr, 2, nullptr);
    xTaskCreate(TaskDisplay8Func,         "Display",  256, nullptr, 1, nullptr);

    printf_P(PSTR("Lab 8: Analog Actuator Control (Servo)\n"));
    printf_P(PSTR("Servo PWM: D%u  Angle range: %u-%u deg\n"),
        ServoPin8, ServoAngleMin8, ServoAngleMax8);
    printf_P(PSTR("Pipeline: Sat[%u-%u] -> Med[%u] -> WAvg[%u] -> Ramp[%u/tick]\n"),
        SpeedMin8, SpeedMax8, MedianWindowSize8, WeightedAvgSize8, RampStepPerTick8);
    printf_P(PSTR("Commands: serial 'NN' or keypad digits + '%c' (clear='%c' stop='%c')\n"),
        KeyCommit8, KeyClear8, KeyEmergency8);

    vTaskStartScheduler();
}

void LoopLab8() {
}
