#include "wifi_scanner.hpp"

namespace WifiExtender
{

WifiScanner::WifiScanner():
    m_ScannedNetworks(),
    m_State(ScannerState::Idle)
{
}

bool WifiScanner::StartScanFor(const ScanOptions& opts)
{
    m_State = ScannerState::Scanning;
    wifi_scan_config_t scanConfig = {};
    wifi_scan_time_t scanTime = {};
    wifi_active_scan_time_t activeScanTime = {
        .min = 30,
        .max = 120
    };                     
    scanTime.active = activeScanTime;
    scanTime.passive = 100;

    scanConfig.scan_time = scanTime;
    scanConfig.scan_type = WIFI_SCAN_TYPE_ACTIVE;
    scanConfig.show_hidden = false;

    esp_err_t error = esp_wifi_scan_start(&scanConfig, false);
    return  error == ESP_OK;
}

bool WifiScanner::CancelScan(bool timerExpired)
{
    if (timerExpired)
    {
        m_State = ScannerState::Cancelled;
    }
    return esp_wifi_scan_stop() == ESP_OK;
}

void WifiScanner::ScanningCompleteSignal()
{
    uint16_t count = 0;
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&count));
    if (count == 0) { m_State = ScannerState::Done; return; }

    std::vector<wifi_ap_record_t> recs(count);
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&count, recs.data()));

    m_ScannedNetworks.clear();
    m_ScannedNetworks.reserve(count);
    for (auto& r : recs) {
        // ssid może nie być null-terminated — licz do pierwszego '\0' lub do 32
        size_t ssid_len = strnlen(reinterpret_cast<const char*>(r.ssid), sizeof(r.ssid));
        std::string ssid(reinterpret_cast<const char*>(r.ssid), ssid_len);
        std::array<uint8_t, 6> bssid{};
        std::copy(std::begin(r.bssid), std::end(r.bssid), bssid.begin());
        m_ScannedNetworks.emplace_back(
            WifiNetwork(
                r.ssid,
                sizeof(r.ssid),
                r.bssid,
                sizeof(r.bssid),
                r.rssi,
                0,
                ToAuthMode(r.authmode)
            )
        );
    }
    m_State = m_State == ScannerState::Cancelled ? ScannerState::Idle : ScannerState::Done;
}

void WifiScanner::CleanResults()
{
    m_ScannedNetworks.clear();
}

const std::vector<WifiNetwork> & WifiScanner::GetResults() const
{
    return m_ScannedNetworks;
}

ScannerState WifiScanner::GetScannerState() const
{
    return m_State;
}

AuthMode WifiScanner::ToAuthMode(wifi_auth_mode_t mode)
{
    switch (mode)
    {
        case WIFI_AUTH_OPEN: return AuthMode::Open;
        case WIFI_AUTH_WEP: return AuthMode::WEP;
        case WIFI_AUTH_WPA_PSK: return AuthMode::WPA_PSK;
        case WIFI_AUTH_WPA2_PSK: return AuthMode::WPA2_PSK;
        case WIFI_AUTH_WPA_WPA2_PSK: return AuthMode::WPA_WPA2_PSK;
        case WIFI_AUTH_WPA3_PSK: return AuthMode::WPA3_PSK;
        default: return AuthMode::Unknown;
    };
}

}
