/*  WiFi softAP Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "wifi_extender_if/wifi_extender_factory.hpp"
#include "wifi_extender_if/wifi_extender_if.hpp"
#include "nvs/nvs.hpp"

#include "esp_log.h"
#include "freertos/FreeRTOS.h"

extern "C" void app_main(void)
{
    using namespace Hw::Nvs;
    using namespace Hw::WifiExtender;

    const AccessPointConfig apConfig(
        "ESP32",
        "dupadupa1"
    );

    const StaConfig staConfig(
        "TP-Link_777D",
        "dsadasdasda"
    );

    Nvs::Init();
    WifiExtenderIf* pWifiExtender = WifiExtenderFactory::GetWifiExtenderIf();
    pWifiExtender->Init();
    pWifiExtender->Startup(apConfig, staConfig);

    constexpr TickType_t DELAY_5_S = 5000 / portTICK_PERIOD_MS;

    while (true)
    {
        vTaskDelay(DELAY_5_S);
        WifiExtenderState state = pWifiExtender->GetState();
        ESP_LOGI("Main", "%s",  WifiExtenderHelpers::WifiExtenderStaToString(state).data());
    }

}