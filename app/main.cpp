#include "wifi_extender_if/wifi_extender_factory.hpp"
#include "wifi_extender_if/wifi_extender_if.hpp"
#include "nvs/nvs.hpp"

#include "esp_log.h"
#include "freertos/FreeRTOS.h"

#include "config.hpp"
#include <string>

class LogEventListener:
    public WifiExtender::EventListener
{
    void Callback(WifiExtender::WifiExtenderState event) override
    {
        ESP_LOGI("WifiExtender", "State: %s", WifiExtender::WifiExtenderHelpers::WifiExtenderStaToString(event).data());
    }
};


extern "C" void app_main(void)
{
    using namespace Hw::Nvs;
    using namespace WifiExtender;

    const AccessPointConfig apConfig(
        static_cast<std::string>(DEFAULT_AP_SSID),
        static_cast<std::string>(DEFAULT_AP_PASSWORD)
    );

    const StaConfig staConfig;
    WifiExtenderConfig config(apConfig, staConfig);
    Nvs::Init();
    WifiExtenderIf & rWifiExtender = WifiExtenderFactory::GetWifiExtender();
    static LogEventListener listener;
    rWifiExtender.RegisterListener(&listener);
    rWifiExtender.Startup(config);
}