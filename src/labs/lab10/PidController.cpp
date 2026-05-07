#include "labs/lab10/PidController.h"

int16_t PidStep(int16_t setPoint, int16_t measured,
                int32_t kpQ88, int32_t kiQ88, int32_t kdQ88,
                int32_t* integral, int16_t* prevError,
                int32_t integralLimit, int16_t outputMax) {
    int16_t error = (int16_t)(setPoint - measured);

    int32_t newIntegral = *integral + error;
    if (newIntegral >  integralLimit) newIntegral =  integralLimit;
    if (newIntegral < -integralLimit) newIntegral = -integralLimit;
    *integral = newIntegral;

    int16_t derivative = (int16_t)(error - *prevError);
    *prevError = error;

    int32_t outQ88 = kpQ88 * (int32_t)error
                   + kiQ88 * newIntegral
                   + kdQ88 * (int32_t)derivative;

    int32_t out = outQ88 >> 8;   // strip Q8.8 fractional bits
    if (out >  outputMax) out =  outputMax;
    if (out < -outputMax) out = -outputMax;
    return (int16_t)out;
}
