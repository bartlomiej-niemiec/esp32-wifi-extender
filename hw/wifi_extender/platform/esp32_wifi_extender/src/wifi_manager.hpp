#ifndef HW_WIFIEXTENDER_WIFIMANAGER_HPP
#define HW_WIFIEXTENDER_WIFIMANAGER_HPP

#include "esp_netif.h"
#include "esp_wifi.h"
#include "wifi_extender_if/wifi_extender_if.hpp"

#include "wifi_ap.hpp"
#include "wifi_sta.hpp"

#include <array>

namespace Hw
{

namespace WifiExtender
{
    class WifiEventCallback;
}

namespace Platform
{

namespace WifiExtender
{

class WifiManager:
    public Hw::WifiExtender::WifiExtenderIf
{
    public:

        WifiManager():
            m_WifiAp(),
            m_WifiSta()
        {};

        bool Init();

        bool Startup(const Hw::WifiExtender::AccessPointConfig &ap_config,
                     const Hw::WifiExtender::StaConfig &sta_config);

        bool RegisterListener(Hw::WifiExtender::EventListener * pEventListener);

        bool Shutdown();

        bool UpdateConfig(const Hw::WifiExtender::AccessPointConfig &ap_config,
                        const Hw::WifiExtender::StaConfig &sta_config);

    private:

        TaskHandle_t m_xTaskHandle = nullptr;

        WifiAp m_WifiAp;

        WifiSta m_WifiSta;

        static constexpr int MAX_LISTENERS = 1;

        static std::array<Hw::WifiExtender::WifiEventCallback, MAX_LISTENERS> m_WifiEventListeners;

        void InitTask();

        static void WifiMonitoringTask(void *arg);

        static void wifi_ip_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

        static void wifi_ip_state_machine(esp_event_base_t event_base, int32_t event_id, void* event_data);

        

};


}

}

}


#endif