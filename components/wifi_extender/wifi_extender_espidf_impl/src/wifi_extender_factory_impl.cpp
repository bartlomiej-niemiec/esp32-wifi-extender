#include "wifi_extender_if/wifi_extender_factory.hpp"
#include "wifi_extender_if/wifi_extender_if.hpp"

#include "wifi_extender_impl.hpp"

#include <new>

namespace WifiExtender
{

WifiExtenderFactory::WifiExtenderFactory()
{
}

const WifiExtenderFactory & WifiExtenderFactory::GetInstance()
{
    static const WifiExtenderFactory factory;
    return factory;
}

WifiExtenderIf & WifiExtenderFactory::GetWifiExtender() const
{
    static WifiExtenderImpl * pWifiExtenderIf = nullptr;
    if (pWifiExtenderIf == nullptr)
    {
        pWifiExtenderIf = new (std::nothrow) WifiExtenderImpl();
        pWifiExtenderIf->Init();
    }
    assert(pWifiExtenderIf != nullptr);

    return *pWifiExtenderIf;
}

}