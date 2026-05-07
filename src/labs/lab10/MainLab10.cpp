#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "drivers/LedDriver.h"
#include "drivers/SerialStream.h"
#include "drivers/KeypadDriver.h"
#include "drivers/NtcSensor.h"
#include "services/StdioRedirect.h"
#include "labs/lab10/Lab10Config.h"
#include "labs/lab10/Lab10Sync.h"
#include "labs/lab10/TaskAcquisition10.h"
#include "labs/lab10/TaskControl10.h"
#include "labs/lab10/TaskActuator10.h"
#include "labs/lab10/TaskDisplay10.h"
#include "labs/lab10/TaskCommandRead10.h"

// ── Shared state definitions ────────────────────────────────────────────────
volatile uint16_t Lab10RawAdc      = 0;
volatile int16_t  Lab10TempC10     = 0;
volatile int16_t  Lab10SetPointC10 = SetPointDefaultC10_10;

volatile int32_t  Lab10KpQ88       = Kp_Q88_Default;
volatile int32_t  Lab10KiQ88       = Ki_Q88_Default;
volatile int32_t  Lab10KdQ88       = Kd_Q88_Default;

volatile int32_t  Lab10Integral    = 0;
volatile int16_t  Lab10PrevError   = 0;
volatile int16_t  Lab10Output      = 0;
volatile uint8_t  Lab10ServoAngle  = ServoCenterAngle10;

SemaphoreHandle_t xLab10Mutex = nullptr;

static SerialStream SerialIo;

void SetupLab10() {
    SerialIo.begin(9600);
    initStdio(&SerialIo);

    InitializeNtcSensor(NtcAnalogPin10);

    InitializeLed(GreenLedPin10);
    InitializeLed(RedLedPin10);
    InitializeLed(YellowLedPin10);

    SetLedState(GreenLedPin10, true);
    SetLedState(RedLedPin10, false);
    SetLedState(YellowLedPin10, false);

    const uint8_t RowPins[KeypadRowCount] = {
        KeypadRow0Pin10, KeypadRow1Pin10, KeypadRow2Pin10, KeypadRow3Pin10
    };
    const uint8_t ColPins[KeypadColCount] = {
        KeypadCol0Pin10, KeypadCol1Pin10, KeypadCol2Pin10, KeypadCol3Pin10
    };
    InitializeKeypad(RowPins, ColPins);

    xLab10Mutex = xSemaphoreCreateMutex();

    xTaskCreate(TaskAcquisition10Func, "Acq",  192, nullptr, 3, nullptr);
    xTaskCreate(TaskCommandRead10Func, "Cmd",  256, nullptr, 3, nullptr);
    xTaskCreate(TaskControl10Func,     "PID",  192, nullptr, 2, nullptr);
    xTaskCreate(TaskActuator10Func,    "Act",  192, nullptr, 2, nullptr);
    xTaskCreate(TaskDisplay10Func,     "Disp", 256, nullptr, 1, nullptr);

    printf_P(PSTR("Lab 10: PID Control (Variant D - servo)\n"));
    printf_P(PSTR("Sensor: NTC on A%u  Servo: D%u\n"),
        NtcAnalogPin10 - A0, ServoPin10);
    printf_P(PSTR("SP=%d.%d C  PID period=%u ms\n"),
        Lab10SetPointC10 / 10, abs(Lab10SetPointC10) % 10,
        ControlIntervalMs10);
    printf_P(PSTR("Setpoint: serial 'NN' or keypad NN '%c'  *=clr A/B=+/-1 C/D=+/-10\n"),
        KeyCommit10);
    printf_P(PSTR("Tune via serial: 'P1.5'  'I0.05'  'D0.5'  'RESET'\n"));

    vTaskStartScheduler();
}

void LoopLab10() {
}
