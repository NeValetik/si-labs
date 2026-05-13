#include "labs/lab11/ButtonLedFsm.h"

LedFsmState ButtonLedFsmStep(LedFsmState current, LedFsmEvent event) {
    switch (current) {
        case LedStateOff:
            if (event == EventButtonPress) return LedStateOn;
            return LedStateOff;

        case LedStateOn:
            if (event == EventButtonPress) return LedStateOff;
            return LedStateOn;
    }
    return LedStateOff;
}

const char* ButtonLedFsmStateName(LedFsmState state) {
    return state == LedStateOn ? "ON" : "OFF";
}
