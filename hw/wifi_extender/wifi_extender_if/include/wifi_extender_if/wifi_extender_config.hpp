#ifndef HW_WIFIEXTENDER_WIFIEXTENDERCONFIG_HPP
#define HW_WIFIEXTENDER_WIFIEXTENDERCONFIG_HPP

#include <stdint.h>
#include <string>

namespace Hw
{

namespace WifiExtender
{

struct AccessPointConfig {

    AccessPointConfig():
        ssid(),
        password(),
        max_clients(0){};

    std::string ssid;
    std::string password;
    const int max_clients;

    bool operator==(AccessPointConfig const& apconfig) const
    {
        return ((apconfig.ssid == this->ssid) && (apconfig.password == this->password));
    };

    bool operator!=(AccessPointConfig const& apconfig) const
    {   
        return !this->operator==(apconfig);
    };

};

struct StaConfig {

    StaConfig():
        ssid(),
        password(){};

    std::string ssid;
    std::string password;

    bool operator==(StaConfig const& staconfig) const
    {
        return ((staconfig.ssid == this->ssid) && (staconfig.password == this->password));
    };

    bool operator!=(StaConfig const& staconfig) const
    {   
        return !this->operator==(staconfig);
    };

};


enum class WifiExtenderEvent{
    INITIALIZED,
    RUNNING,
    STOPEED,
    STA_NOT_CONNECTED,
    STA_NOT_CONNECTED_WRONG_PARAMETERS
};


class EventListener
{
    public:

        virtual ~EventListener() = default;


        virtual void Callback(WifiExtenderEvent event) = 0;


};


}

}

#endif