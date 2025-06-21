#ifndef PLATFORM_ESP32_WIFIAP_WIFIAP_HPP
#define PLATFORM_ESP32_WIFIAP_WIFIAP_HPP

#include <stdint.h>
#include <string>
#include "wifi_access_point/wifi_access_point.hpp"

namespace Hw
{

namespace WifiAccessPoint
{

class WifiAccessPointImpl:
    public WifiAccessPoint
{
public:
    WifiAccessPointImpl():
        m_ssid(),
        m_passsword(){};

    virtual void Init(const std::string_view &ssid, const std::string_view &password);

    virtual void Startup();

private:

    std::string_view m_ssid;
    std::string_view m_passsword;

};

}

}

#endif