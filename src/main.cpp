// ─────────────────────────────────────────────────────────────────────────────
// Lab selector
// Change ACTIVE_LAB to switch which lab is built:
//   1 → Lab 1: Serial STDIO – LED control via text commands
//   2 → Lab 2: LCD + Keypad STDIO – access-code verification
// ─────────────────────────────────────────────────────────────────────────────
#define ACTIVE_LAB 2

#include <Arduino.h>

// Forward declarations for all labs
void SetupLab1();
void LoopLab1();
void SetupLab2();
void LoopLab2();

void setup() {
#if ACTIVE_LAB == 1
    SetupLab1();
#elif ACTIVE_LAB == 2
    SetupLab2();
#else
    #error "Unknown ACTIVE_LAB value. Set it to 1 or 2."
#endif
}

void loop() {
#if ACTIVE_LAB == 1
    LoopLab1();
#elif ACTIVE_LAB == 2
    LoopLab2();
#endif
}
