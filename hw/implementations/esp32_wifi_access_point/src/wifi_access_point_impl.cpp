#include "wifi_access_point_impl.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include <string.h>

namespace Hw
{

namespace WifiAccessPoint
{

#define EXAMPLE_ESP_WIFI_SSID      "CONFIG_ESP_WIFI_SSID"
#define EXAMPLE_ESP_WIFI_PASS      "CONFIG_ESP_WIFI_PASSWORD"
#define EXAMPLE_ESP_WIFI_CHANNEL   1
#define EXAMPLE_MAX_STA_CONN       1

static const char *TAG = "wifi softAP";

static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                    int32_t event_id, void* event_data)
{
    if (event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        (void) event;
        // ESP_LOGI(TAG, "station "MACSTR" join, AID=%d",
        //          MAC2STR(event->mac), event->aid);
    } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        (void) event;
        // ESP_LOGI(TAG, "station "MACSTR" leave, AID=%d, reason=%d",
        //          MAC2STR(event->mac), event->aid, event->reason);
    }
}

void WifiAccessPointImpl::Init(const std::string_view &ssid, const std::string_view &password)
{
    memcpy(m_ssid, ssid.data(), MAX_SSID_LEN);
    memcpy(m_password, password.data(), MAX_PASSWORD_LEN);
    m_ssidlen = ssid.size();
    m_passwordlen = password.size();
};

void WifiAccessPointImpl::Startup()
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        NULL));

    wifi_config_t wifi_config = {
        .ap = {
            .channel = EXAMPLE_ESP_WIFI_CHANNEL,
#ifdef CONFIG_ESP_WIFI_SOFTAP_SAE_SUPPORT
            .authmode = WIFI_AUTH_WPA3_PSK,
#else /* CONFIG_ESP_WIFI_SOFTAP_SAE_SUPPORT */
            .authmode = WIFI_AUTH_WPA2_PSK,
#endif
            .max_connection = EXAMPLE_MAX_STA_CONN,
            .pmf_cfg = {
                    .required = true,
            },
#ifdef CONFIG_ESP_WIFI_SOFTAP_SAE_SUPPORT
            .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
#endif
        },
    };

    memcpy(wifi_config.ap.ssid, m_ssid, m_ssidlen);
    memcpy(wifi_config.ap.password, m_password, m_passwordlen);
    wifi_config.ap.ssid_len = m_ssidlen;

    if (m_passwordlen == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_softap finished. SSID:%s password:%s channel:%d",
             EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS, EXAMPLE_ESP_WIFI_CHANNEL);
};

}

}