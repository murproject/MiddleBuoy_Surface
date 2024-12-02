#pragma once

/* 1-byte alignment for structs */
#pragma pack(push, 1)

namespace Protocol {
    const uint8_t packetHeader = 0xAA;
    const uint8_t packetFooter = 0xBB;

    struct Telemetry {
        uint8_t header = packetHeader;

        uint32_t uptime = 0;
        int32_t depth = 0;
        int32_t pressure = 0;
        int32_t altitude = 0;
        int32_t temperature = 0;

        uint8_t footer = packetFooter;
    } packetTelemetry;

    struct Control {
        uint8_t header = packetHeader;

        int8_t motorPower = 0;
        uint8_t ledR = 0;
        uint8_t ledG = 0;
        uint8_t ledB = 0;

        uint8_t footer = packetFooter;
    } packetControl;
}

#pragma pack(pop)
