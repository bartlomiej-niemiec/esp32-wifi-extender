#ifndef WIFIEXTENDER_WIFISCANNER_HPP
#define WIFIEXTENDER_WIFISCANNER_HPP

#include "wifi_extender_if/wifi_extender_scanner_types.hpp"
#include "esp_wifi.h"

namespace WifiExtender
{

class WifiScanner
{
    public:

        WifiScanner();

        bool StartScanFor(const ScanOptions& opts = {});

        bool CancelScan();

        void CleanResults();

        void ScanningCompleteSignal();

        const std::vector<WifiNetwork> & GetResults() const;

        ScannerState GetScannerState() const;

    private:

        std::vector<WifiNetwork> m_ScannedNetworks;

        ScannerState m_State;

        AuthMode ToAuthMode(wifi_auth_mode_t mode);

        esp_err_t StartPassiveScan(const ScanOptions& opts);

        esp_err_t StartActiveScan(const ScanOptions& opts);

        static constexpr uint32_t SCAN_TIME_PER_CHANNEL_PASSIVE_MS = 300;
    
};

}


#endif