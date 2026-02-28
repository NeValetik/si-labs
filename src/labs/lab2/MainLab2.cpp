#include <Arduino.h>
#include "drivers/LedDriver.h"
#include "drivers/LcdDriver.h"
#include "drivers/LcdStream.h"
#include "drivers/KeypadDriver.h"
#include "services/StdioRedirect.h"

const uint8_t GreenLedPin  = 12;
const uint8_t RedLedPin    = 13;

const uint8_t LcdRsPin     = 2;
const uint8_t LcdEnPin     = 3;
const uint8_t LcdD4Pin     = 4;
const uint8_t LcdD5Pin     = 5;
const uint8_t LcdD6Pin     = 6;
const uint8_t LcdD7Pin     = 7;

const uint8_t LcdColumnCount = 16;
const uint8_t LcdRowCount    = 2;

const uint8_t KeypadRow0Pin  = 8;
const uint8_t KeypadRow1Pin  = 9;
const uint8_t KeypadRow2Pin  = 10;
const uint8_t KeypadRow3Pin  = 11;

const uint8_t KeypadCol0Pin  = A0;
const uint8_t KeypadCol1Pin  = A1;
const uint8_t KeypadCol2Pin  = A2;
const uint8_t KeypadCol3Pin  = A3;

const char    ValidCode[]       = "96831111";
const uint8_t CodeLength        = 8;
const uint16_t ResultDisplayMs  = 2000;

static LcdStream LcdIo;

static bool ReadAndVerifyCode() {
    char entered[CodeLength + 1];

    LcdSetCursor(0, 1);
    for (uint8_t i = 0; i < CodeLength; i++) {
        entered[i] = (char)getchar();   
        putchar('*');                  
    }
    entered[CodeLength] = '\0';

    return (strncmp(entered, ValidCode, CodeLength) == 0);
}

void SetupLab2() {
    InitializeLcd(LcdRsPin, LcdEnPin,
                  LcdD4Pin, LcdD5Pin, LcdD6Pin, LcdD7Pin,
                  LcdColumnCount, LcdRowCount);

    const uint8_t RowPins[KeypadRowCount] = {
        KeypadRow0Pin, KeypadRow1Pin, KeypadRow2Pin, KeypadRow3Pin
    };
    const uint8_t ColPins[KeypadColCount] = {
        KeypadCol0Pin, KeypadCol1Pin, KeypadCol2Pin, KeypadCol3Pin
    };
    InitializeKeypad(RowPins, ColPins);

    InitializeLed(GreenLedPin);
    InitializeLed(RedLedPin);

    initStdio(&LcdIo);

    printf("Access Control");
    LcdSetCursor(0, 1);
    printf("Code: %u digits", CodeLength);
    delay(ResultDisplayMs);
}

void LoopLab2() {
    SetLedState(GreenLedPin, false);
    SetLedState(RedLedPin,   false);

    LcdClear();
    LcdSetCursor(0, 0);
    printf("Enter Code:");

    bool isValid = ReadAndVerifyCode();

    LcdClear();
    LcdSetCursor(0, 0);

    if (isValid) {
        SetLedState(GreenLedPin, true);
        printf("Access Granted!");
    } else {
        SetLedState(RedLedPin, true);
        printf("Wrong Code!");
    }

    delay(ResultDisplayMs);
}
