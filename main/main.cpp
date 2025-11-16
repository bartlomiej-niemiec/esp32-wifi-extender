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
        static_cast<std::string>(DEFAULT_AP_SSID),
        static_cast<std::string>(DEFAULT_AP_PASSWORD)
    );

    const StaConfig staConfig(
        static_cast<std::string>(DEFAULT_STA_SSID),
        static_cast<std::string>(DEFAULT_STA_PASSWORD)
    );

    DataStorage::DataStorer & dataStorer = DataStorage::DataStorer::GetInstance();
    std::string_view ap_conifg_key = "apconfig";
    DataStorage::DataEntry<int> apConfigEntry = dataStorer.GetDataEntry<int>(ap_conifg_key);

    auto printApConfig = [](int x){
        ESP_LOGI("NvsApConfig", "x: %i", x);
    };

    int nvsApConfig{};
    apConfigEntry.Remove();
    DataStorage::DataRawStorerIf::ReadStatus status = apConfigEntry.GetData(nvsApConfig);
    if (status == DataStorage::DataRawStorerIf::ReadStatus::NOT_FOUND)
    {
        ESP_LOGI("NvsApConfig", "NO DATA FOUND");
        int a = 10;
        apConfigEntry.SetData(a);
        apConfigEntry.GetData(nvsApConfig);
    }
    else if (status == DataStorage::DataRawStorerIf::ReadStatus::NOK)
    {
        ESP_LOGI("NvsApConfig", "NOK");
    }

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