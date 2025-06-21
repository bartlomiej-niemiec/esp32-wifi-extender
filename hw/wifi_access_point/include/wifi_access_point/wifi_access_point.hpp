#ifndef HW_WIFIAP_WIFIAP_HPP
#define HW_WIFIAP_WIFIAP_HPP

#include <stdint.h>
#include <string>

namespace Hw
{

namespace WifiAccessPoint
{

class WifiAccessPoint{

public:

    ~WifiAccessPoint() = default;

    virtual void Init(const std::string_view &ssid, const std::string_view &password) = 0;

    virtual void Startup() = 0;

};

}

}

#endif