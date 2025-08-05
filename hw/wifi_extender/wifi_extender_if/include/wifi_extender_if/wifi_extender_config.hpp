#ifndef HW_WIFIEXTENDER_WIFIEXTENDERCONFIG_HPP
#define HW_WIFIEXTENDER_WIFIEXTENDERCONFIG_HPP

#include <stdint.h>
#include <string>
#include <string_view>

namespace Hw
{

namespace WifiExtender
{

struct AccessPointConfig {

    AccessPointConfig():
        ssid(),
        password(),
        max_clients(0){};

    AccessPointConfig(std::string ssid, std::string password, int max_clients = 1):
        ssid(ssid),
        password(password),
        max_clients(max_clients)
        {};

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

    void operator=(AccessPointConfig const& apconfig)
    {
        this->ssid = apconfig.ssid;
        this->password = apconfig.password;
    }

};

struct StaConfig {

    StaConfig():
        ssid(),
        password(){};

    StaConfig(std::string ssid, std::string password):
        ssid(ssid),
        password(password){};

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

    void operator=(StaConfig const& staconfig)
    {
        this->ssid = staconfig.ssid;
        this->password = staconfig.password;
    }

};


enum class WifiExtenderState{
    UNINTIALIZED,
    INITIALIZED,
    STARTED,
    IN_PROGRESS,
    RUNNING,
    STOPEED,
    STA_CANNOT_CONNECT
};

class WifiExtenderHelpers
{

public:

static const std::string_view WifiExtenderStaToString(WifiExtenderState & state)
{
    switch(state)
    {
        case WifiExtenderState::UNINTIALIZED:
        {
            return "WifiExtender not initialized";
        }
        case WifiExtenderState::INITIALIZED:
        {
            return "WifiExtender initialized";
        }
        case WifiExtenderState::STARTED:
        {
            return "WifiExtender started";
        }
        case WifiExtenderState::IN_PROGRESS:
        {
            return "WifiExtender in progres...";
        }
        case WifiExtenderState::RUNNING:
        {
            return "WifiExtender running";
        }
        case WifiExtenderState::STOPEED:
        {
            return "WifiExtender stopped";
        }
        case WifiExtenderState::STA_CANNOT_CONNECT:
        {
            return "WifiExtender STA cannot connect";
        }
    };
    
    return "WifiExtender unknown state";
}


};


class EventListener
{
    public:

        virtual ~EventListener() = default;


        virtual void Callback(WifiExtenderState event) = 0;


};


}

}

#endif