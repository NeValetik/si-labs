#pragma once

// FreeRTOS task: applies signal conditioning pipeline.
// Saturation -> Median filter -> Weighted average -> Temperature conversion.
void TaskCondition6Func(void* pvParameters);
