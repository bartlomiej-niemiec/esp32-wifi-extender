#ifndef HW_WIFIEXTENDER_WIFIMANAGER_HPP
#define HW_WIFIEXTENDER_WIFIMANAGER_HPP

#include "esp_netif.h"
#include "esp_wifi.h"
#include "wifi_extender_if/wifi_extender_if.hpp"

#include "wifi_ap.hpp"
#include "wifi_sta.hpp"
#include "esp_timer.h"

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

using namespace Hw::WifiExtender;

class WifiManager:
    public WifiExtenderIf
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

        bool Startup(const AccessPointConfig &ap_config,
                     const StaConfig &sta_config);

        bool RegisterListener(EventListener * pEventListener);

        bool Shutdown();

        bool UpdateConfig(const AccessPointConfig &ap_config,
                        const StaConfig &sta_config);

        WifiExtenderState GetState();

    private:

        static void wifi_ip_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

        struct WifiManagerContext
        {
            WifiManagerContext():
                m_WifiAp(),
                m_WifiSta(),
                m_WifiEventListeners(),
                m_WifiManagerState(WifiExtenderState::UNINTIALIZED),
                m_PendingNewConfiguration(),
                m_StaConnectionTimer(),
                m_timerArgs()
            {
            };
            
            void Init();
            void UpdateWifiManagerState();
            static void TimerCallback(void *arg);
            static constexpr int MAX_LISTENERS = 1;
            static constexpr uint64_t TIMER_EXPIRED_TIME_US = 10000000; //10 s
            WifiAp m_WifiAp;
            WifiSta m_WifiSta;
            const std::array<WifiEventCallback *, MAX_LISTENERS> m_WifiEventListeners;
            WifiExtenderState m_WifiManagerState;
            bool m_PendingNewConfiguration;
            esp_timer_handle_t m_StaConnectionTimer;
            esp_timer_create_args_t m_timerArgs;
        };

        WifiManagerContext m_WifiManagerContext;
};


}

}

}


#endif