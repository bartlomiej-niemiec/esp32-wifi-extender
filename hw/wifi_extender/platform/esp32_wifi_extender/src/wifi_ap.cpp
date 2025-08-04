#include "wifi_ap.hpp"
#include "assert.h"
#include <utility>
#include <string.h>

#include "lwip/inet.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"
#include "lwip/ip_addr.h"
#include "lwip/netif.h"
#include "lwip/lwip_napt.h"

#include "esp_wifi.h"
#include "esp_netif_net_stack.h"
#include "esp_netif.h"

namespace Hw
{

namespace Platform
{

namespace WifiExtender
{

WifiAp::WifiAp():
    m_ap_netif(nullptr),
    m_State(WifiAp::State::NOT_INITIALIZED)
{
};

bool WifiAp::Init()
{
    m_ap_netif = esp_netif_create_default_wifi_ap();
    assert(nullptr != m_ap_netif);
    return true;
}


bool WifiAp::SetConfig(const Hw::WifiExtender::AccessPointConfig &ap_config)
{
    const uint8_t ssid_len = ap_config.ssid.length();

    wifi_config_t ap_cfg = {
        .ap = {
            .ssid_len = ssid_len,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK,
            .max_connection = 4,
        },
    };

    memcpy(ap_cfg.ap.password, ap_config.password.data(), ap_config.password.length());
    memcpy(ap_cfg.ap.ssid, ap_config.ssid.data(), ap_config.ssid.length());

    esp_err_t result = esp_wifi_set_config(WIFI_IF_AP, &ap_cfg);
    assert(ESP_OK == result);
    return true;
}

bool WifiAp::EnableNat()
{
    return esp_netif_napt_enable(m_ap_netif) == ESP_OK ? true : false;
}

bool WifiAp::DisableNat()
{
    return esp_netif_napt_disable(m_ap_netif) == ESP_OK ? true : false;
}

void WifiAp::SetUpDnsOnDhcpServer(esp_netif_dns_info_t  dnsInfo)
{
    uint8_t dhcps_offer_option = DHCPS_OFFER_DNS;
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_netif_dhcps_stop(m_ap_netif));
    ESP_ERROR_CHECK(esp_netif_dhcps_option(m_ap_netif, ESP_NETIF_OP_SET, ESP_NETIF_DOMAIN_NAME_SERVER, &dhcps_offer_option, sizeof(dhcps_offer_option)));
    ESP_ERROR_CHECK(esp_netif_set_dns_info(m_ap_netif, ESP_NETIF_DNS_MAIN, &dnsInfo));
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_netif_dhcps_start(m_ap_netif));
}

void WifiAp::SetState(WifiAp::State state)
{
    m_State = state;
}

WifiAp::State WifiAp::GetState()
{
    return m_State;
}


}

}

}

