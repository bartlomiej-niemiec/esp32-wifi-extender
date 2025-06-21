#ifndef HW_WIFIAP_WIFIAPFACTORY_HPP
#define HW_WIFIAP_WIFIAPFACTORY_HPP

#include <stdint.h>
#include <string>

namespace Hw
{

namespace WifiAccessPoint
{

class WifiAccessPoint;

class WifiAccessPointFactory{

public:

    static WifiAccessPoint* GetWifiAP();

};

}

}

#endif