#pragma once

#include "Pinout.hpp"
#include "MotorController.hpp"
#include "Voltmeter.hpp"
#include "Protocol.hpp"
#include "Buttons.hpp"

#include <Wire.h>
#include <GyverOLED.h>

GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled(Pinout::Display::address);

namespace Display {
    constexpr uint8_t width = 128;
    constexpr uint8_t height = 64;

    bool started = false;

    const auto telemetry = &Protocol::packetTelemetry;

    void prepare() {
        oled.home();
    }

    void blank() {
        /* Clear unused area after text */
        oled.println(F("    "));
    }

    void welcome() {
        prepare();
        oled.setCursorXY(4, 0);
        oled.println(F("MiddleBuoy"));

        oled.setScale(1);
        blank();
        oled.println(F(" Центр Робототехники"));
        oled.println(F("   ROBOCENTER.NET"));
        blank();
        oled.setScale(2);
    }

    void progressbarTick() {
        static uint16_t x = 0;
        oled.fastLineV(x++, height-6, height);
    }

    void delayWithProgressbar(int msToWait) {
        int tickDuration = msToWait / Display::width;

        for (unsigned int i = 0; i < Display::width; i++) {
            progressbarTick();
            delay(tickDuration);
        }
    }

    void setRightColumn(int row) {
        static const int letterWidth = 6;
        static const int letterCount = 6;
        static const int letterSize = 2;
        static const int offset = 2;
        static const int x = letterWidth * letterCount * letterSize - offset;
        oled.setCursorXY(x, 16 * row);
    }

    bool showCalibScreen() {
        if (Buttons::btnBoth && !Buttons::bothLongPress() && Buttons::timeSinceBothPressed() > 500) {
            oled.print(F("Калибровка"));
            blank();

            oled.setScale(1);
            oled.println(F("Чтобы исправить       "));
            oled.println(F("отклонение глубины,   "));
            oled.println(F("зажмите на 5 сек      "));
            oled.println(F("кнопки вверх + вниз   "));
            oled.println(F("когда буй НА ВОЗДУХЕ  "));

            for(int i = 0; i < ((Buttons::timeSinceBothPressed() - 500) / 200); i++) {
                oled.print(F("."));
            }

            oled.print(F("                        "));
            oled.setScale(2);
            return true;
        } else {
            return false;
        }
    }

    void showDepth() {
        oled.print(F("Глуб.:"));
        setRightColumn(0);

        int depth = round(telemetry->depth * 0.1f);

        oled.print(depth);
        blank();
    }

    void showTime() {
        oled.print(F("Время:"));
        setRightColumn(1);

        int minutes = telemetry->uptime / 60;
        int seconds = telemetry->uptime % 60;

        oled.print(minutes);
        oled.print(seconds < 10 ? F(":0") : F(":"));
        oled.print(seconds);

        blank();
    }

    void showMotorPower() {
        int motorPower = Motor::Controller::currentUserPower;

        oled.print(F("Мотор:"));
        setRightColumn(2);

        oled.print(
            motorPower > 0 ? F("+") :
            motorPower < 0 ? F("-") :
            F(" ")
        );

        oled.print(Motor::Controller::currentUserLimit);
        oled.print(F("%"));
        blank();
    }

    void showVoltage() {
        float voltage = Voltmeter::getVoltage();
        oled.print(F("Вольт:"));
        setRightColumn(3);
        oled.print(voltage);
        blank();
    }

    void clearOnFirstUpdate() {
        static bool firstUpdate = true;

        if (firstUpdate) {
            firstUpdate = false;
            oled.clear();
        }
    }

    void update() {
        if (!started) {
            return;
        }

        clearOnFirstUpdate();
        prepare();

        if (showCalibScreen()) {
            return;
        }

        showDepth();
        showTime();
        showMotorPower();
        showVoltage();
    }

    void begin() {
        started = true;

        oled.init();
        Wire.setClock(800000L);
        Wire.setTimeout(50);

        oled.clear();
        oled.update();
        oled.setScale(2);

        if (Pinout::Display::rotated) {
            oled.flipV(true);
            oled.flipH(true);
        }

        welcome();
    }
}
