#pragma once

#include "Pinout.hpp"
#include "Protocol.hpp"

using namespace Pinout::Uart;

namespace UART {
    const unsigned long packetTimeout = 1000;
    const unsigned long serialTimeout = 5;
    unsigned long receivedTimestamp = 0;

    constexpr size_t bufferRxSize = 128;
    uint8_t bufferRx[bufferRxSize] = { 0x00 };

    void begin() {
        serialBuoy->begin(serialBuoySpeed);
        serialLog->begin(serialLogSpeed);

        serialBuoy->setTimeout(serialTimeout);
        serialLog->setTimeout(serialTimeout);
    }

    template <typename PacketType>
    void transmitPacket(PacketType *packet) {
        serialBuoy->write(reinterpret_cast<uint8_t*>(packet), sizeof(PacketType));
    }

    template <typename PacketType>
    bool receivePacket(PacketType *packet) {
        constexpr size_t packetSize = sizeof(PacketType);
        static_assert(packetSize <= bufferRxSize, "Packet size is bigger than bufferRx");

        uint8_t firstByte = 0x00;
        serialBuoy->readBytes(&firstByte, 1);

        if (firstByte != Protocol::packetHeader) {
            return false;
        }

        memset(bufferRx, 0x00, bufferRxSize);
        bufferRx[0] = firstByte;
        serialBuoy->readBytes(bufferRx+1, packetSize-1);

        if (bufferRx[packetSize-1] != Protocol::packetFooter) {
            return false;
        }

        memcpy(packet, bufferRx, packetSize);
        receivedTimestamp = millis();
        return true;
    }

    template <typename PacketType>
    bool receiveWhileAvailable(PacketType *packet) {
        bool received = false;
        while (serialBuoy->available()) {
            received |= receivePacket(packet);
        }
        return received;
    }

    unsigned long timeSinceLastReceived() {
        return abs(millis() - receivedTimestamp);
    }

    bool isPacketTimeout() {
        return timeSinceLastReceived() > packetTimeout;
    }
}
