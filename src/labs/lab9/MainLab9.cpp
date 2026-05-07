#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "drivers/LedDriver.h"
#include "drivers/SerialStream.h"
#include "drivers/KeypadDriver.h"
#include "drivers/NtcSensor.h"
#include "services/StdioRedirect.h"
#include "labs/lab9/Lab9Config.h"
#include "labs/lab9/Lab9Sync.h"
#include "labs/lab9/TaskAcquisition9.h"
#include "labs/lab9/TaskControl9.h"
#include "labs/lab9/TaskActuator9.h"
#include "labs/lab9/TaskDisplay9.h"
#include "labs/lab9/TaskCommandRead9.h"

// ── Shared data definitions ─────────────────────────────────────────────────
volatile uint16_t Lab9RawAdc        = 0;
volatile int16_t  Lab9TempC10       = 0;
volatile int16_t  Lab9SetPointC10   = SetPointDefaultC10;
volatile int16_t  Lab9HysteresisC10 = HysteresisC10;
volatile bool     Lab9OutputState   = false;
volatile uint8_t  Lab9ServoAngle    = ServoAngleNeutral9;

SemaphoreHandle_t xLab9Mutex = nullptr;

static SerialStream SerialIo;

void SetupLab9() {
    SerialIo.begin(9600);
    initStdio(&SerialIo);

    InitializeNtcSensor(NtcAnalogPin9);

    InitializeLed(GreenLedPin9);
    InitializeLed(RedLedPin9);
    InitializeLed(YellowLedPin9);

    SetLedState(GreenLedPin9, true);
    SetLedState(RedLedPin9, false);
    SetLedState(YellowLedPin9, false);

    const uint8_t RowPins[KeypadRowCount] = {
        KeypadRow0Pin9, KeypadRow1Pin9, KeypadRow2Pin9, KeypadRow3Pin9
    };
    const uint8_t ColPins[KeypadColCount] = {
        KeypadCol0Pin9, KeypadCol1Pin9, KeypadCol2Pin9, KeypadCol3Pin9
    };
    InitializeKeypad(RowPins, ColPins);

    xLab9Mutex = xSemaphoreCreateMutex();

    xTaskCreate(TaskAcquisition9Func, "Acq",   192, nullptr, 3, nullptr);
    xTaskCreate(TaskCommandRead9Func, "Cmd",   256, nullptr, 3, nullptr);
    xTaskCreate(TaskControl9Func,     "Ctrl",  128, nullptr, 2, nullptr);
    xTaskCreate(TaskActuator9Func,    "Act",   192, nullptr, 2, nullptr);
    xTaskCreate(TaskDisplay9Func,     "Disp",  256, nullptr, 1, nullptr);

    printf_P(PSTR("Lab 9: ON-OFF Hysteresis (Variant B - servo)\n"));
    printf_P(PSTR("Sensor: NTC on A%u  Servo: D%u\n"),
        NtcAnalogPin9 - A0, ServoPin9);
    printf_P(PSTR("SP=%d.%d C  Hyst=+/-%d.%d C\n"),
        Lab9SetPointC10 / 10, abs(Lab9SetPointC10) % 10,
        HysteresisC10 / 10, HysteresisC10 % 10);
    printf_P(PSTR("Setpoint: serial 'NN' or keypad NN '%c'  *=clr A/B=+/-1 C/D=+/-10\n"),
        KeyCommit9);

    vTaskStartScheduler();
}

void LoopLab9() {
}
