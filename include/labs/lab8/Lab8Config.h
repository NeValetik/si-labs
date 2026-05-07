#pragma once
#include <Arduino.h>

// ── Pin assignments ─────────────────────────────────────────────────────────
const uint8_t ServoPin8       = 9;    // servo PWM signal
const uint8_t GreenLedPin8    = 12;   // system OK indicator
const uint8_t RedLedPin8      = 11;   // alert / limit indicator
const uint8_t YellowLedPin8   = 10;   // conditioning activity indicator

// Keypad 4×4 — rows driven LOW one at a time, columns INPUT_PULLUP
const uint8_t KeypadRow0Pin8  = 2;
const uint8_t KeypadRow1Pin8  = 3;
const uint8_t KeypadRow2Pin8  = 4;
const uint8_t KeypadRow3Pin8  = 5;
const uint8_t KeypadCol0Pin8  = A0;
const uint8_t KeypadCol1Pin8  = A1;
const uint8_t KeypadCol2Pin8  = A2;
const uint8_t KeypadCol3Pin8  = A3;

// Keypad command keys
const char KeyCommit8     = '#';   // submit accumulated digits as new speed
const char KeyClear8      = '*';   // clear current entry
const char KeyEmergency8  = 'A';   // immediate 0% (emergency stop)

// ── Speed / angle range ─────────────────────────────────────────────────────
const uint8_t SpeedMin8       = 0;    // minimum command [%]
const uint8_t SpeedMax8       = 100;  // maximum command [%]
const uint8_t ServoAngleMin8  = 0;    // servo minimum angle [°]
const uint8_t ServoAngleMax8  = 180;  // servo maximum angle [°]

// ── Signal conditioning parameters ──────────────────────────────────────────
const uint8_t MedianWindowSize8  = 5;
const uint8_t WeightedAvgSize8   = 4;
const uint16_t WeightedAvgWeights8[WeightedAvgSize8] = {50, 25, 15, 10};

// ── Ramp (soft start / stop) ────────────────────────────────────────────────
const uint8_t RampStepPerTick8   = 2;  // command change per control cycle [%]

// ── Task timing ─────────────────────────────────────────────────────────────
const uint16_t CommandReadIntervalMs8    = 50;    // command polling period
const uint16_t ConditionIntervalMs8      = 50;    // conditioning period
const uint16_t ActuatorControlIntervalMs8 = 50;   // actuator drive period
const uint16_t DisplayIntervalMs8        = 500;   // display/report period

// ── Command buffer ──────────────────────────────────────────────────────────
const uint8_t CmdBufferSize8  = 8;
