#pragma once
#include <Arduino.h>

// ── Pin assignments ─────────────────────────────────────────────────────────
const uint8_t MotorPwmPin8    = 9;    // PWM output → L298 ENA
const uint8_t MotorIn1Pin8    = 8;    // L298 IN1 (direction)
const uint8_t MotorIn2Pin8    = 7;    // L298 IN2 (direction)
const uint8_t GreenLedPin8    = 12;   // system OK indicator
const uint8_t RedLedPin8      = 11;   // alert / limit indicator
const uint8_t YellowLedPin8   = 10;   // conditioning activity indicator

// ── Speed range ─────────────────────────────────────────────────────────────
const uint8_t SpeedMin8       = 0;    // minimum speed [%]
const uint8_t SpeedMax8       = 100;  // maximum speed [%]

// ── Signal conditioning parameters ──────────────────────────────────────────
const uint8_t MedianWindowSize8  = 5;
const uint8_t WeightedAvgSize8   = 4;
const uint16_t WeightedAvgWeights8[WeightedAvgSize8] = {50, 25, 15, 10};

// ── Ramp (soft start / stop) ────────────────────────────────────────────────
const uint8_t RampStepPerTick8   = 2;  // speed change per control cycle [%]

// ── Task timing ─────────────────────────────────────────────────────────────
const uint16_t CommandReadIntervalMs8    = 50;    // command polling period
const uint16_t ConditionIntervalMs8      = 50;    // conditioning period
const uint16_t ActuatorControlIntervalMs8 = 50;   // actuator drive period
const uint16_t DisplayIntervalMs8        = 500;   // display/report period

// ── Command buffer ──────────────────────────────────────────────────────────
const uint8_t CmdBufferSize8  = 8;
