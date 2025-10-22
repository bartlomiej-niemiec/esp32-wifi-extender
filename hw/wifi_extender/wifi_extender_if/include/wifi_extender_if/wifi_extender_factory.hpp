#ifndef HW_WIFIEXTENDER_WIFIEXTENDERFACTORY_HPP
#define HW_WIFIEXTENDER_WIFIEXTENDERFACTORY_HPP

#include <stdint.h>
#include <string>

#include "wifi_extender_scanner_if.hpp"

namespace Hw
{

namespace WifiExtender
{

class WifiExtenderIf;

class WifiExtenderFactory{

public:

    struct WifiExtenderBoundle{
        WifiExtenderIf *pWifiExtenderIf;
        WifiExtenderScannerIf* pWifiExtenderScannerIf;
    };

    static WifiExtenderBoundle GetWifiExtenderBoundle();

};

}

}

#endif