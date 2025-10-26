#include "freertos/FreeRTOS.h"

#include "wifi_extender_if/wifi_extender_factory.hpp"
#include "wifi_extender_if/wifi_extender_if.hpp"
#include "wifi_extender_if/wifi_extencer_scanner_types.hpp"
#include "nvs/nvs.hpp"

#include "esp_log.h"

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
    WifiExtenderScannerIf * pScanner = rWifiExtender.GetScanner();
    assert(nullptr != pScanner);
    static LogEventListener listener;
    rWifiExtender.RegisterListener(&listener);
    rWifiExtender.Startup(config);
    volatile bool scanningStarted = false;
    while (true)
    {
        if (scanningStarted == false)
        {
            if (rWifiExtender.GetState() == WifiExtenderState::RUNNING)
            {
                pScanner->ScanFor(10);
                scanningStarted = true;
            }
        }
        else
        {
            if (pScanner->GetCurrentState() == ScannerState::Done)
            {
                const std::vector<WifiNetwork> & networks = pScanner->GetResults();
                for (const WifiNetwork & n : networks)
                {
                    printNetwork(n);
                }
                break;
            }
        }
        ESP_LOGI("mian", "Scanner State: %s", getScannerStateString(pScanner->GetCurrentState()).data());
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    while (true)
    {
        ESP_LOGI("main", "....");
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}