# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build & Upload Commands

```bash
# Build default env (uno)
pio run

# Build a specific environment
pio run -e uno
pio run -e mega        # FreeRTOS Lab 4
pio run -e mega_lab5   # FreeRTOS Lab 5 (binary threshold)
pio run -e mega_lab6   # FreeRTOS Lab 6 (analog conditioning)
pio run -e mega_lab7   # FreeRTOS Lab 7 (binary actuator control)
pio run -e mega_lab8   # FreeRTOS Lab 8 (analog actuator control)

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
| 5 | Binary signal – threshold alerting (hysteresis + debounce) | `mega_lab5` |
| 6 | Analog signal conditioning (saturation → median → weighted avg) | `mega_lab6` |
| 7 | Binary actuator control – relay via ON/OFF commands (debounce) | `mega_lab7` |
| 8 | Analog actuator control – motor DC via PWM + signal conditioning + ramp | `mega_lab8` |

## Architecture

### STDIO Abstraction Layer

All labs share the same pattern: one `IStream*` wired into AVR `stdin`/`stdout` via `fdev_setup_stream` in `StdioRedirect`. Labs call `initStdio(&stream)` once in setup, then use `printf`/`scanf`/`getchar` exclusively.

```
printf/scanf
     │
StdioRedirect (fdev_setup_stream)
     │
  IStream*
  ├── SerialStream  →  Hardware UART        (Labs 1, 3, 4, 5, 6, 7, 8)
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

### Lab 5 — Binary Signal (Threshold Alerting) (env:mega_lab5)

NTC thermistor on A0 via 10kΩ voltage divider. Three FreeRTOS tasks:

| Task | Period | Role |
|---|---|---|
| `TaskSensorRead5Func` | 50 ms | Reads ADC, converts to temperature (Steinhart-Hart) |
| `TaskThresholdAlert5Func` | 50 ms | Hysteresis threshold (30°C/28°C) + debounce counter (5 samples) |
| `TaskDisplay5Func` | 500 ms | Prints RAW, temperature, threshold state, debounce counter, alert |

LEDs: Green=normal (D12), Red=alert (D11). All shared data protected by `xLab5Mutex`.

### Lab 6 — Analog Signal Conditioning (env:mega_lab6)

Same NTC thermistor sensor. Signal conditioning pipeline in three FreeRTOS tasks:

| Task | Period | Role |
|---|---|---|
| `TaskSensorRead6Func` | 50 ms | Reads ADC, signals conditioner via binary semaphore |
| `TaskCondition6Func` | event-driven | Saturation [100-900] → Median filter [5] → Weighted avg [50,25,15,10] → Temp conversion |
| `TaskDisplay6Func` | 500 ms | Prints all intermediate pipeline values + final temperature |

LEDs: Green=normal (D12), Red=alert (D11), Yellow=conditioning active blink (D10).

### Lab 7 — Binary Actuator Control (env:mega_lab7)

Relay (simulated as LED on D9) controlled via serial ON/OFF commands. Four FreeRTOS tasks:

| Task | Period | Role |
|---|---|---|
| `TaskCommandRead7Func` | 50 ms | Non-blocking serial read, parses ON/OFF/1/0 commands |
| `TaskCondition7Func` | 50 ms | Temporal debouncing (5 consecutive identical values to confirm state change) |
| `TaskActuatorControl7Func` | 100 ms | Drives relay output, updates status LEDs |
| `TaskDisplay7Func` | 500 ms | Prints CMD, conditioned state, debounce counter, actuator state |

LEDs: Green=OFF/safe (D12), Red=ON/energised (D11), Yellow=activity blink (D10). Relay output on D9.

### Lab 8 — Analog Actuator Control (env:mega_lab8)

DC motor via L298 driver with PWM. Signal conditioning pipeline for command signal. Four FreeRTOS tasks:

| Task | Period | Role |
|---|---|---|
| `TaskCommandRead8Func` | 50 ms | Non-blocking serial read, parses speed 0-100% |
| `TaskCondition8Func` | event-driven | Saturation [0-100] → Median [5] → Weighted avg [50,25,15,10] |
| `TaskActuatorControl8Func` | 50 ms | Ramp (soft start/stop, 2%/tick) → PWM conversion [0-255] → analogWrite |
| `TaskDisplay8Func` | 500 ms | Prints all pipeline values + current speed + PWM duty |

Motor: PWM=D9 (ENA), IN1=D8, IN2=D7. LEDs: Green=OK (D12), Red=limit (D11), Yellow=conditioning blink (D10).

### Component Structure

- `include/interfaces/` — `IStream` pure virtual interface
- `include/drivers/` — `LedDriver`, `SerialStream`, `LcdDriver`, `LcdStream`, `KeypadDriver`, `ButtonDriver`, `NtcSensor`
- `include/services/` — `StdioRedirect`, `PressData`, `Scheduler`
- `include/labs/lab3/` — task headers for Lab 3/4
- `include/labs/lab5/` — Lab 5 config, sync objects, task headers
- `include/labs/lab6/` — Lab 6 config, sync objects, signal conditioner, task headers
- `include/labs/lab7/` — Lab 7 config, sync objects, task headers
- `include/labs/lab8/` — Lab 8 config, sync objects, actuator conditioner, task headers
- `src/components/` — all driver and service implementations
- `src/labs/labN/` — lab entry points (`SetupLabN` / `LoopLabN`)
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
