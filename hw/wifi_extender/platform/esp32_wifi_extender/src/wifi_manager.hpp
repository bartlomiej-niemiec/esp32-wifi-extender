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
            m_WifiManagerContext()
            {};

        enum class State {
            UNINITIALIZED,
            STARTED,
            CONNECTED,
            PARTIAL_FAILURE,
            ERROR
        };

        bool Init();

        bool Startup(const Hw::WifiExtender::AccessPointConfig &ap_config,
                     const Hw::WifiExtender::StaConfig &sta_config);

        bool RegisterListener(Hw::WifiExtender::EventListener * pEventListener);

        bool Shutdown();

        bool UpdateConfig(const Hw::WifiExtender::AccessPointConfig &ap_config,
                        const Hw::WifiExtender::StaConfig &sta_config);

        virtual bool IsInternarAvailable();    

    private:

        static void wifi_ip_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

        struct WifiManagerContext
        {
            WifiManagerContext():
                m_WifiAp(),
                m_WifiSta(),
                m_WifiEventListeners(),
                m_WifiManagerState(),
                m_PendingNewConfiguration()
            {};

            void UpdateWifiManagerState();

            static constexpr int MAX_LISTENERS = 1;
            WifiAp m_WifiAp;
            WifiSta m_WifiSta;
            const std::array<Hw::WifiExtender::WifiEventCallback *, MAX_LISTENERS> m_WifiEventListeners;
            State m_WifiManagerState;
            bool m_PendingNewConfiguration;
        };

        WifiManagerContext m_WifiManagerContext;
};


}

}

}


#endif