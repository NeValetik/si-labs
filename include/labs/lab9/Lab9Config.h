#pragma once
#include <Arduino.h>

// ── Pin assignments ─────────────────────────────────────────────────────────
const uint8_t NtcAnalogPin9   = A0;   // NTC voltage divider
const uint8_t ServoPin9       = 9;    // servo PWM signal
const uint8_t GreenLedPin9    = 12;   // PV at/below set point (OFF)
const uint8_t RedLedPin9      = 11;   // PV above set point (ON / heating)
const uint8_t YellowLedPin9   = 10;   // controller activity blink

// Keypad 4×4 — A0 is busy with NTC, so columns use A1..A4
const uint8_t KeypadRow0Pin9  = 2;
const uint8_t KeypadRow1Pin9  = 3;
const uint8_t KeypadRow2Pin9  = 4;
const uint8_t KeypadRow3Pin9  = 5;
const uint8_t KeypadCol0Pin9  = A1;
const uint8_t KeypadCol1Pin9  = A2;
const uint8_t KeypadCol2Pin9  = A3;
const uint8_t KeypadCol3Pin9  = A4;

// Keypad command keys (digit entry → set point in whole °C)
const char KeyCommit9     = '#';   // submit accumulated digits as new setpoint
const char KeyClear9      = '*';   // clear current entry
const char KeyIncSmall9   = 'A';   // +1 °C
const char KeyDecSmall9   = 'B';   // -1 °C
const char KeyIncLarge9   = 'C';   // +10 °C
const char KeyDecLarge9   = 'D';   // -10 °C

// ── Control configuration (ON-OFF with hysteresis, "Variant B") ─────────────
// All temperatures stored as tenths of °C (int16_t). 250 = 25.0 °C.
const int16_t SetPointDefaultC10  = 250;   // default 25.0 °C
const int16_t SetPointMinC10      = -100;  // -10.0 °C
const int16_t SetPointMaxC10      = 500;   //  50.0 °C
const int16_t HysteresisC10       = 10;    // ±1.0 °C dead-band

// Servo "saturation" — direction switches between two fixed angles.
const uint8_t ServoAngleHeat9     = 180;   // PV below SP → drive "heat" direction
const uint8_t ServoAngleCool9     = 0;     // PV above SP → drive "cool" direction
const uint8_t ServoAngleNeutral9  = 90;    // initial idle angle

// ── Task timing ─────────────────────────────────────────────────────────────
const uint16_t AcquisitionIntervalMs9 = 50;
const uint16_t ControlIntervalMs9     = 50;
const uint16_t ActuatorIntervalMs9    = 50;
const uint16_t CommandIntervalMs9     = 50;
const uint16_t DisplayIntervalMs9     = 500;

// ── Buffer ──────────────────────────────────────────────────────────────────
const uint8_t CmdBufferSize9  = 8;
