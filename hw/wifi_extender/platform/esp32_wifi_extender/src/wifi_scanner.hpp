#ifndef WIFIEXTENDER_WIFISCANNER_HPP
#define WIFIEXTENDER_WIFISCANNER_HPP

#include "wifi_extender_if/wifi_extencer_scanner_types.hpp"

namespace WifiExtender
{

class WifiScanner
{
    public:

        WifiScanner();

        bool StartScanFor(const int & time_in_s,
                    const ScanOptions& opts = {});

        bool CancelScan();

        void CleanResults();

        bool AddApToResults(const WifiNetwork & network);

        void ScanningCompleteSignal();

        const std::vector<WifiNetwork> & GetResults() const;

        ScannerState GetScannerState() const;

    private:

        std::vector<WifiNetwork> m_ScannedNetworks;

        ScannerState m_State;
    
};

}


#endif