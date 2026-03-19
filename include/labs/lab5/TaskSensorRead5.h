#pragma once

// FreeRTOS task: reads NTC sensor via ADC every 50 ms.
// Converts to temperature and stores in shared data.
void TaskSensorRead5Func(void* pvParameters);
