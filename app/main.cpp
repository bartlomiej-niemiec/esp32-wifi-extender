#include "wifi_extender_if/wifi_extender_factory.hpp"
#include "wifi_extender_if/wifi_extender_if.hpp"
#include "nvs/nvs.hpp"

#include "esp_log.h"
#include "freertos/FreeRTOS.h"

#include "config.hpp"
#include <string>

extern "C" void app_main(void)
{
    using namespace Hw::Nvs;
    using namespace Hw::WifiExtender;

    const AccessPointConfig apConfig(
        static_cast<std::string>(DEFAULT_AP_SSID),
        static_cast<std::string>(DEFAULT_AP_PASSWORD)
    );

    const StaConfig staConfig;

    WifiExtenderConfig config(apConfig, staConfig);

    Nvs::Init();
    WifiExtenderFactory::WifiExtenderBoundle wifiExtenderBoundle = WifiExtenderFactory::GetWifiExtenderBoundle();
    WifiExtenderIf *pWifiExtenderIf = wifiExtenderBoundle.pWifiExtenderIf;
    pWifiExtenderIf->Startup(config);

    WifiExtenderState state;

    while (state != WifiExtenderState::RUNNING)
    {
        state = pWifiExtenderIf->GetState();
        ESP_LOGI("WifiExtender", "State: %s", WifiExtenderHelpers::WifiExtenderStaToString(state).data());
        vTaskDelay((2000 / portTICK_PERIOD_MS));
    }

    ESP_LOGI("WifiExtender", "Shutdown extender");

    const StaConfig newStaConfig(
        "TP-Link_777D",
        "58516386"
    );

    config.apConfig = apConfig;
    config.staConfig = newStaConfig;

    pWifiExtenderIf->UpdateConfig(config);

    while (true)
    {
        state = pWifiExtenderIf->GetState();
        ESP_LOGI("WifiExtender", "State: %s", WifiExtenderHelpers::WifiExtenderStaToString(state).data());
        vTaskDelay((1000 / portTICK_PERIOD_MS));
    }


}