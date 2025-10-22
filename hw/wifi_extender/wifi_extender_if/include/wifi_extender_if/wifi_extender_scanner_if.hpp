#ifndef HW_WIFIEXTENDER_WIFIEXTENDERSCANNER_HPP
#define HW_WIFIEXTENDER_WIFIEXTENDERSCANNER_HPP

#include <stdint.h>
#include <string>
#include <vector>

#include "wifi_extencer_scanner_types.hpp"

namespace Hw
{

namespace WifiExtender
{

class WifiExtenderScannerIf{

public:

    virtual ~WifiExtenderScannerIf() = default;

    virtual bool ScanFor(const int & time_in_s,
                        const ScanOptions& opts = {}) = 0;

    virtual bool CancelScan() = 0;

    virtual ScannerState GetCurrentState() = 0;

    virtual std::vector<WifiNetwork> GetResults() const = 0;

    virtual void OnNetworkFound(WifiNetworkCallback cb) = 0;      
    
    virtual void OnFinished(ScanFinishedCallback cb) = 0;

};
 
}

}

#endif