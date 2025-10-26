#ifndef WIFIEXTENDER_WIFISCANNER_HPP
#define WIFIEXTENDER_WIFISCANNER_HPP

#include "wifi_extender_if/wifi_extencer_scanner_types.hpp"
#include "esp_wifi.h"

namespace WifiExtender
{

class WifiScanner
{
    public:

        WifiScanner();

        bool StartScanFor(const ScanOptions& opts = {});

        bool CancelScan(bool timerExpired);

        void ScanningCompleteSignal();

        const std::vector<WifiNetwork> & GetResults() const;

        ScannerState GetScannerState() const;

    private:

        std::vector<WifiNetwork> m_ScannedNetworks;

        ScannerState m_State;

        void CleanResults();

        AuthMode ToAuthMode(wifi_auth_mode_t mode);
    
};

}


#endif