#include "wifi_sta.hpp"
#include "esp_wifi.h"
#include "assert.h"
#include <utility>
#include <string.h>
#include "esp_log.h"

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

bool WifiSta::SetConfig(const StaConfig &sta_config)
{
    wifi_config_t sta_cfg = {};
    sta_cfg.sta.scan_method = WIFI_ALL_CHANNEL_SCAN;
    size_t ssid_size = strnlen(sta_config.ssid.data(), StaConfig::MAX_SSID_SIZE);
    if (ssid_size < StaConfig::MAX_SSID_SIZE) {
        memcpy(sta_cfg.sta.ssid, sta_config.ssid.data(), ssid_size + 1);
    } else {
        memcpy(sta_cfg.sta.ssid, sta_config.ssid.data(), StaConfig::MAX_SSID_SIZE);
        sta_cfg.sta.ssid[StaConfig::MAX_SSID_SIZE - 1] = '\0';
    }

    size_t password_size = strnlen(sta_config.password.data(), StaConfig::MAX_PASSWORD_SIZE);
    if (ssid_size < StaConfig::MAX_PASSWORD_SIZE) {
        memcpy(sta_cfg.sta.password, sta_config.password.data(), password_size + 1);
    } else {
        memcpy(sta_cfg.sta.ssid, sta_config.password.data(), StaConfig::MAX_PASSWORD_SIZE);
        sta_cfg.sta.password[StaConfig::MAX_SSID_SIZE - 1] = '\0';
    }

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

