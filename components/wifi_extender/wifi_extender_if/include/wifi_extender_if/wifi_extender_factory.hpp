#pragma once

#include "wifi_extender_if.hpp"

namespace WifiExtender
{

class WifiExtenderFactory{

public:

    static const WifiExtenderFactory & GetInstance();

    WifiExtenderIf & GetWifiExtender() const;

private:

    WifiExtenderFactory();

};

}