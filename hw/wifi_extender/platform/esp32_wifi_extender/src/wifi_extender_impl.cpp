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
    if (m_WifiManager.GetState() > WifiExtenderState::IN_PROGRESS)
    {
        return false;
    }

    if ((ap_config != m_CurrApConfig) || (sta_config != m_CurrStaConfig))
    {
        return m_WifiManager.UpdateConfig(ap_config, sta_config);
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
