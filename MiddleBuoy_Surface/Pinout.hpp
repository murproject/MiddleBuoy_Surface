#pragma once

namespace Pinout {
    namespace Uart {
        HardwareSerial *serialBuoy = &Serial1;
        Serial_ *serialLog = &SerialUSB;

        const unsigned long int serialBuoySpeed = 115200;
        const unsigned long int serialLogSpeed = 115200;
    }

    namespace Buttons {
        constexpr int up    = 4;
        constexpr int down  = 11;

        constexpr int red   = 5;
        constexpr int green = 12;
    }

    namespace Leds {
        constexpr int surface = 6;
    }

    namespace Display {
        constexpr uint8_t address = 0x3C;
        constexpr bool rotated = false;
    }

    namespace Analog {
        namespace Voltmeter {
            constexpr int pin = A0;
            constexpr int adcRawMin = 687;
            constexpr int adcRawMax = 975;
            constexpr int adcVoltsMin = 6000;
            constexpr int adcVoltsMax = 8500;
        }

        namespace Potentiometer {
            constexpr int pin = A1;
            constexpr int adcRawMin = 950;
            constexpr int adcRawMax = 30;
        }
    }
}
