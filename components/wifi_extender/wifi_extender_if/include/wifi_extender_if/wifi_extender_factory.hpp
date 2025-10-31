#ifndef HW_WIFIEXTENDER_WIFIEXTENDERFACTORY_HPP
#define HW_WIFIEXTENDER_WIFIEXTENDERFACTORY_HPP

#include "wifi_extender_if.hpp"

namespace WifiExtender
{

class WifiExtenderFactory{

public:

    static WifiExtenderIf & GetWifiExtender();

};

}

#endif