#include "Pinout.hpp"
#include "Buttons.hpp"
#include "LedStripSurface.hpp"
#include "Display.hpp"
#include "Potentiometer.hpp"
#include "Voltmeter.hpp"
#include "MotorController.hpp"
#include "UART.hpp"
#include "Protocol.hpp"


void setup() {
    UART::begin();
    LedStrip::Surface::begin();
    Buttons::begin();
    Voltmeter::begin();
    Potentiometer::begin();
    Display::begin();
    Display::delayWithProgressbar(2000);
}

void updateUserControls() {
    Buttons::update();

    /* LED */

    uint8_t colorPower = 50;

    LedStrip::Surface::setUserInputColor(
        Buttons::btnLedRed      ? colorPower : 0,
        Buttons::btnLedGreen    ? colorPower : 0,
        0
    );

    /* Motor */

    int8_t motorLimit = Potentiometer::getValue();

    int8_t motorPower = Buttons::btnBoth ? 0           :
                        Buttons::btnUp   ? +motorLimit :
                        Buttons::btnDown ? -motorLimit : 0;

    Motor::Controller::setLimit(motorLimit);
    Motor::Controller::setPower(motorPower);
}

void uptimeCorrection() {
    static int uptimeOffset = 0;

    if (Buttons::bothLongPress()) {
        uptimeOffset = Protocol::packetTelemetry.uptime;
    }

    Protocol::packetTelemetry.uptime -= uptimeOffset;
}

void depthCorrection() {
    static int depthOffset = 0;

    if (Buttons::bothLongPress()) {
        depthOffset = Protocol::packetTelemetry.depth;
    }

    Protocol::packetTelemetry.depth -= depthOffset;
}

void receiveTelemetry() {
    bool received = UART::receiveWhileAvailable(&Protocol::packetTelemetry);
    if (received) {
        depthCorrection();
        uptimeCorrection();
    }
}

void sendControl() {
    auto &control = Protocol::packetControl;

    auto color = LedStrip::Surface::colorUser;
    control.ledR = color.r;
    control.ledG = color.g;
    control.ledB = color.b;

    control.motorPower = Motor::Controller::getCurrentPower();

    UART::transmitPacket(&control);
}

void display() {
    LedStrip::Surface::update();
    Display::update();
}

void loop() {
    receiveTelemetry();
    updateUserControls();
    sendControl();
    display();

    delay(10);
}
