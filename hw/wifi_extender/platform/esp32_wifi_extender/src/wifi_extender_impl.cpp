#include "wifi_extender_impl.hpp"

namespace Hw
{

namespace Platform
{

namespace WifiExtender
{

bool WifiExtenderImpl::Init()
{
    return m_WifiManager.Init();
}

bool WifiExtenderImpl::Startup(const Hw::WifiExtender::AccessPointConfig &ap_config,
                               const Hw::WifiExtender::StaConfig &sta_config)
{
    return m_WifiManager.Startup(ap_config, sta_config);
}

void WifiExtenderImpl::RegisterListener()
{

}

bool WifiExtenderImpl::Shutdown()
{
    return m_WifiManager.Shutdown();
}

bool WifiExtenderImpl::UpdateConfig(const Hw::WifiExtender::AccessPointConfig &ap_config,
                                    const Hw::WifiExtender::StaConfig &sta_config)
{
    if ((ap_config != m_CurrApConfig) || (sta_config != m_CurrStaConfig))
    {
        return m_WifiManager.UpdateConfig(ap_config, sta_config);
    } 
    return false;
}


}

}

}
