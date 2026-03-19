#pragma once
#include <Arduino.h>

// NTC thermistor parameters (10k @ 25°C, B=3950).
// Voltage divider: VCC -> R_fixed (10k) -> ADC_pin -> NTC -> GND.
const float NtcR0         = 10000.0f;   // resistance at T0 [ohm]
const float NtcT0K        = 298.15f;    // reference temperature [K] (25°C)
const float NtcBeta       = 3950.0f;    // B constant
const float NtcRFixed     = 10000.0f;   // fixed resistor in voltage divider [ohm]

// Initialize the analog pin for reading.
void InitializeNtcSensor(uint8_t analogPin);

// Read raw ADC value (0-1023).
uint16_t ReadNtcRaw(uint8_t analogPin);

// Convert raw ADC value to temperature in °C.
// Returns temperature as float.
float NtcAdcToTempC(uint16_t adcValue);

// Convert raw ADC value to temperature in tenths of °C (integer).
// e.g. 25.3°C -> 253
int16_t NtcAdcToTempC10(uint16_t adcValue);

// Linear ADC-to-temperature mapping for simulation (potentiometer).
// ADC 0 -> TempMinC, ADC 1023 -> TempMaxC.
// Default range: -10°C to 50°C.
const float LinearTempMinC = -20.0f;
const float LinearTempMaxC =  50.0f;

int16_t LinearAdcToTempC10(uint16_t adcValue);
