#ifndef HW_WIFIEXTENDER_WIFIEXTENDERIMPL_HPP
#define HW_WIFIEXTENDER_WIFIEXTENDERIMPL_HPP

#include "wifi_extender_if/wifi_extender_if.hpp"
#include "wifi_manager.hpp"
#include "freertos/semphr.h"

namespace WifiExtender
{

class WifiExtenderImpl:
    public WifiExtenderIf
{

public:

    WifiExtenderImpl();

    bool Startup(const WifiExtenderConfig & config);

    bool Shutdown();

    bool UpdateConfig(const WifiExtenderConfig & config);

    bool RegisterListener(EventListener * pEventListener);

    bool TryToReconnect();

    WifiExtenderState GetState() const;

    WifiExtenderScannerIf * GetScanner()
    {
        return &m_WifiManager;
    }

private:

    WifiExtenderConfig m_CurrentConfig;
    WifiManager m_WifiManager;
    SemaphoreHandle_t m_Semaphore;
};


}


#endif