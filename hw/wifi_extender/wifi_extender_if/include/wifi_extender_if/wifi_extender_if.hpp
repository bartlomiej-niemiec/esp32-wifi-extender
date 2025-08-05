#ifndef HW_WIFIEXTENDER_WIFIEXTENDER_HPP
#define HW_WIFIEXTENDER_WIFIEXTENDER_HPP

#include <stdint.h>
#include <string>
#include "wifi_extender_config.hpp"

namespace Hw
{

namespace WifiExtender
{

class WifiExtenderIf{

public:

    ~WifiExtenderIf() = default;

    virtual bool Init() = 0;

    virtual bool Startup(const AccessPointConfig &ap_config,
                         const StaConfig &sta_config) = 0;

    virtual bool RegisterListener(EventListener * pEventListener) = 0;

    virtual WifiExtenderState GetState() = 0;

    virtual bool Shutdown() = 0;

    virtual bool UpdateConfig(const AccessPointConfig &ap_config,
                              const StaConfig &sta_config) = 0;

};
 
}

}

#endif