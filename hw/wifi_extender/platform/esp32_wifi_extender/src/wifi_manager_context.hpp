#ifndef HW_WIFIEXTENDER_WIFIMANAGERCONTEXT_HPP
#define HW_WIFIEXTENDER_WIFIMANAGERCONTEXT_HPP

#include "wifi_ap.hpp"
#include "wifi_sta.hpp"
#include "esp_timer.h"
#include "esp_wifi.h"
#include "wifi_extender_if/wifi_extender_if.hpp"

namespace Hw
{

namespace Platform
{

namespace WifiExtender
{

using namespace Hw::WifiExtender;

struct WifiManagerContext
{
    WifiManagerContext():
        m_ApClientsCounter(),
        m_WifiAp(),
        m_WifiSta(),
        m_WifiManagerState(WifiExtenderState::UNINTIALIZED),
        m_PendingNewConfiguration(),
        m_StaConnectionTimer(),
        m_timerArgs()
    {
    };
            
    void Init();

    void UpdateWifiManagerState();

    void OnApStart();

    void OnApStop();

    void OnApStaConnected();

    void OnApStaDisconnected();

    void OnStaConnected();

    void OnStaDisconnected();

    void OnStaStart();

    void OnStaStop();

    void OnStaGotIp();

    void OnStaLostIp();

    static void TimerCallback(void *arg);
    static constexpr int MAX_LISTENERS = 1;
    static constexpr uint64_t TIMER_EXPIRED_TIME_US = 30000000; //10 s
    static constexpr int ALLOWED_WIFI_EVENTS = (WIFI_EVENT_AP_START | WIFI_EVENT_AP_STOP | WIFI_EVENT_AP_STACONNECTED | WIFI_EVENT_AP_STADISCONNECTED |
                                                WIFI_EVENT_STA_CONNECTED | WIFI_EVENT_STA_START | WIFI_EVENT_STA_DISCONNECTED);
    static constexpr int ALLOWED_IP_EVENTS = (IP_EVENT_STA_GOT_IP | IP_EVENT_STA_LOST_IP);
    int m_ApClientsCounter;
    WifiAp m_WifiAp;
    WifiSta m_WifiSta;
    // const std::array<EventListener *, MAX_LISTENERS> m_WifiEventListeners;
    WifiExtenderState m_WifiManagerState;
    bool m_PendingNewConfiguration;
    esp_timer_handle_t m_StaConnectionTimer;
    esp_timer_create_args_t m_timerArgs;
};

}

}

}

#endif