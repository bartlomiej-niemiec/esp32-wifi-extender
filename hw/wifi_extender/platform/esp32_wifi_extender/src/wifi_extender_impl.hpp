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
        m_CurrentConfig(),
        m_WifiManager()
    {};

    bool Startup(const WifiExtenderConfig & config);

    bool Shutdown();

    bool UpdateConfig(const WifiExtenderConfig & config);

    bool RegisterListener(EventListener * pEventListener);

    WifiExtenderState GetState();

private:

    WifiExtenderConfig m_CurrentConfig;
    WifiManager m_WifiManager;
};


}

}

}


#endif