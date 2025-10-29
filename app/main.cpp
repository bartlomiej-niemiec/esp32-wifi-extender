#include "freertos/FreeRTOS.h"

#include "wifi_extender_if/wifi_extender_factory.hpp"
#include "wifi_extender_if/wifi_extender_if.hpp"
#include "wifi_extender_if/wifi_extender_scanner_types.hpp"
#include "nvs/nvs.hpp"

#include "rgbled_if/rgbled_if.hpp"

#include "esp_log.h"

#include "config.hpp"
#include <string>

class LogEventListener:
    public WifiExtender::EventListener
{
    public:

        LogEventListener(RgbLed::RgbLedIf & led):
            m_Led(led){}

        void Callback(WifiExtender::WifiExtenderState event) override
        {
            ESP_LOGI("WifiExtender", "State: %s", WifiExtender::WifiExtenderHelpers::WifiExtenderStaToString(event).data());
            switch (event)
            {  
                case WifiExtender::WifiExtenderState::STOPPED:
                {
                    m_Led.SetColor(RgbLed::ColorCreator::CreateColor(RgbLed::ColorType::RED));
                }
                break;

                case WifiExtender::WifiExtenderState::STARTED:
                {
                    m_Led.SetColor(RgbLed::ColorCreator::CreateColor(RgbLed::ColorType::BLUE));
                }
                break;

                case WifiExtender::WifiExtenderState::CONNECTING:
                {
                    m_Led.SetColor(RgbLed::ColorCreator::CreateColor(RgbLed::ColorType::WHITE));
                }
                break;

                case WifiExtender::WifiExtenderState::RUNNING:
                {
                    m_Led.SetColor(RgbLed::ColorCreator::CreateColor(RgbLed::ColorType::GREEN));
                }
                break;

                case WifiExtender::WifiExtenderState::STOPPING:
                {
                    m_Led.SetColor(RgbLed::ColorCreator::CreateColor(RgbLed::ColorType::PINK));
                }
                break;

                case WifiExtender::WifiExtenderState::NEW_CONFIGURATION_PENDING:
                {
                    m_Led.SetColor(RgbLed::ColorCreator::CreateColor(RgbLed::ColorType::PURPLE));
                }
                break;

                case WifiExtender::WifiExtenderState::STA_CANNOT_CONNECT:
                {
                    m_Led.SetColor(RgbLed::ColorCreator::CreateColor(RgbLed::ColorType::YELLOW));
                }
                break;

                default:
                    break;
            }
        }

    private:
        RgbLed::RgbLedIf & m_Led;
        RgbLed::Color m_rgbColor;


};


extern "C" void app_main(void)
{
    using namespace Hw::Nvs;
    using namespace WifiExtender;

    RgbLed::RgbLedIf & rgbled = RgbLed::RgbLedFactory::GetRgbLed();

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
    static LogEventListener listener(rgbled);
    rWifiExtender.RegisterListener(&listener);
    rWifiExtender.Startup(config);
    while(true)
    {
        vTaskDelay(pdMS_TO_TICKS(1500));
    };
}