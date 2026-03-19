#pragma once

// FreeRTOS task: applies threshold with hysteresis and debouncing.
// Controls green/red LEDs based on alert state.
void TaskThresholdAlert5Func(void* pvParameters);
