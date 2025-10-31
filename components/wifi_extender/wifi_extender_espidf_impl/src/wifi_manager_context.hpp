#ifndef HW_WIFIEXTENDER_WIFIMANAGERCONTEXT_HPP
#define HW_WIFIEXTENDER_WIFIMANAGERCONTEXT_HPP

#include "wifi_ap.hpp"
#include "wifi_sta.hpp"
#include "esp_timer.h"
#include "esp_wifi.h"
#include "wifi_extender_if/wifi_extender_if.hpp"

namespace WifiExtender
{

struct WifiManagerContext
{
    WifiManagerContext();

    ~WifiManagerContext();

    void SetStaConfigurationValid(const bool isStaConfValid);

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

    static constexpr int MAX_LISTENERS = 1;

    int m_ApClientsCounter;

    WifiAp m_WifiAp;

    WifiSta m_WifiSta;

    // const std::array<EventListener *, MAX_LISTENERS> m_WifiEventListeners;

    WifiExtenderState m_WifiManagerState;

    bool m_PendingNewConfiguration;

    bool m_StaConfigurationValid;
};

}

#endif