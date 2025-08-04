#include "wifi_manager.hpp"
#include "esp_wifi.h"
#include "esp_log.h"

namespace Hw
{

namespace Platform
{

namespace WifiExtender
{

bool WifiManager::Init()
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    if (m_WifiManagerContext.m_WifiSta.GetState() == WifiSta::State::NOT_INITIALIZED)
    {
        m_WifiManagerContext.m_WifiSta.Init();
        m_WifiManagerContext.m_WifiSta.SetState(WifiSta::State::INITIALIZED);
    }

    if (m_WifiManagerContext.m_WifiAp.GetState() == WifiAp::State::NOT_INITIALIZED)
    {
        m_WifiManagerContext.m_WifiAp.Init();
        m_WifiManagerContext.m_WifiAp.SetState(WifiAp::State::INITIALIZED);
    }

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_ip_event_handler,
                                                        reinterpret_cast<void *>(&m_WifiManagerContext),
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &wifi_ip_event_handler,
                                                        reinterpret_cast<void *>(&m_WifiManagerContext),
                                                        &instance_got_ip));

    return true;
}


bool WifiManager::Startup(const Hw::WifiExtender::AccessPointConfig &ap_config,
                          const Hw::WifiExtender::StaConfig &sta_config)
{
    if (m_WifiManagerContext.m_WifiSta.GetState() == WifiSta::State::INITIALIZED)
    {
        assert(m_WifiManagerContext.m_WifiSta.SetConfig(sta_config) == true);
        m_WifiManagerContext.m_WifiSta.SetState(WifiSta::State::CONFIGURED);
    }

    if (m_WifiManagerContext.m_WifiAp.GetState() == WifiAp::State::INITIALIZED)
    {
        assert(m_WifiManagerContext.m_WifiAp.SetConfig(ap_config) == true);
        m_WifiManagerContext.m_WifiAp.SetState(WifiAp::State::CONFIGURED);
    }



    ESP_ERROR_CHECK(esp_wifi_start());

    return true;
}


bool WifiManager::Shutdown()
{
    assert(false);
    return false;
}

bool WifiManager::UpdateConfig(const Hw::WifiExtender::AccessPointConfig &ap_config,
                                const Hw::WifiExtender::StaConfig &sta_config)
{
    m_WifiManagerContext.m_PendingNewConfiguration = true;
    return true;
}

void WifiManager::wifi_ip_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{

    static int sta_connected_coutner = 0;
    constexpr int ALLOWED_WIFI_EVENTS = (WIFI_EVENT_AP_START | WIFI_EVENT_AP_STOP |
                                              WIFI_EVENT_AP_STACONNECTED | WIFI_EVENT_AP_STADISCONNECTED |
                                              WIFI_EVENT_STA_CONNECTED | WIFI_EVENT_STA_START |
                                              WIFI_EVENT_STA_DISCONNECTED);

    constexpr int ALLOWED_IP_EVENTS = (IP_EVENT_STA_GOT_IP | IP_EVENT_STA_LOST_IP);

    WifiManagerContext* pWifiManagerContext = reinterpret_cast<WifiManagerContext*>(arg);

    if (event_base == WIFI_EVENT) {
        switch (event_id) {
            case WIFI_EVENT_AP_START:
                pWifiManagerContext->m_WifiAp.SetState(WifiAp::State::STARTED);
                break;
            case WIFI_EVENT_AP_STOP:
                pWifiManagerContext->m_WifiAp.SetState(WifiAp::State::STOPPED);
                break;
            case WIFI_EVENT_AP_STACONNECTED:
                sta_connected_coutner++;
                if (sta_connected_coutner > 0)
                {
                    pWifiManagerContext->m_WifiAp.SetState(WifiAp::State::CLIENTS_CONNECTED);
                }
                break;
            case WIFI_EVENT_AP_STADISCONNECTED:
                sta_connected_coutner--;
                if (sta_connected_coutner < 0)
                {
                    sta_connected_coutner = 0;
                }

                if (sta_connected_coutner == 0)
                {
                    pWifiManagerContext->m_WifiAp.SetState(WifiAp::State::STARTED);
                }
                break;
            case WIFI_EVENT_STA_CONNECTED:
                pWifiManagerContext->m_WifiSta.SetState(WifiSta::State::CONNECTED);
                break;
            case WIFI_EVENT_STA_START:
                pWifiManagerContext->m_WifiSta.SetState(WifiSta::State::CONNECTING);
                esp_wifi_connect();
                break;
            case WIFI_EVENT_STA_DISCONNECTED:
                if (pWifiManagerContext->m_PendingNewConfiguration)
                {
                    pWifiManagerContext->m_WifiSta.SetState(WifiSta::State::STOPPED);
                }
                else
                {
                    if (pWifiManagerContext->m_WifiSta.GetState() == WifiSta::State::CONNECTED)
                    {
                        pWifiManagerContext->m_WifiSta.SetState(WifiSta::State::DISCONNECTED);
                        esp_wifi_connect();
                    }
                }
                break;
        }
    }
    else if (event_base == IP_EVENT)
    {
        switch (event_id)
        {
            case IP_EVENT_STA_GOT_IP:
            {
                esp_netif_dns_info_t dnsInfo = pWifiManagerContext->m_WifiSta.GetDnsInfo();
                pWifiManagerContext->m_WifiAp.SetUpDnsOnDhcpServer(dnsInfo);
                pWifiManagerContext->m_WifiAp.EnableNat();
            }
            break;
            
            case IP_EVENT_STA_LOST_IP:
            {
                pWifiManagerContext->m_WifiAp.DisableNat();
            }
            break;
        };
    }

    if ( (event_base == IP_EVENT && event_id && ALLOWED_IP_EVENTS) ||
         (event_base == WIFI_EVENT && event_id && ALLOWED_WIFI_EVENTS) )
    {
        pWifiManagerContext->UpdateWifiManagerState();
    }
}

void WifiManager::WifiManagerContext::UpdateWifiManagerState()
{



}

}

}

}