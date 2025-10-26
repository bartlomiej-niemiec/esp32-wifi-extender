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
    wifi_ap_record_t ap_record = {};
    while (esp_wifi_scan_get_ap_record(&ap_record) == ESP_OK)
    {
        m_ScannedNetworks.emplace_back(
            WifiNetwork(
                ap_record.ssid,
                sizeof(ap_record.ssid),
                ap_record.bssid,
                sizeof(ap_record.bssid),
                ap_record.rssi,
                0,
                ToAuthMode(ap_record.authmode)
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
