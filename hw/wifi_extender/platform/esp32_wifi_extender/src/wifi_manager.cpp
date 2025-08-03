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

    m_WifiSta.Init();
    m_WifiAp.Init();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_ip_event_handler,
                                                        this,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &wifi_ip_event_handler,
                                                        this,
                                                        &instance_got_ip));

    return true;
}

void WifiManager::InitTask()
{       
    BaseType_t taskCreateResult = xTaskCreate(
            &WifiMonitoringTask,
            "WifiMonitoringTask",
            4096,
            NULL,
            10,
            &m_xTaskHandle);
    assert(taskCreateResult == pdPASS);
}

bool WifiManager::Startup(const Hw::WifiExtender::AccessPointConfig &ap_config,
                          const Hw::WifiExtender::StaConfig &sta_config)
{
    m_WifiSta.Start(sta_config);
    m_WifiAp.Start(ap_config);

    ESP_ERROR_CHECK(esp_wifi_start());

    return true;
}

void WifiMonitoringTask(void *arg)
{
    while (true)
    {



    };
}

bool WifiManager::Shutdown()
{
    assert(false);
    return false;
}

bool WifiManager::UpdateConfig(const Hw::WifiExtender::AccessPointConfig &ap_config,
                                const Hw::WifiExtender::StaConfig &sta_config)
{
    return true;
}

void WifiManager::wifi_ip_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT) {
        switch (event_id) {
            case WIFI_EVENT_STA_START:
                esp_wifi_connect();
                ESP_LOGI("WIFI", "STA start – próbuję połączyć");
                break;
            case WIFI_EVENT_STA_DISCONNECTED:
                ESP_LOGW("WIFI", "STA rozłączone – ponawiam połączenie");
                esp_wifi_connect();
                break;
        }
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI("WIFI", "STA połączone, IP: " IPSTR, IP2STR(&event->ip_info.ip));
        
    }
}

}

}

}