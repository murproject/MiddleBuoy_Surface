#pragma once

#include "MovingAverage.hpp"

namespace Motor::Controller {
    constexpr int smoothingSamples = 5;

    int currentUserLimit = 0;
    int currentUserPower = 0;

    Utils::MovingAverage<int, smoothingSamples> powerSmoothed;

    void setLimit(int8_t limit) {
        currentUserLimit = constrain(limit, 0, 100);
    }

    void setPower(int8_t power) {
        currentUserPower = constrain(power, -currentUserLimit, +currentUserLimit);
        powerSmoothed.add(currentUserPower);
    }

    int8_t getCurrentPower() {
        return constrain(powerSmoothed.get(), -100, 100);
    }
}
