#include "wifi_extender_if/wifi_extender_factory.hpp"
#include "wifi_extender_if/wifi_extender_if.hpp"

#include "wifi_extender_impl.hpp"

#include <new>

namespace Hw
{

namespace WifiExtender
{

WifiExtenderFactory::WifiExtenderBoundle WifiExtenderFactory::GetWifiExtenderBoundle()
{
    static WifiExtenderIf * pWifiExtenderIf = nullptr;
    if (pWifiExtenderIf == nullptr)
    {
        pWifiExtenderIf = new (std::nothrow) Hw::Platform::WifiExtender::WifiExtenderImpl();
    }

    assert(pWifiExtenderIf != nullptr);

    return {.pWifiExtenderIf = pWifiExtenderIf, .pWifiExtenderScannerIf = nullptr};
}

}

}