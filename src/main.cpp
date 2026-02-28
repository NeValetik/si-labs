// ─────────────────────────────────────────────────────────────────────────────
// Lab selector — override via platformio.ini build_flags (-D ACTIVE_LAB=N)
// or change the default below:
//   1 → Lab 1: Serial STDIO – LED control via text commands
//   2 → Lab 2: LCD + Keypad STDIO – access-code verification
//   3 → Lab 3: Button press monitor – bare-metal sequential scheduler
//   4 → Lab 4: Button press monitor – FreeRTOS (requires [env:mega])
// ─────────────────────────────────────────────────────────────────────────────
#ifndef ACTIVE_LAB
#define ACTIVE_LAB 3
#endif

#include <Arduino.h>

void SetupLab1();
void LoopLab1();
void SetupLab2();
void LoopLab2();
void SetupLab3();
void LoopLab3();
void SetupLab4();
void LoopLab4();

void setup() {
#if ACTIVE_LAB == 1
    SetupLab1();
#elif ACTIVE_LAB == 2
    SetupLab2();
#elif ACTIVE_LAB == 3
    SetupLab3();
#elif ACTIVE_LAB == 4
    SetupLab4();
#else
    #error "Unknown ACTIVE_LAB value. Set it to 1, 2, 3, or 4."
#endif
}

void loop() {
#if ACTIVE_LAB == 1
    LoopLab1();
#elif ACTIVE_LAB == 2
    LoopLab2();
#elif ACTIVE_LAB == 3
    LoopLab3();
#elif ACTIVE_LAB == 4
    LoopLab4();
#endif
}
