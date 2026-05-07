#pragma once
#include <Arduino.h>

// ── Pin assignments ─────────────────────────────────────────────────────────
const uint8_t NtcAnalogPin10   = A0;
const uint8_t ServoPin10       = 9;
const uint8_t GreenLedPin10    = 12;   // |error| within deadband
const uint8_t RedLedPin10      = 11;   // saturated output (|out|=100)
const uint8_t YellowLedPin10   = 10;   // controller activity blink

const uint8_t KeypadRow0Pin10  = 2;
const uint8_t KeypadRow1Pin10  = 3;
const uint8_t KeypadRow2Pin10  = 4;
const uint8_t KeypadRow3Pin10  = 5;
const uint8_t KeypadCol0Pin10  = A1;
const uint8_t KeypadCol1Pin10  = A2;
const uint8_t KeypadCol2Pin10  = A3;
const uint8_t KeypadCol3Pin10  = A4;

const char KeyCommit10     = '#';
const char KeyClear10      = '*';
const char KeyIncSmall10   = 'A';   // +1 °C
const char KeyDecSmall10   = 'B';   // -1 °C
const char KeyIncLarge10   = 'C';   // +10 °C
const char KeyDecLarge10   = 'D';   // -10 °C

// ── Setpoint range (tenths of °C) ───────────────────────────────────────────
const int16_t SetPointDefaultC10_10  = 250;
const int16_t SetPointMinC10_10      = -100;
const int16_t SetPointMaxC10_10      = 500;

// ── PID gains (Q8.8 fixed-point: stored * 256). All ops use int32_t. ────────
// Defaults: Kp = 4.0, Ki = 0.10, Kd = 0.50.
const int32_t Kp_Q88_Default = (int32_t)(4.0f   * 256);
const int32_t Ki_Q88_Default = (int32_t)(0.10f  * 256);
const int32_t Kd_Q88_Default = (int32_t)(0.50f  * 256);

// ── Output mapping ──────────────────────────────────────────────────────────
// PID output is clamped to [-OutputMax, +OutputMax]. Servo angle = 90 + scale.
const int16_t OutputMax10        = 100;
const uint8_t ServoCenterAngle10 = 90;
const uint8_t ServoSwingAngle10  = 90;   // ±90° around centre → 0..180 full range

// Anti-windup: integral magnitude limit.
const int32_t IntegralLimit10    = 5000;   // tenths-°C·tick units

// ── Task timing ─────────────────────────────────────────────────────────────
const uint16_t AcquisitionIntervalMs10 = 50;
const uint16_t ControlIntervalMs10     = 100;   // PID period
const uint16_t ActuatorIntervalMs10    = 50;
const uint16_t CommandIntervalMs10     = 50;
const uint16_t DisplayIntervalMs10     = 500;

const uint8_t CmdBufferSize10  = 16;
