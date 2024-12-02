#pragma once

#include "Pinout.hpp"

namespace Buttons {
    namespace Pin = Pinout::Buttons;

    bool btnUp, btnDown, btnLedRed, btnLedGreen, btnBoth;

    unsigned long inputTimestamp = 0;
    unsigned long bothHoldTimestamp = 0;
    bool anyIsActive = false;

    bool check(uint8_t pin) {
        bool active = digitalRead(pin) == LOW;
        if (active) {
            anyIsActive = true;
        }
        return active;
    }

    void update() {
        anyIsActive = false;

        btnUp = check(Pin::up);
        btnDown = check(Pin::down);
        btnLedRed = check(Pin::red);
        btnLedGreen = check(Pin::green);

        if (anyIsActive) {
            inputTimestamp = millis();
        }

        bool oldBothState = btnBoth;
        btnBoth = btnUp && btnDown;

        if (btnBoth && !oldBothState) {
            bothHoldTimestamp = millis();
        }
    }

    void begin() {
        pinMode(Pin::up,    INPUT_PULLUP);
        pinMode(Pin::down,  INPUT_PULLUP);
        pinMode(Pin::red,   INPUT_PULLUP);
        pinMode(Pin::green, INPUT_PULLUP);
        update();
    }

    unsigned long timeSinceInput() {
        return millis() - inputTimestamp;
    }

    unsigned long timeSinceBothPressed() {
        if (!btnBoth) {
            return 0;
        } else {
            return millis() - bothHoldTimestamp;
        }
    }

    bool bothLongPress() {
        return btnBoth && timeSinceBothPressed() > 5000;
    }
}
