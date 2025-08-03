#include "wifi_extender_if/wifi_extender_factory.hpp"
#include "wifi_extender_if/wifi_extender_if.hpp"

#include "wifi_extender_impl.hpp"

#include <new>

namespace Hw
{

namespace WifiExtender
{


WifiExtenderIf* WifiExtenderFactory::GetWifiExtenderIf()
{
    static WifiExtenderIf * pWifiExtenderIf = nullptr;
    if (pWifiExtenderIf == nullptr)
    {
        pWifiExtenderIf = new (std::nothrow) Hw::Platform::WifiExtender::WifiExtenderImpl();
    }
    return pWifiExtenderIf;
}

}

}