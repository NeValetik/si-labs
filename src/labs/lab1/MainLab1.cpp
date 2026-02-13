#include <Arduino.h>
#include "drivers/LedDriver.h"
#include "drivers/SerialStream.h"
#include "services/StdioRedirect.h"

const int LED_PIN = 13;

static SerialStream SerialIo;

static void readUserAction() {
    printf("\nEnter command: ");

    while (!SerialIo.available()) {
        delay(10);
    }
    delay(50);

    int c;
    while (SerialIo.available()) {
        c = getchar();
        if (c != '\n' && c != '\r') {
            ungetc(c, stdin);
            break;
        }
    }

    char action[7] = {0};
    int result = scanf("led %6s", action);
    printf("Debug: scanf result = %d, action = '%s'\n", result, action);

    if (result == 1) {
        if (strcmp(action, "on") == 0) {
            SetLedState(LED_PIN, true);
            printf("MSG: LED state changed to ON\n");
        } else if (strcmp(action, "off") == 0) {
            SetLedState(LED_PIN, false);
            printf("MSG: LED state changed to OFF\n");
        } else {
            printf("ERROR: Unknown action '%s'\n", action);
        }
    } else {
        printf("ERROR: Invalid command format\n");
    }

    while (SerialIo.available()) {
        getchar();
    }
    delay(100);
}

void SetupLab1() {
    SerialIo.begin(9600);
    initStdio(&SerialIo);

    InitializeLed(LED_PIN);

    printf("System Initialized...\n");
    printf("Commands: led on | led off\n");
}

void LoopLab1() {
    readUserAction();
}
