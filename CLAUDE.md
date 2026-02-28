# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build & Upload Commands

```bash
# Build default env (uno)
pio run

# Build a specific environment
pio run -e uno
pio run -e mega        # FreeRTOS Lab 3

# Upload to board
pio run -e uno --target upload

# Open serial monitor (COM3, 9600 baud)
pio device monitor
```

There are no tests — this is embedded firmware, verified by flashing to hardware.

## Switching Active Labs

`ACTIVE_LAB` is set per environment in `platformio.ini` (`-D ACTIVE_LAB=N`). The default fallback in `src/main.cpp` is `3`. Override by editing `platformio.ini` or the `#ifndef ACTIVE_LAB` default in `main.cpp`.

| Value | Lab | Environment |
|---|---|---|
| 1 | Serial STDIO – LED control via text | `uno` |
| 2 | LCD + Keypad STDIO – access-code | `uno` |
| 3 | Button monitor – bare-metal scheduler | `uno` |
| 4 | Button monitor – FreeRTOS | `mega` |

## Architecture

### STDIO Abstraction Layer

All labs share the same pattern: one `IStream*` wired into AVR `stdin`/`stdout` via `fdev_setup_stream` in `StdioRedirect`. Labs call `initStdio(&stream)` once in setup, then use `printf`/`scanf`/`getchar` exclusively.

```
printf/scanf
     │
StdioRedirect (fdev_setup_stream)
     │
  IStream*
  ├── SerialStream  →  Hardware UART        (Labs 1, 3, 4)
  └── LcdStream     →  write→LCD, read→Keypad (Lab 2)
```

### Lab 3 — Bare-metal sequential scheduler

Timer1 ISR fires every 1 ms (`OCR1A = 1999`, prescaler 8, CTC mode). The `Scheduler` module (`include/services/Scheduler.h`) runs registered `Task_t` descriptors based on recurrence + offset counters.

Task schedule:
| Task | Rec | Offset | Role |
|---|---|---|---|
| `TaskButtonMeasure` | 20 ms | 0 ms | Producer — reads button, calls `SetPressResult` |
| `TaskPressStats` | 20 ms | 0 ms | Consumer — registered second, runs after producer in same tick |
| `TaskReport` | 10 000 ms | 50 ms | Sets `ReportPending` flag (ISR-safe); actual `printf` in `LoopLab3` |

`TaskReport` only sets a flag from ISR context; `LoopLab3` drains it so that Serial I/O never blocks an ISR.

### Lab 3 — FreeRTOS (env:mega)

RTOS tasks replace the scheduler. `vTaskStartScheduler()` is called in `SetupLab3Rtos()` and never returns.

| Task | Rec | Sync |
|---|---|---|
| `TaskButtonMeasureRtosFunc` | `vTaskDelayUntil` 20 ms | gives `xPressSemaphore` on new press |
| `TaskPressStatsRtosFunc` | blocks on `xPressSemaphore` | takes `xStatsMutex` to write stats |
| `TaskReportRtosFunc` | `vTaskDelayUntil` 10 000 ms | takes `xStatsMutex` to snapshot+reset |

### Component Structure

- `include/interfaces/` — `IStream` pure virtual interface
- `include/drivers/` — `LedDriver`, `SerialStream`, `LcdDriver`, `LcdStream`, `KeypadDriver`, `ButtonDriver`
- `include/services/` — `StdioRedirect`, `PressData`, `Scheduler`
- `include/labs/lab3/` — task headers for Lab 3 (`TaskButtonMeasure`, `TaskPressStats`, `TaskReport`, `SyncObjects`)
- `src/components/` — all driver and service implementations
- `src/labs/labN/` — lab entry points (`SetupLabN` / `LoopLabN`)
- `src/labs/lab3/rtos/` — FreeRTOS task implementations (excluded from `[env:uno]`)
- `src/main.cpp` — lab switcher via `ACTIVE_LAB`

### Key Implementation Details

- `StdioRedirect` emits `\r\n` for every `\n`; `LcdPrintChar` ignores `\r`.
- `LcdDriver` uses `LiquidCrystal` (parallel 4-bit), NOT `LiquidCrystal_I2C`.
- `KeypadDriver` pending-key buffer: `IsKeypadKeyAvailable()` scans, `ScanKeypad()` consumes.
- `setup()`/`loop()` defined unconditionally in `main.cpp` — never inside `#if` blocks.
- `Scheduler`'s `ISR(TIMER1_COMPA_vect)` is guarded by `#ifndef USE_FREERTOS` — the mega env defines this to avoid conflict with FreeRTOS's own timer ISR.
- FreeRTOS header: `#include <Arduino_FreeRTOS.h>` (not `FreeRTOS.h`).

## Conventions

- CamelCase for all identifiers (functions, variables, classes, constants).
- No magic numbers — all pin numbers and timing values as named `const`.
- One peripheral per file pair (`.h` / `.cpp`).
- Header guards via `#pragma once`.
