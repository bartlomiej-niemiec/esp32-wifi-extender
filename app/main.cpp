#include "wifi_extender_if/wifi_extender_factory.hpp"
#include "wifi_extender_if/wifi_extender_if.hpp"
#include "nvs/nvs.hpp"

#include "esp_log.h"
#include "freertos/FreeRTOS.h"

#include "config.hpp"
#include <string>

class LogEventListener:
    public Hw::WifiExtender::EventListener
{
    void Callback(Hw::WifiExtender::WifiExtenderState event) override
    {
        ESP_LOGI("WifiExtender", "State: %s", Hw::WifiExtender::WifiExtenderHelpers::WifiExtenderStaToString(event).data());
    }
};


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
    static LogEventListener listener;
    pWifiExtenderIf->RegisterListener(&listener);
    pWifiExtenderIf->Startup(config);
}