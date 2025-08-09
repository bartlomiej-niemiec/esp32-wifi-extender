#ifndef HW_WIFIEXTENDER_WIFIEXTENDERIMPL_HPP
#define HW_WIFIEXTENDER_WIFIEXTENDERIMPL_HPP

#include "wifi_extender_if/wifi_extender_if.hpp"
#include "wifi_manager.hpp"

namespace Hw
{

namespace Platform
{

namespace WifiExtender
{

using namespace Hw::WifiExtender;

class WifiExtenderImpl:
    public WifiExtenderIf
{

public:

    WifiExtenderImpl():
        m_CurrApConfig(),
        m_CurrStaConfig(),
        m_WifiManager(),
        m_Mode(WifiExtenderMode::FACTORY_DEFAULT_MODE)
    {};

    bool Init(const WifiExtenderMode & mode);

    bool Startup(const AccessPointConfig &ap_config,
                const StaConfig &sta_config);

    bool Shutdown();

    bool UpdateConfig(const AccessPointConfig &ap_config,
                    const StaConfig &sta_config);

    bool RegisterListener(EventListener * pEventListener);

    WifiExtenderState GetState();

private:

    AccessPointConfig m_CurrApConfig;    
    StaConfig m_CurrStaConfig;
    WifiManager m_WifiManager;
    WifiExtenderMode m_Mode;

};


}

}

}


#endif