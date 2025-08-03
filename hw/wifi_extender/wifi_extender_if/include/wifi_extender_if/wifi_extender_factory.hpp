#ifndef HW_WIFIEXTENDER_WIFIEXTENDERFACTORY_HPP
#define HW_WIFIEXTENDER_WIFIEXTENDERFACTORY_HPP

#include <stdint.h>
#include <string>

namespace Hw
{

namespace WifiExtender
{

class WifiExtenderIf;

class WifiExtenderFactory{

public:

    static WifiExtenderIf* GetWifiExtenderIf();

};

}

}

#endif