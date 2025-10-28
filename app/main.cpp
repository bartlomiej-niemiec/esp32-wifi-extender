#include "freertos/FreeRTOS.h"

#include "wifi_extender_if/wifi_extender_factory.hpp"
#include "wifi_extender_if/wifi_extender_if.hpp"
#include "wifi_extender_if/wifi_extender_scanner_types.hpp"
#include "nvs/nvs.hpp"

#include "esp_log.h"

#include "config.hpp"
#include <string>

class LogEventListener:
    public WifiExtender::EventListener
{
    public:

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

    const StaConfig staConfig(
        static_cast<std::string>(DEFAULT_STA_SSID),
        static_cast<std::string>(DEFAULT_STA_PASSWORD)
    );

    WifiExtenderConfig config(apConfig, staConfig);
    Nvs::Init();
    WifiExtenderIf & rWifiExtender = WifiExtenderFactory::GetWifiExtender();
    WifiExtenderScannerIf * pScanner = rWifiExtender.GetScanner();
    pScanner->RegisterOnFinished([pScanner](){
        const std::vector<WifiNetwork> & networks = pScanner->GetResults();
        for (const WifiNetwork & n : networks)
        {
            printNetwork(n);
        }
    });
    assert(nullptr != pScanner);
    static LogEventListener listener;
    rWifiExtender.RegisterListener(&listener);
    rWifiExtender.Startup(config);

    while(true)
    {
        vTaskDelay(pdMS_TO_TICKS(2000));
    };
}