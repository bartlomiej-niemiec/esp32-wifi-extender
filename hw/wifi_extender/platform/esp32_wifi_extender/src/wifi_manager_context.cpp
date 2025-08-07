#include "wifi_manager_context.hpp"
#include "esp_log.h"

namespace Hw
{

namespace Platform
{

namespace WifiExtender
{

void WifiManagerContext::Init()
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

void WifiManagerContext::TimerCallback(void *arg)
{
    ESP_LOGI("WifiExtender", "Timer expired..");
    WifiManagerContext* pWifiManagerContext = reinterpret_cast<WifiManagerContext*>(arg);
    pWifiManagerContext->m_WifiManagerState = WifiExtenderState::STA_CANNOT_CONNECT;
    esp_wifi_connect();
    ESP_ERROR_CHECK(esp_timer_start_once(pWifiManagerContext->m_StaConnectionTimer, pWifiManagerContext->TIMER_EXPIRED_TIME_US));
}

void WifiManagerContext::UpdateWifiManagerState()
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

    if ((wifiStaState == WifiSta::State::STOPPED) &&
        (wifiApState == WifiAp::State::STOPPED))
    {
        m_WifiManagerState = WifiExtenderState::STOPEED;
        return;
    }
}


void WifiManagerContext::OnApStart()
{
    m_WifiAp.SetState(WifiAp::State::STARTED);
    ESP_LOGI("WifiAp", "ApStartEvent");
}

void WifiManagerContext::OnApStop()
{
    m_WifiAp.SetState(WifiAp::State::STOPPED);
    m_ApClientsCounter = 0;
    ESP_LOGI("WifiAp", "ApStoptEvent");
}

void WifiManagerContext::OnApStaConnected()
{
    m_ApClientsCounter++;
    m_WifiAp.SetState(WifiAp::State::CLIENTS_CONNECTED);
}

void WifiManagerContext::OnApStaDisconnected()
{
    m_ApClientsCounter--;
    if (m_ApClientsCounter < 0)
    {
        m_ApClientsCounter = 0;
    }

    if (m_ApClientsCounter == 0 && !m_PendingNewConfiguration)
    {
        m_WifiAp.SetState(WifiAp::State::STARTED);
    }
}

void WifiManagerContext::OnStaConnected()
{
    m_WifiSta.SetState(WifiSta::State::CONNECTED);
    ESP_ERROR_CHECK(esp_timer_stop(m_StaConnectionTimer));
}

void WifiManagerContext::OnStaDisconnected()
{   
    if (m_PendingNewConfiguration)
    {
        m_WifiSta.SetState(WifiSta::State::STOPPED);
    }
    else
    {
        if (m_WifiSta.GetState() == WifiSta::State::CONNECTED)
        {
            m_WifiSta.SetState(WifiSta::State::DISCONNECTED);
            ESP_ERROR_CHECK(esp_timer_start_once(m_StaConnectionTimer, TIMER_EXPIRED_TIME_US));
            esp_wifi_connect();
        }
    }
}

void WifiManagerContext::OnStaStart()
{
    ESP_LOGI("WifiAp", "StaStartEvent");
    m_WifiSta.SetState(WifiSta::State::CONNECTING);
    esp_err_t wifi_error = esp_wifi_connect();
    if (wifi_error == ESP_ERR_WIFI_SSID)
    {
        m_WifiManagerState = WifiExtenderState::STA_CANNOT_CONNECT;
    }
    else
    {
        ESP_LOGI("WifiExtender", "Starting timer..");
        ESP_ERROR_CHECK(esp_timer_start_once(m_StaConnectionTimer, TIMER_EXPIRED_TIME_US));
    }
}

void WifiManagerContext::OnStaStop()
{
    ESP_LOGI("WifiAp", "StaStopEvent");
    m_WifiSta.SetState(WifiSta::State::STOPPED);
    ESP_ERROR_CHECK(esp_timer_stop(m_StaConnectionTimer));
}

void WifiManagerContext::OnStaGotIp()
{
    esp_netif_dns_info_t dnsInfo = m_WifiSta.GetDnsInfo();
    ESP_LOGI("DNS_INFO", "Primary dns ip: %d.%d.%d.%d", esp_ip4_addr1(&(dnsInfo.ip.u_addr.ip4)),
                                            esp_ip4_addr2(&(dnsInfo.ip.u_addr.ip4)),
                                            esp_ip4_addr3(&(dnsInfo.ip.u_addr.ip4)),
                                            esp_ip4_addr4(&(dnsInfo.ip.u_addr.ip4)));
    esp_netif_ip_info_t ipInfo = m_WifiSta.GetIpInfo();

    if (ipInfo.gw.addr == dnsInfo.ip.u_addr.ip4.addr)
    {
        ESP_LOGI("DNS_INFO", "Setting default primary dns to 8.8.8.8");
        dnsInfo.ip.u_addr.ip4.addr = esp_netif_ip4_makeu32(8, 8, 8, 8);
    }
            
    m_WifiAp.SetUpDnsOnDhcpServer(dnsInfo);
    m_WifiSta.SetDefaultNetIf();
    m_WifiAp.EnableNat();
    m_WifiSta.SetState(WifiSta::State::CONNECTED_AND_GOT_IP);
}

void WifiManagerContext::OnStaLostIp()
{
    ESP_LOGI("WifiAp", "StaLostIpEvent");
    m_WifiAp.DisableNat();
    if (m_WifiSta.GetState() != WifiSta::State::STOPPED)
    {
        m_WifiSta.SetState(WifiSta::State::DISCONNECTED);
    }
}


}


}


}