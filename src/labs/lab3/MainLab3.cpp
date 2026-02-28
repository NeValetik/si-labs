#include <Arduino.h>
#include <avr/interrupt.h>
#include "drivers/ButtonDriver.h"
#include "drivers/LedDriver.h"
#include "drivers/SerialStream.h"
#include "services/StdioRedirect.h"
#include "services/Scheduler.h"
#include "labs/lab3/TaskButtonMeasure.h"
#include "labs/lab3/TaskPressStats.h"
#include "labs/lab3/TaskReport.h"

// ── Pin assignments ───────────────────────────────────────────────────────────
const uint8_t ButtonPin    =  2;
const uint8_t GreenLedPin  = 12;
const uint8_t RedLedPin    = 11;
const uint8_t YellowLedPin = 10;

// ── STDIO ─────────────────────────────────────────────────────────────────────
static SerialStream SerialIo;

// ── Task descriptors ──────────────────────────────────────────────────────────
// Both measure and stats share the same rec/offset so they run in the same
// tick — Task1 (registered first) produces, Task2 consumes in the same ISR call.
static Task_t TaskMeasureDesc = { TaskButtonMeasure, 20,    0,  0 };
static Task_t TaskStatsDesc   = { TaskPressStats,    20,    0,  0 };
static Task_t TaskReportDesc  = { TaskReport,        10000, 50, 0 };

// ── Lab entry points ──────────────────────────────────────────────────────────

void SetupLab3() {
    SerialIo.begin(9600);
    initStdio(&SerialIo);

    InitializeButton(ButtonPin);
    InitializeLed(GreenLedPin);
    InitializeLed(RedLedPin);
    InitializeLed(YellowLedPin);

    TaskButtonMeasureInit(ButtonPin, GreenLedPin, RedLedPin);
    TaskPressStatsInit(YellowLedPin);

    // Disable interrupts while configuring scheduler to avoid partial state.
    cli();
    SchedulerInit();
    SchedulerRegister(&TaskMeasureDesc);   // producer — runs first in tick
    SchedulerRegister(&TaskStatsDesc);     // consumer — runs second in tick
    SchedulerRegister(&TaskReportDesc);    // reporter — every 10 s
    sei();

    printf("Lab 3: Button Monitor\n");
    printf("Short press < 500 ms -> green\n");
    printf("Long  press >= 500ms -> red\n");
    printf("Report every 10 s via STDIO\n");
}

void LoopLab3() {
    // Actual printf output lives here (main context — safe for Serial I/O).
    TaskReportProcess();
}
