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
    m_WifiManagerContext.Init();

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

    m_WifiManagerContext.m_WifiManagerState = WifiExtenderState::INITIALIZED;

    return true;
}


bool WifiManager::Startup(const Hw::WifiExtender::AccessPointConfig &ap_config,
                          const Hw::WifiExtender::StaConfig &sta_config)
{
    if (m_WifiManagerContext.m_WifiSta.GetState() == WifiSta::State::INITIALIZED || m_WifiManagerContext.m_WifiSta.GetState() == WifiSta::State::STOPPED )
    {
        assert(m_WifiManagerContext.m_WifiSta.SetConfig(sta_config) == true);
        m_WifiManagerContext.m_WifiSta.SetState(WifiSta::State::CONFIGURED);
    }

    if (m_WifiManagerContext.m_WifiAp.GetState() == WifiAp::State::INITIALIZED || m_WifiManagerContext.m_WifiAp.GetState() == WifiAp::State::STOPPED)
    {
        assert(m_WifiManagerContext.m_WifiAp.SetConfig(ap_config) == true);
        m_WifiManagerContext.m_WifiAp.SetState(WifiAp::State::CONFIGURED);
    }

    ESP_ERROR_CHECK(esp_wifi_start());

    m_WifiManagerContext.m_WifiManagerState = WifiExtenderState::STARTED;

    return true;
}


bool WifiManager::Shutdown()
{
    Deinit();
    return true;
}

bool WifiManager::UpdateConfig(const Hw::WifiExtender::AccessPointConfig &ap_config,
                                const Hw::WifiExtender::StaConfig &sta_config)
{
    m_WifiManagerContext.m_PendingNewConfiguration = true;

    ESP_LOGI("WifiManager", "ApNewConfig ssid=%s, password=%s", ap_config.ssid.data(), ap_config.password.data());
    ESP_LOGI("WifiManager", "StaNewConfig ssid=%s, password=%s", sta_config.ssid.data(), sta_config.password.data());
    m_WifiManagerContext.m_WifiAp.DisableNat();
    
    Deinit();

    while (m_WifiManagerContext.m_WifiAp.GetState() != WifiAp::State::STOPPED && 
          m_WifiManagerContext.m_WifiSta.GetState() != WifiSta::State::STOPPED)
    {
        vTaskDelay((500 / portTICK_PERIOD_MS));
    }

    Startup(ap_config, sta_config);

    m_WifiManagerContext.m_PendingNewConfiguration = false;

    ESP_LOGI("WifiManager", "Update completed");

    return true;
}

bool WifiManager::RegisterListener(EventListener * pEventListener)
{
    // TO DO
    return true;
}

void WifiManager::Deinit()
{
    esp_wifi_disconnect();
    esp_wifi_stop();
}

void WifiManager::wifi_ip_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{

    WifiManagerContext* pWifiManagerContext = reinterpret_cast<WifiManagerContext*>(arg);

    if (event_base == WIFI_EVENT) {
        switch (event_id) {
            case WIFI_EVENT_AP_START:
                pWifiManagerContext->OnApStart();
                break;
            case WIFI_EVENT_AP_STOP:
                pWifiManagerContext->OnApStop();
                break;
            case WIFI_EVENT_AP_STACONNECTED:
                pWifiManagerContext->OnApStaConnected();
                break;
            case WIFI_EVENT_AP_STADISCONNECTED:
                pWifiManagerContext->OnApStaDisconnected();
                break;
            case WIFI_EVENT_STA_CONNECTED:
                pWifiManagerContext->OnStaConnected();
                break;
            case WIFI_EVENT_STA_START:
                pWifiManagerContext->OnStaStart();
                break;
            case WIFI_EVENT_STA_STOP:
                pWifiManagerContext->OnStaStop();
                break;
            case WIFI_EVENT_STA_DISCONNECTED:
                pWifiManagerContext->OnStaDisconnected();
                break;
        }
    }
    else if (event_base == IP_EVENT)
    {
        switch (event_id)
        {
            case IP_EVENT_STA_GOT_IP:
            {
                pWifiManagerContext->OnStaGotIp();
            }
            break;
            
            case IP_EVENT_STA_LOST_IP:
            {
                pWifiManagerContext->OnStaLostIp();
            }
            break;
        };
    }

    if ( (event_base == IP_EVENT && ((event_id & pWifiManagerContext->ALLOWED_IP_EVENTS) != 0)) ||
         (event_base == WIFI_EVENT && ((event_id & pWifiManagerContext->ALLOWED_WIFI_EVENTS) != 0) ) )
    {
        pWifiManagerContext->UpdateWifiManagerState();
    }
}


WifiExtenderState WifiManager::GetState()
{
    m_WifiManagerContext.UpdateWifiManagerState();
    return m_WifiManagerContext.m_WifiManagerState;
}

}

}

}