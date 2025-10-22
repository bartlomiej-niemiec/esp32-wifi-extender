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
        if (m_WifiManager.Startup(config))
        {
            m_CurrentConfig = config;
            return true;
        }
    }
    return false;
}

bool WifiExtenderImpl::RegisterListener(EventListener * pEventListener)
{
    return m_WifiManager.RegisterListener(pEventListener);
}

bool WifiExtenderImpl::Shutdown()
{
    if (m_WifiManager.GetState() == WifiExtenderState::CONNECTING ||
        m_WifiManager.GetState() == WifiExtenderState::STA_CANNOT_CONNECT ||
        m_WifiManager.GetState() == WifiExtenderState::RUNNING ||
        m_WifiManager.GetState() == WifiExtenderState::STARTED
    )
    {
        return m_WifiManager.Shutdown();
    }
    return false;
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

        if (m_WifiManager.UpdateConfig(newConfig))
        {
            m_CurrentConfig = newConfig;
            return true;
        }
    } 

    return false;
}

WifiExtenderState WifiExtenderImpl::GetState()
{
    return m_WifiManager.GetState();
}

}

}

}
