#include "wifi_extender_impl.hpp"

namespace Hw
{

namespace Platform
{

namespace WifiExtender
{

bool WifiExtenderImpl::Init()
{
    if (m_WifiManager.GetState() == WifiExtenderState::UNINTIALIZED)
    {
        return m_WifiManager.Init();
    }
    return false;
}

bool WifiExtenderImpl::Startup(const AccessPointConfig &ap_config,
                               const StaConfig &sta_config)
{
    if (m_WifiManager.GetState() == WifiExtenderState::INITIALIZED)
    {
        if ( true == m_WifiManager.Startup(ap_config, sta_config) )
        {
            m_CurrApConfig = ap_config;
            m_CurrStaConfig = sta_config;
            return true;
        }
    }
    return false;
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

bool WifiExtenderImpl::UpdateConfig(const AccessPointConfig &ap_config,
                                    const StaConfig &sta_config)
{
    if (m_WifiManager.GetState() <= WifiExtenderState::IN_PROGRESS)
    {
        return false;
    }

    bool isApNewConfig = ap_config != m_CurrApConfig;
    bool isStaNewConfig = sta_config != m_CurrStaConfig;

    if (isApNewConfig || isStaNewConfig)
    {
        AccessPointConfig newApConfig = isApNewConfig ? ap_config : m_CurrApConfig;
        StaConfig newStaConfig = isStaNewConfig ? sta_config : m_CurrStaConfig;

        m_WifiManager.UpdateConfig(newApConfig, newStaConfig);

        m_CurrApConfig = newApConfig;
        m_CurrStaConfig = sta_config;

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
