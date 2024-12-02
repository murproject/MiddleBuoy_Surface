#pragma once

#include "Pinout.hpp"
#include "MovingAverage.hpp"

namespace Voltmeter {
    namespace Config = Pinout::Analog::Voltmeter;

    const float lowVoltageLevel = 6.5;
    bool isLowVoltage = false;

    constexpr int smoothingSamples = 10;
    Utils::MovingAverage<int, smoothingSamples> adcSmoothed;

    void begin() {
        analogReference(INTERNAL); // Internal 2.56V Reference Votlage
        pinMode(Config::pin, INPUT);
    }

    int readAdc() {
        return analogRead(Config::pin);
    }

    float getVoltage() {
        static bool firstMeasure = true;

        if (firstMeasure) {
            adcSmoothed.clear(readAdc());
            firstMeasure = false;
        }

        int rawVoltageAdc = readAdc();
        adcSmoothed.add(rawVoltageAdc);
        rawVoltageAdc = adcSmoothed.get();

        float voltage = 0.001f * map(
            rawVoltageAdc,
            Config::adcRawMin,
            Config::adcRawMax,
            Config::adcVoltsMin,
            Config::adcVoltsMax
        );

        isLowVoltage = voltage <= lowVoltageLevel;
        return voltage;
    }
}
