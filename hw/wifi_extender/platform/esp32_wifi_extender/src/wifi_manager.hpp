#ifndef HW_WIFIEXTENDER_WIFIMANAGER_HPP
#define HW_WIFIEXTENDER_WIFIMANAGER_HPP

#include "esp_netif.h"
#include "esp_wifi.h"
#include "wifi_extender_if/wifi_extender_if.hpp"

#include "wifi_manager_context.hpp"

namespace Hw
{

namespace Platform
{

namespace WifiExtender
{

using namespace Hw::WifiExtender;

class WifiManager:
    public WifiExtenderIf
{
    public:

        WifiManager():
            m_WifiManagerContext(),
            m_IpEventRegistered(false),
            m_WifiEventRegistered(false)
            {};

        bool Init(const WifiExtenderMode & mode);

        bool Startup(const AccessPointConfig &ap_config,
                     const StaConfig &sta_config);

        bool RegisterListener(EventListener * pEventListener);

        bool Shutdown();

        bool UpdateConfig(const AccessPointConfig &ap_config,
                        const StaConfig &sta_config);

        WifiExtenderState GetState();

    private:

        bool InitFactoryMode();

        bool InitOperationMode();

        void Deinit();

        static void wifi_ip_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

        WifiManagerContext m_WifiManagerContext;

        bool m_IpEventRegistered;

        bool m_WifiEventRegistered;
};


}

}

}


#endif