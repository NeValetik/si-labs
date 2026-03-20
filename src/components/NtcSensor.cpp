#include "drivers/NtcSensor.h"
#include <math.h>

void InitializeNtcSensor(uint8_t analogPin) {
    pinMode(analogPin, INPUT);
}

uint16_t ReadNtcRaw(uint8_t analogPin) {
    return (uint16_t)analogRead(analogPin);
}

float NtcAdcToTempC(uint16_t adcValue) {
    // Clamp to avoid division by zero.
    if (adcValue == 0) adcValue = 1;
    if (adcValue >= 1023) adcValue = 1022;

    float resistance = NtcRFixed * (float)adcValue / (1023.0f - (float)adcValue);

    float steinhart = log(resistance / NtcR0) / NtcBeta;
    steinhart += 1.0f / NtcT0K;
    float temperatureK = 1.0f / steinhart;

    return temperatureK - 273.15f;
}

int16_t NtcAdcToTempC10(uint16_t adcValue) {
    float tempC = NtcAdcToTempC(adcValue);
    return (int16_t)(tempC * 10.0f);
}

int16_t LinearAdcToTempC10(uint16_t adcValue) {
    // Linear: ADC 0 -> MinC*10, ADC 1023 -> MaxC*10
    int16_t minC10 = (int16_t)(LinearTempMinC * 10.0f);
    int16_t maxC10 = (int16_t)(LinearTempMaxC * 10.0f);
    return minC10 + (int32_t)(maxC10 - minC10) * adcValue / 1023;
}
