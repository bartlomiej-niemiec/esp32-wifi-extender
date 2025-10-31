#ifndef RGBLEDIF_RGBLEDIF
#define RGBLEDIF_RGBLEDIF

#include <stdint.h>
#include <array>

namespace RgbLed
{

struct RgbColor {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

class RgbLedIf {
    public:
        virtual void Solid(const RgbColor) = 0;

        virtual void Blink(const RgbColor, const uint32_t frequency_hz) = 0;
};

}

#endif