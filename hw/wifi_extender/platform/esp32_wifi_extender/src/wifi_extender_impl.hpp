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

class WifiExtenderImpl:
    public Hw::WifiExtender::WifiExtenderIf
{

public:

    WifiExtenderImpl():
        m_CurrApConfig(),
        m_CurrStaConfig(),
        m_WifiManager()
    {};

    bool Init();

    bool Startup(const Hw::WifiExtender::AccessPointConfig &ap_config,
                const Hw::WifiExtender::StaConfig &sta_config);

    bool Shutdown();

    bool UpdateConfig(const Hw::WifiExtender::AccessPointConfig &ap_config,
                    const Hw::WifiExtender::StaConfig &sta_config);

    bool RegisterListener(Hw::WifiExtender::EventListener * pEventListener);

private:

    Hw::WifiExtender::AccessPointConfig m_CurrApConfig;    
    Hw::WifiExtender::StaConfig m_CurrStaConfig;
    WifiManager m_WifiManager;


};


}

}

}


#endif