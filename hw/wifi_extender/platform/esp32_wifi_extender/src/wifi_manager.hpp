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
            m_WifiManagerContext()
            {};

        bool Init();

        bool Startup(const AccessPointConfig &ap_config,
                     const StaConfig &sta_config);

        bool RegisterListener(EventListener * pEventListener);

        bool Shutdown();

        bool UpdateConfig(const AccessPointConfig &ap_config,
                        const StaConfig &sta_config);

        WifiExtenderState GetState();

    private:

        void Deinit();

        static void wifi_ip_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

        WifiManagerContext m_WifiManagerContext;
};


}

}

}


#endif