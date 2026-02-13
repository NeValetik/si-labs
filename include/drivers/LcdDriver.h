#pragma once
#include <Arduino.h>

// I2C LCD driver (AiP31068 / PCF8574 backpack, HD44780-compatible).
// Call InitializeLcd() once before any other LCD function.
void InitializeLcd(uint8_t i2cAddress, uint8_t cols, uint8_t rows);

void LcdPrintChar(char c);
void LcdClear();
void LcdSetCursor(uint8_t col, uint8_t row);
