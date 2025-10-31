#ifndef HW_WIFIEXTENDER_WIFIEXTENDERSCANNER_HPP
#define HW_WIFIEXTENDER_WIFIEXTENDERSCANNER_HPP

#include <stdint.h>
#include <string>
#include <vector>

#include "wifi_extender_scanner_types.hpp"

namespace WifiExtender
{

class WifiExtenderScannerIf{

public:

    virtual ~WifiExtenderScannerIf() = default;

    virtual bool Scan(const ScanOptions& opts = {}) = 0;

    virtual bool CancelScan() = 0;

    virtual ScannerState GetCurrentState() = 0;

    virtual const std::vector<WifiNetwork> & GetResults() const = 0;   
    
    virtual void RegisterOnFinished(ScanFinishedCallback cb) = 0;

};
 
}

#endif