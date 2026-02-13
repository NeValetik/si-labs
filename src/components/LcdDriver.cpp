// #include "drivers/LcdDriver.h"
// #include <LiquidCrystal_I2C.h>

// static LiquidCrystal_I2C* Lcd      = nullptr;
// static uint8_t             LcdCols  = 0;
// static uint8_t             LcdRows  = 0;
// static uint8_t             CursorCol = 0;
// static uint8_t             CursorRow = 0;

// // Single heap allocation that lives for the full program lifetime.
// void InitializeLcd(uint8_t i2cAddress, uint8_t cols, uint8_t rows) {
//     Lcd       = new LiquidCrystal_I2C(i2cAddress, cols, rows);
//     LcdCols   = cols;
//     LcdRows   = rows;
//     CursorCol = 0;
//     CursorRow = 0;
//     Lcd->init();
//     Lcd->backlight();
//     Lcd->clear();
// }

// void LcdClear() {
//     CursorCol = 0;
//     CursorRow = 0;
//     Lcd->clear();
// }

// void LcdSetCursor(uint8_t col, uint8_t row) {
//     CursorCol = col;
//     CursorRow = row;
//     Lcd->setCursor(col, row);
// }

// // Writes one character and tracks the cursor position.
// // '\r' is ignored (StdioRedirect emits \r\n for every \n).
// // '\n' advances to the next row; wraps to row 0 with a clear when full.
// void LcdPrintChar(char c) {
//     if (c == '\r') return;

//     if (c == '\n') {
//         CursorRow++;
//         CursorCol = 0;
//         if (CursorRow >= LcdRows) {
//             CursorRow = 0;
//             Lcd->clear();
//         }
//         Lcd->setCursor(CursorCol, CursorRow);
//         return;
//     }

//     Lcd->print(c);
//     CursorCol++;

//     if (CursorCol >= LcdCols) {
//         CursorCol = 0;
//         CursorRow++;
//         if (CursorRow >= LcdRows) {
//             CursorRow = 0;
//             Lcd->clear();
//         }
//         Lcd->setCursor(CursorCol, CursorRow);
//     }
// }
