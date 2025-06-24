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
        m_ssid{},
        m_ssidlen(0),
        m_password{},
        m_passwordlen(0){};

    virtual void Init(const std::string_view &ssid, const std::string_view &password);

    virtual void Startup();

private:

    static const size_t MAX_SSID_LEN = 32;
    static const size_t MAX_PASSWORD_LEN = 64;

    uint8_t m_ssid[MAX_SSID_LEN];
    uint8_t m_ssidlen;
    uint8_t m_password[MAX_PASSWORD_LEN];
    uint8_t m_passwordlen;

};

}

}

#endif