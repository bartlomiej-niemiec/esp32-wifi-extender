#include "freertos/FreeRTOS.h"

#include "wifi_extender_if/wifi_extender_config.hpp"
#include "wifi_extender_if/wifi_extender_factory.hpp"
#include "wifi_extender_if/wifi_extender_if.hpp"
#include "wifi_extender_if/wifi_extender_scanner_types.hpp"

#include "rgbled_if/rgbled_if.hpp"
#include "rgbled_if/rgbled_factory.hpp"
#include "rgbled_if/rgbled_utils.hpp"

#include "data_storer_if/data_storer.hpp"

#include "mongoose.h"

#include "esp_log.h"

#include "config.hpp"
#include <string>


static volatile bool isMongooseRunning = false;
static constexpr char m_pTaskName[] = "MongooseWebServer";
static constexpr int MONGOOSE_TASK_STACK_SIZE = 4096 * 2;
static constexpr int MONGOOSE_TASK_PRIO = 3;
static TaskHandle_t m_MongooseTaskHandle = nullptr;

static void event_handler(struct mg_connection *c, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (mg_http_message *) ev_data;  // Parsed HTTP request
        ESP_LOGI("MONGOOSE", "HTTP %.*s %.*s",
                (int)hm->method.len, hm->method.buf,
                (int)hm->uri.len, hm->uri.buf);
        mg_http_reply(c, 200, "", "ok, uptime: %llu\r\n", mg_millis());
    }
}

static void run_mongoose(void *pArg) {
    ESP_LOGI("MONGOOSE", "run_mongoose started");
    struct mg_mgr mgr;        // Mongoose event manager
    mg_mgr_init(&mgr);        // Initialise event manager
    struct mg_connection *lc = mg_http_listen(&mgr, "http://0.0.0.0:80", event_handler, NULL);

        if (lc == nullptr) {
        ESP_LOGE("MONGOOSE", "mg_http_listen failed");
        mg_mgr_free(&mgr);
        vTaskDelete(NULL);
    }
    uint32_t counter = 0;
    mg_log_set(MG_LL_DEBUG);  // Set log level to debug
    for (;;) {                // Infinite event loop
        mg_mgr_poll(&mgr, 1000);   // Process network events
    }
}

class LogEventListener:
    public WifiExtender::EventListener
{
    public:

        void Callback(WifiExtender::WifiExtenderState event) override
        {
            ESP_LOGI("WifiExtender", "State: %s", WifiExtender::WifiExtenderHelpers::WifiExtenderStaToString(event).data());
            if ((event == WifiExtender::WifiExtenderState::CONNECTING ||
                event == WifiExtender::WifiExtenderState::RUNNING) && !isMongooseRunning)
            {
                xTaskCreate(
                    run_mongoose,
                    m_pTaskName,
                    MONGOOSE_TASK_STACK_SIZE,
                    nullptr,
                    MONGOOSE_TASK_PRIO,
                    &m_MongooseTaskHandle
                );
                assert(nullptr != m_MongooseTaskHandle);
                isMongooseRunning = true;
            }
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