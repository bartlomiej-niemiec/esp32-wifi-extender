#include "rgbled_if/rgbled_if.hpp"
#include "rgbled_sk68_impl.hpp"
#include <new>

namespace RgbLed
{

static Sk68xxminiHsImpl * pRgbled = nullptr;

RgbLedIf & RgbLedFactory::GetRgbLed()
{
    if (pRgbled == nullptr)
    {
        pRgbled = new (std::nothrow) Sk68xxminiHsImpl();
    }
    return *pRgbled;
}

}