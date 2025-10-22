#include "wifi_extender_impl.hpp"

namespace Hw
{

namespace Platform
{

namespace WifiExtender
{

bool WifiExtenderImpl::Startup(const WifiExtenderConfig & config)
{
    if (m_WifiManager.GetState() == WifiExtenderState::STOPPED)
    {
        m_WifiManager.Startup(config);
        m_CurrentConfig = config;
    }
    return true;
}

bool WifiExtenderImpl::RegisterListener(EventListener * pEventListener)
{
    // TO DO
    return true;
}

bool WifiExtenderImpl::Shutdown()
{
    return m_WifiManager.Shutdown();
}

bool WifiExtenderImpl::UpdateConfig(const WifiExtenderConfig & config)
{
    if (m_WifiManager.GetState() <= WifiExtenderState::CONNECTING)
    {
        return false;
    }

    bool isApNewConfig = config.apConfig != m_CurrentConfig.apConfig;
    bool isStaNewConfig = config.staConfig != m_CurrentConfig.staConfig;

    if (isApNewConfig || isStaNewConfig)
    {
        AccessPointConfig newApConfig = isApNewConfig ? config.apConfig : m_CurrentConfig.apConfig;
        StaConfig newStaConfig = isStaNewConfig ? config.staConfig : m_CurrentConfig.staConfig;

        WifiExtenderConfig newConfig(newApConfig, newStaConfig);

        m_WifiManager.UpdateConfig(newConfig);

        m_CurrentConfig = newConfig;

    } 
    return true;
}

WifiExtenderState WifiExtenderImpl::GetState()
{
    return m_WifiManager.GetState();
}

}

}

}
