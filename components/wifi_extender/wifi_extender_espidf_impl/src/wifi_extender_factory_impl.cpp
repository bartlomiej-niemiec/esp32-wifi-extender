#include "wifi_extender_if/wifi_extender_factory.hpp"
#include "wifi_extender_if/wifi_extender_if.hpp"

#include "wifi_extender_impl.hpp"

#include <new>

namespace WifiExtender
{

WifiExtenderIf & WifiExtenderFactory::GetWifiExtender()
{
    static WifiExtenderIf * pWifiExtenderIf = nullptr;
    if (pWifiExtenderIf == nullptr)
    {
        pWifiExtenderIf = new (std::nothrow) WifiExtenderImpl();
    }
    assert(pWifiExtenderIf != nullptr);

    return *pWifiExtenderIf;
}

}