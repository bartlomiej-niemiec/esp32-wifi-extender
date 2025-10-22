#include "wifi_sta.hpp"
#include "esp_wifi.h"
#include "assert.h"
#include <utility>
#include <string.h>
#include "esp_log.h"

namespace Hw
{

namespace Platform
{

namespace WifiExtender
{


WifiSta::WifiSta():
    m_sta_netif(nullptr),
    m_State(WifiSta::State::NOT_INITIALIZED)
{
};

WifiSta::~WifiSta()
{
    esp_netif_destroy_default_wifi(m_sta_netif);
}

bool WifiSta::Init()
{
    m_sta_netif = esp_netif_create_default_wifi_sta();
    assert(nullptr != m_sta_netif);
    return true;
}

bool WifiSta::SetConfig(const Hw::WifiExtender::StaConfig &sta_config)
{
    wifi_config_t sta_cfg = {};

    memcpy(sta_cfg.sta.password, sta_config.password.data(), sta_config.password.length());
    memcpy(sta_cfg.sta.ssid, sta_config.ssid.data(), sta_config.ssid.length());

    esp_err_t result = esp_wifi_set_config(WIFI_IF_STA, &sta_cfg);
    assert(ESP_OK == result);
    return true;
}

esp_netif_dns_info_t WifiSta::GetDnsInfo()
{
    esp_netif_dns_info_t dns;
    esp_netif_get_dns_info(m_sta_netif, ESP_NETIF_DNS_MAIN, &dns);
    return dns;
}

void WifiSta::SetState(WifiSta::State state)
{
    m_State = state;
}

void WifiSta::SetDefaultNetIf()
{
    ESP_ERROR_CHECK(esp_netif_set_default_netif(m_sta_netif));
}

WifiSta::State WifiSta::GetState() const
{
    return m_State;
}

esp_netif_ip_info_t WifiSta::GetIpInfo()
{
    esp_netif_ip_info_t ip_info;
    esp_netif_get_ip_info(m_sta_netif, &ip_info);
    return ip_info;
}

}

}

}

