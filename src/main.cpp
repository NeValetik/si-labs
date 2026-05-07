// ─────────────────────────────────────────────────────────────────────────────
// Lab selector — override via platformio.ini build_flags (-D ACTIVE_LAB=N)
// or change the default below:
//   1  → Lab 1: Serial STDIO – LED control via text commands
//   2  → Lab 2: LCD + Keypad STDIO – access-code verification
//   3  → Lab 3: Button press monitor – bare-metal sequential scheduler
//   4  → Lab 4: Button press monitor – FreeRTOS (requires [env:mega])
//   5  → Lab 5: Binary signal – threshold alerting with hysteresis (FreeRTOS)
//   6  → Lab 6: Analog signal conditioning pipeline (FreeRTOS)
//   7  → Lab 7: Binary actuator control – relay via keypad ON/OFF (FreeRTOS)
//   8  → Lab 8: Analog actuator control – servo via PWM + ramp (FreeRTOS)
//   9  → Lab 9: ON-OFF hysteresis control – NTC + servo (FreeRTOS)
//   10 → Lab 10: PID control – NTC + servo (FreeRTOS)
// ─────────────────────────────────────────────────────────────────────────────
#ifndef ACTIVE_LAB
#define ACTIVE_LAB 7
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
void SetupLab5();
void LoopLab5();
void SetupLab6();
void LoopLab6();
void SetupLab7();
void LoopLab7();
void SetupLab8();
void LoopLab8();
void SetupLab9();
void LoopLab9();
void SetupLab10();
void LoopLab10();

void setup() {
#if ACTIVE_LAB == 1
    SetupLab1();
#elif ACTIVE_LAB == 2
    SetupLab2();
#elif ACTIVE_LAB == 3
    SetupLab3();
#elif ACTIVE_LAB == 4
    SetupLab4();
#elif ACTIVE_LAB == 5
    SetupLab5();
#elif ACTIVE_LAB == 6
    SetupLab6();
#elif ACTIVE_LAB == 7
    SetupLab7();
#elif ACTIVE_LAB == 8
    SetupLab8();
#elif ACTIVE_LAB == 9
    SetupLab9();
#elif ACTIVE_LAB == 10
    SetupLab10();
#else
    #error "Unknown ACTIVE_LAB value. Set it to 1-10."
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
#elif ACTIVE_LAB == 5
    LoopLab5();
#elif ACTIVE_LAB == 6
    LoopLab6();
#elif ACTIVE_LAB == 7
    LoopLab7();
#elif ACTIVE_LAB == 8
    LoopLab8();
#elif ACTIVE_LAB == 9
    LoopLab9();
#elif ACTIVE_LAB == 10
    LoopLab10();
#endif
}
