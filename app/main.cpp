#include "wifi_extender_if/wifi_extender_factory.hpp"
#include "wifi_extender_if/wifi_extender_if.hpp"
#include "nvs/nvs.hpp"

#include "esp_log.h"
#include "freertos/FreeRTOS.h"

extern "C" void app_main(void)
{
    using namespace Hw::Nvs;
    using namespace Hw::WifiExtender;

    const AccessPointConfig apConfig;
    const StaConfig staConfig;

    Nvs::Init();
    WifiExtenderIf* pWifiExtender = WifiExtenderFactory::GetWifiExtenderIf();
    pWifiExtender->Init();
    pWifiExtender->Startup(apConfig, staConfig);
}