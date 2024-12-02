#pragma once

#include "Pinout.hpp"

namespace Potentiometer {
    namespace Config = Pinout::Analog::Potentiometer;

    constexpr int percentMin = 10;
    constexpr int percentMax = 100;

    int getValue() {
        int value = analogRead(Config::pin);

        value = map(
            value,
            Config::adcRawMin,
            Config::adcRawMax,
            percentMin,
            percentMax
        );

        value = constrain(value, percentMin, percentMax);
        value = value / 5 * 5;
        return value;
    }

    void begin() {
        pinMode(Config::pin, INPUT);
    }
}
