#pragma once

// Called from the scheduler ISR — sets a pending flag only.
// Rec = 10000 ms, Offset = 50 ms.
void TaskReport();

// Called from LoopLab3 — does the actual printf output.
// Safe to call every loop iteration (no-op when nothing pending).
void TaskReportProcess();
