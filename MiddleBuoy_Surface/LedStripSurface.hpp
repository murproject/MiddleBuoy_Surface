#pragma once

#include "Pinout.hpp"
#include "Voltmeter.hpp"
#include "Buttons.hpp"
#include <Adafruit_NeoPixel.h>

namespace LedStrip {
    struct Color {
        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;

        Color() {}
        Color(uint8_t _r, uint8_t _g, uint8_t _b) : r(_r), g(_g), b(_b) {}

        uint32_t toUint32() const {
            return Adafruit_NeoPixel::Color(r, g, b);
        }

        operator uint32_t() const {
            return toUint32();
        }
    };
}

namespace LedStrip::Surface {
    const unsigned int ledsCount = 2;
    Color colorUser;
    Color colorStatus;

    Adafruit_NeoPixel ledstripSurface(ledsCount, Pinout::Leds::surface, NEO_GRB + NEO_KHZ800);

    void welcomeIndication() {
        auto color = Adafruit_NeoPixel::Color(0, 0, 25);
        ledstripSurface.fill(color);
        ledstripSurface.show();
    }

    void begin() {
        ledstripSurface.begin();
        ledstripSurface.clear();
        ledstripSurface.show();
        welcomeIndication();
    }

    void setUserInputColor(uint8_t r, uint8_t g, uint8_t b) {
        colorUser.r = r;
        colorUser.g = g;
        colorUser.b = b;
    }

    void setUserInputColor(const Color &color) {
        colorUser.r = color.r;
        colorUser.g = color.g;
        colorUser.b = color.b;
    }

    Color getColorDischarged() {
        static const int timeStretch = 10;
        static const int lightnessDivider = 20;

        uint8_t blink = Adafruit_NeoPixel::sine8((millis() / timeStretch) % 255) / lightnessDivider;

        return {
            blink,
            blink,
            0
        };
    }

    Color getColorIdle() {
        return {0, 0, 0};
    }

    void update() {
        colorStatus = Voltmeter::isLowVoltage ? getColorDischarged() : getColorIdle();

        ledstripSurface.setPixelColor(0, colorUser);
        ledstripSurface.setPixelColor(1, colorStatus);
        ledstripSurface.show();
    }
}
