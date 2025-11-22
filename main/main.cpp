#include "freertos/FreeRTOS.h"

#include "wifi_extender_if/wifi_extender_config.hpp"
#include "wifi_extender_if/wifi_extender_factory.hpp"
#include "wifi_extender_if/wifi_extender_if.hpp"
#include "wifi_extender_if/wifi_extender_scanner_types.hpp"

#include "rgbled_if/rgbled_if.hpp"
#include "rgbled_if/rgbled_factory.hpp"
#include "rgbled_if/rgbled_utils.hpp"

#include "data_storer_if/data_storer.hpp"

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
    using namespace WifiExtender;
    
    DataStorage::DataStorer::Init();

    const AccessPointConfig apConfig(
        DEFAULT_AP_SSID,
        DEFAULT_AP_PASSWORD
    );

    const StaConfig staConfig(
        DEFAULT_STA_SSID,
        DEFAULT_STA_PASSWORD
    );

    auto printApConfig = [](AccessPointConfig ap){
        ESP_LOGI("NvsApConfig", "ssid: %s", ap.ssid.data());
        ESP_LOGI("NvsApConfig", "password: %s", ap.password.data());
    };

    DataStorage::DataStorer & dataStorer = DataStorage::DataStorer::GetInstance();
    std::string_view ap_conifg_key = "apconfig";
    AccessPointConfig nvsApConfig{};
    DataStorage::DataEntry<AccessPointConfig> apConfigEntry = dataStorer.GetDataEntry<AccessPointConfig>(ap_conifg_key);
    DataStorage::DataRawStorerIf::ReadStatus status = apConfigEntry.GetData(nvsApConfig);
    if (status == DataStorage::DataRawStorerIf::ReadStatus::NOK)
    {
        apConfigEntry.Remove();
    }

    apConfigEntry.SetData(apConfig);
    apConfigEntry.GetData(nvsApConfig);

    printApConfig(nvsApConfig);

    WifiExtenderConfig config(apConfig, staConfig);
    WifiExtenderIf & rWifiExtender = WifiExtenderFactory::GetInstance().GetWifiExtender();
    WifiExtenderScannerIf * pScanner = rWifiExtender.GetScanner();
    pScanner->RegisterStateListener([pScanner](ScannerState state){
        if (state == ScannerState::Done)
        {
            const std::vector<WifiNetwork> & networks = pScanner->GetResults();
            for (const WifiNetwork & n : networks)
            {
                printNetwork(n);
            }
        }
    });
    assert(nullptr != pScanner);
    static LogEventListener listener;
    rWifiExtender.RegisterListener(&listener);
    rWifiExtender.Startup(config);
    while(true)
    {
        vTaskDelay(pdMS_TO_TICKS(1500));
    };
}