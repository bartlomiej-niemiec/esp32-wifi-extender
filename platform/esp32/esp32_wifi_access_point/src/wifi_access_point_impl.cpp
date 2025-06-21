#include "wifi_access_point_impl.hpp"

namespace Hw
{

namespace WifiAccessPoint
{

void WifiAccessPointImpl::Init(const std::string_view &ssid, const std::string_view &password)
{
    m_ssid = ssid;
    m_passsword = password;
};

void WifiAccessPointImpl::Startup()
{

};

}

}