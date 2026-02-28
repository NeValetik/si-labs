#include "labs/lab3/TaskReport.h"
#include "services/PressData.h"
#include <Arduino.h>
#include <stdio.h>

// Flag set from ISR context; consumed in main-loop context.
static volatile bool ReportPending = false;

// Called from scheduler ISR — sets flag only (no I/O in ISR).
void TaskReport() {
    ReportPending = true;
}

// Called from LoopLab3 — safe to use printf here (main context).
void TaskReportProcess() {
    if (!ReportPending) return;
    ReportPending = false;

    // Atomically snapshot and reset stats.
    cli();
    PressStats Stats = GetStats();
    ResetStats();
    sei();

    uint32_t Avg = (Stats.TotalPresses > 0)
                   ? (Stats.TotalDurationMs / Stats.TotalPresses)
                   : 0;

    printf("\n--- Report (10s) ---\n");
    printf("Total : %lu\n", Stats.TotalPresses);
    printf("Short : %lu\n", Stats.ShortPresses);
    printf("Long  : %lu\n", Stats.LongPresses);
    printf("Avg   : %lu ms\n", Avg);
    printf("--------------------\n");
}
