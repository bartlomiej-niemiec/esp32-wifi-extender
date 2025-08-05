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

    m_WifiManagerContext.m_WifiManagerState = WifiExtenderState::STARTED;

    return true;
}


bool WifiManager::Shutdown()
{
    // TO DO
    ESP_LOGW("WIFI", "Shutdown() not implemented");
    return false;
}

bool WifiManager::UpdateConfig(const Hw::WifiExtender::AccessPointConfig &ap_config,
                                const Hw::WifiExtender::StaConfig &sta_config)
{
    // TO DO
    m_WifiManagerContext.m_PendingNewConfiguration = true;
    return true;
}

bool WifiManager::RegisterListener(EventListener * pEventListener)
{
    // TO DO
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
                ESP_ERROR_CHECK(esp_timer_stop(pWifiManagerContext->m_StaConnectionTimer));
                break;
            case WIFI_EVENT_STA_START:
                pWifiManagerContext->m_WifiSta.SetState(WifiSta::State::CONNECTING);
                if (esp_wifi_connect() == ESP_ERR_WIFI_SSID)
                {
                    pWifiManagerContext->m_WifiManagerState = WifiExtenderState::STA_CANNOT_CONNECT;
                }
                else
                {
                    ESP_LOGI("WifiExtender", "Starting timer..");
                    ESP_ERROR_CHECK(esp_timer_start_once(pWifiManagerContext->m_StaConnectionTimer, pWifiManagerContext->TIMER_EXPIRED_TIME_US));
                    esp_wifi_connect();
                }
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
                        ESP_ERROR_CHECK(esp_timer_start_once(pWifiManagerContext->m_StaConnectionTimer, pWifiManagerContext->TIMER_EXPIRED_TIME_US));
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
                ESP_LOGI("DNS_INFO", "Primary dns ip: %d.%d.%d.%d", esp_ip4_addr1(&(dnsInfo.ip.u_addr.ip4)),
                                            esp_ip4_addr2(&(dnsInfo.ip.u_addr.ip4)),
                                            esp_ip4_addr3(&(dnsInfo.ip.u_addr.ip4)),
                                            esp_ip4_addr4(&(dnsInfo.ip.u_addr.ip4)));
                esp_netif_ip_info_t ipInfo = pWifiManagerContext->m_WifiSta.GetIpInfo();

                if (ipInfo.gw.addr == dnsInfo.ip.u_addr.ip4.addr)
                {
                    ESP_LOGI("DNS_INFO", "Setting default primary dns to 8.8.8.8");
                    dnsInfo.ip.u_addr.ip4.addr = esp_netif_ip4_makeu32(8, 8, 8, 8);
                }
            
                pWifiManagerContext->m_WifiAp.SetUpDnsOnDhcpServer(dnsInfo);
                pWifiManagerContext->m_WifiSta.SetDefaultNetIf();
                 pWifiManagerContext->m_WifiAp.EnableNat();
                pWifiManagerContext->m_WifiSta.SetState(WifiSta::State::CONNECTED_AND_GOT_IP);
            }
            break;
            
            case IP_EVENT_STA_LOST_IP:
            {
                pWifiManagerContext->m_WifiAp.DisableNat();
                pWifiManagerContext->m_WifiSta.SetState(WifiSta::State::DISCONNECTED);
            }
            break;
        };
    }

    if ( (event_base == IP_EVENT && ((event_id & ALLOWED_IP_EVENTS) != 0)) ||
         (event_base == WIFI_EVENT && ((event_id & ALLOWED_WIFI_EVENTS) != 0) ) )
    {
        pWifiManagerContext->UpdateWifiManagerState();
    }
}

void WifiManager::WifiManagerContext::Init()
{
    // Call onlu once
    static bool initialized = false;
    if (false == initialized)
    {
        assert(esp_timer_init() != ESP_ERR_NO_MEM );

        m_timerArgs.callback = TimerCallback;
        m_timerArgs.arg = this;
        m_timerArgs.dispatch_method = ESP_TIMER_TASK;
        m_timerArgs.name = "STA_CONNECTION_TIMER";
        m_timerArgs.skip_unhandled_events = true;

        ESP_ERROR_CHECK(esp_timer_create(&m_timerArgs, &m_StaConnectionTimer));
        initialized = true;
    }
}

void WifiManager::WifiManagerContext::TimerCallback(void *arg)
{
    ESP_LOGI("WifiExtender", "Timer expired..");
    WifiManagerContext* pWifiManagerContext = reinterpret_cast<WifiManagerContext*>(arg);
    pWifiManagerContext->m_WifiManagerState = WifiExtenderState::STA_CANNOT_CONNECT;
    esp_wifi_connect();
    ESP_ERROR_CHECK(esp_timer_start_once(pWifiManagerContext->m_StaConnectionTimer, pWifiManagerContext->TIMER_EXPIRED_TIME_US));
}

void WifiManager::WifiManagerContext::UpdateWifiManagerState()
{

    WifiAp::State wifiApState = m_WifiAp.GetState();
    WifiSta::State wifiStaState = m_WifiSta.GetState();

    if ( (wifiStaState == WifiSta::State::CONNECTING) &&
        (wifiApState == WifiAp::State::STARTED) && (m_WifiManagerState != WifiExtenderState::STA_CANNOT_CONNECT))
    {
        m_WifiManagerState = WifiExtenderState::IN_PROGRESS;
        return;
    }

    if ((wifiApState == WifiAp::State::STARTED || wifiApState == WifiAp::State::CLIENTS_CONNECTED) &&
        ((wifiStaState == WifiSta::State::CONNECTED_AND_GOT_IP)))
    {
        m_WifiManagerState = WifiExtenderState::RUNNING;
        return;
    }

    if (((wifiStaState == WifiSta::State::CONNECTED)) && m_WifiManagerState == WifiExtenderState::STA_CANNOT_CONNECT)
    {
        if ((wifiApState == WifiAp::State::STARTED || wifiApState == WifiAp::State::CLIENTS_CONNECTED))
        {
             m_WifiManagerState = WifiExtenderState::RUNNING;
        }
        else
        {
            m_WifiManagerState = WifiExtenderState::IN_PROGRESS;
        }
        return;
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