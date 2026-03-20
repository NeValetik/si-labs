#pragma once
#ifdef USE_FREERTOS
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

extern volatile uint16_t Lab6RawAdc;           
extern volatile uint16_t Lab6Saturated;        
extern volatile uint16_t Lab6MedianFiltered;   
extern volatile uint16_t Lab6WeightedAvg;      
extern volatile int16_t  Lab6TempC10;          
extern volatile bool     Lab6AlertActive;      

extern SemaphoreHandle_t xLab6Mutex;
extern SemaphoreHandle_t xLab6NewSample;       

#endif
