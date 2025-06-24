#include "wifi_access_point/wifi_access_point_factory.hpp"
#include "wifi_access_point_impl.hpp"
#include <new>

namespace Hw
{

namespace WifiAccessPoint
{

WifiAccessPoint* WifiAccessPointFactory::GetWifiAP()
{
    static WifiAccessPointImpl wifiap;
    return &wifiap;
}

}

}