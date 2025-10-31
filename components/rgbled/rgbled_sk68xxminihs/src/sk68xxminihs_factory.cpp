#include "rgbled_if/rgbled_factory.hpp"
#include "rgbled_sk68_impl.hpp"
#include <new>

namespace RgbLed
{

RgbLedFactory::RgbLedFactory(){}

RgbLedFactory & RgbLedFactory::GetInstance()
{
    static RgbLedFactory factory;
    return factory;
}


RgbLedIf * RgbLedFactory::Create(const uint32_t gpio_pin_num)
{
    std::unordered_map<uint32_t, RgbLedIf *>::iterator it;
    it = m_RgbLedIfMap.find(gpio_pin_num);
    RgbLedIf * pRgbLedIf = nullptr;
    if (it == m_RgbLedIfMap.end())
    {
        pRgbLedIf = new (std::nothrow) Sk68xxminiHsImpl(gpio_pin_num);
        assert(nullptr != pRgbLedIf);
        m_RgbLedIfMap[gpio_pin_num] = pRgbLedIf;
    }
    else
    {
        pRgbLedIf = it->second;
    }

    return pRgbLedIf;
}

RgbLedIf * RgbLedFactory::Get(const uint32_t gpio_pin_num)
{
    std::unordered_map<uint32_t, RgbLedIf *>::iterator it;
    it = m_RgbLedIfMap.find(gpio_pin_num);
    return it->second;
}


}
