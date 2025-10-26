#include "wifi_scanner.hpp"
#include "esp_wifi.h"

namespace WifiExtender
{

WifiScanner::WifiScanner():
    m_ScannedNetworks(),
    m_State(ScannerState::Idle)
{
}

bool WifiScanner::StartScanFor(const int & time_in_s,
                    const ScanOptions& opts)
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

bool WifiScanner::CancelScan()
{
    m_State = ScannerState::Idle;

    return false;
}

void WifiScanner::ScanningCompleteSignal()
{
    m_State = ScannerState::HaveResults;
}

void WifiScanner::CleanResults()
{
    esp_wifi_clear_ap_list();
    m_ScannedNetworks.clear();
}

bool WifiScanner::AddApToResults(const WifiNetwork & network)
{
    return false;
}

const std::vector<WifiNetwork> & WifiScanner::GetResults() const
{
    return m_ScannedNetworks;
}

ScannerState WifiScanner::GetScannerState() const
{
    return m_State;
}

}
