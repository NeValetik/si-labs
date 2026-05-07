#pragma once
#include <Arduino.h>

// PID step using Q8.8 gains and tenths-°C process variable.
//   error      = setpoint - measured                    [tenths °C]
//   integral  += error                                   [tenths °C·tick]
//   derivative = error - prevError                       [tenths °C/tick]
//   output_Q88 = Kp*error + Ki*integral + Kd*derivative  [Q8.8 of "control units"]
//   output     = clamp(output_Q88 >> 8, -outputMax, +outputMax)
//
// Caller owns `integral` and `prevError` storage and must clamp `integral`
// to a sane window (anti-windup).
int16_t PidStep(int16_t setPoint, int16_t measured,
                int32_t kpQ88, int32_t kiQ88, int32_t kdQ88,
                int32_t* integral, int16_t* prevError,
                int32_t integralLimit, int16_t outputMax);
