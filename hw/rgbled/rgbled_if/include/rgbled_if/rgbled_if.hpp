#ifndef RGBLEDIF_RGBLEDIF
#define RGBLEDIF_RGBLEDIF

#include <stdint.h>
#include <array>

namespace RgbLed
{

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

enum class ColorType {
    RED,
    BLUE,
    GREEN,
    BLACK,
    WHITE,
    PINK,
    YELLOW,
    PURPLE,
    CYAN,
    MAGENTA,
    OLIVE,
    NAVY,
    GRAY,
    ORANGE,
    BROWN,
    TEAL,
    COLOR_COUNT
};

class RgbLedIf {
    public:
        virtual void Solid(const Color) = 0;

        virtual void Blink(const Color, const uint32_t frequency_hz) = 0;
};

class RgbLedFactory {
    public:
        static RgbLedIf & GetRgbLed();
};

class ColorCreator{
    public:

        static constexpr Color CreateColor(ColorType color){
            switch (color)
            {
                case ColorType::RED: return {255, 0, 0};
                case ColorType::BLUE: return {0, 0, 255};
                case ColorType::GREEN: return {0, 255, 0};
                case ColorType::BLACK: return {0, 0, 0};
                case ColorType::YELLOW: return {255, 255, 0};
                case ColorType::WHITE: return {255, 255, 255};
                case ColorType::PINK: return {255, 192, 203};
                case ColorType::PURPLE: return {153, 0, 153};
                case ColorType::CYAN: return {0, 255, 255};
                case ColorType::MAGENTA: return {255, 0, 255};
                case ColorType::OLIVE: return {128, 128, 0};
                case ColorType::NAVY: return {0, 0, 128};
                case ColorType::GRAY: return {128, 128, 128};
                case ColorType::ORANGE: return {255, 156, 0};
                case ColorType::BROWN: return {165,42,42};
                case ColorType::TEAL: return {0, 128, 128};

                default:
                    break;
            };

            return Color();
        }
};


static const std::array<ColorType, static_cast<std::size_t>(ColorType::COLOR_COUNT)> colorArray = {
    ColorType::RED,
    ColorType::BLUE,
    ColorType::GREEN,
    ColorType::BLACK,
    ColorType::YELLOW,
    ColorType::WHITE,
    ColorType::PINK,
    ColorType::PURPLE,
    ColorType::CYAN,
    ColorType::MAGENTA,
    ColorType::OLIVE,
    ColorType::NAVY,
    ColorType::GRAY,
    ColorType::ORANGE,
    ColorType::BROWN,
    ColorType::TEAL
};

}

#endif