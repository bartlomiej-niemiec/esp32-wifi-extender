#pragma once

#include "rgbled_if.hpp"
#include <unordered_map>

namespace RgbLed
{

class RgbLedFactory {
    public:

        static RgbLedFactory & GetInstance();

        RgbLedIf * Create(const uint32_t gpio_pin_num);

        RgbLedIf * Get(const uint32_t gpio_pin_num);

    private:

        RgbLedFactory();

        std::unordered_map<uint32_t, RgbLedIf *> m_RgbLedIfMap;

};

}