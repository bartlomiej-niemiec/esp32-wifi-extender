#ifndef HW_WIFIEXTENDER_WIFIEXTENDERCONFIG_HPP
#define HW_WIFIEXTENDER_WIFIEXTENDERCONFIG_HPP

#include <stdint.h>
#include <string>
#include <string_view>

namespace WifiExtender
{

struct AccessPointConfig {

    AccessPointConfig(std::string ssid, std::string password, int max_clients = 1):
        ssid(std::move(ssid)), password(std::move(password)), max_clients(max_clients) {}
    
    AccessPointConfig():
        ssid(),
        password(),
        max_clients() {}

    AccessPointConfig(const AccessPointConfig & config) = default;
    AccessPointConfig(AccessPointConfig & config) = default;
    AccessPointConfig & operator=(AccessPointConfig & apconfig) = default;
    AccessPointConfig & operator=(const AccessPointConfig & apconfig) = default;

    bool operator==(AccessPointConfig const& apconfig) const
    {
        return ((apconfig.ssid == this->ssid) && (apconfig.password == this->password));
    };

    bool operator!=(AccessPointConfig const& apconfig) const
    {   
        return !this->operator==(apconfig);
    };

    bool IsValid() const {
        return !ssid.empty() && !password.empty();
    }

    std::string ssid;
    std::string password;
    int max_clients;

};

struct StaConfig {

    std::string ssid;
    std::string password;

    StaConfig(std::string ssid, std::string password):
        ssid(std::move(ssid)), password(std::move(password)) {}

    StaConfig():
        ssid(),
        password() {}

    StaConfig(StaConfig & config) = default;
    StaConfig(const StaConfig & config) = default;
    StaConfig & operator=(StaConfig & staconfig) = default;
    StaConfig & operator=(const StaConfig & staconfig) = default;

    bool operator==(StaConfig const& staconfig) const
    {
        return ((staconfig.ssid == this->ssid) && (staconfig.password == this->password));
    };

    bool operator!=(StaConfig const& staconfig) const
    {   
        return !this->operator==(staconfig);
    };

    bool IsValid() const {
        return !ssid.empty() && !password.empty();
    }

};

struct WifiExtenderConfig {
    
    AccessPointConfig apConfig;
    StaConfig         staConfig;

    WifiExtenderConfig(const AccessPointConfig& ap, const StaConfig& sta)
    : apConfig(ap), staConfig(sta) {}

    WifiExtenderConfig():
       apConfig(),
       staConfig() {}

    WifiExtenderConfig(const WifiExtenderConfig&)            = default;
    WifiExtenderConfig(WifiExtenderConfig&&)                 = default;
    WifiExtenderConfig& operator=(const WifiExtenderConfig&) = default;
    WifiExtenderConfig& operator=(WifiExtenderConfig&&)      = default;

    bool operator==(const WifiExtenderConfig& other) const {
        return apConfig == other.apConfig && staConfig == other.staConfig;
    }
    bool operator!=(const WifiExtenderConfig& other) const {
        return !(*this == other);
    }
};

enum class WifiExtenderState : uint8_t {
    STOPPED = 0,
    STARTED,
    CONNECTING,
    RUNNING,
    STOPPING,
    NEW_CONFIGURATION_PENDING,
    STA_CANNOT_CONNECT
};

class WifiExtenderHelpers
{

public:

static const std::string_view WifiExtenderStaToString(const WifiExtenderState & state)
{
    switch(state)
    {
        case WifiExtenderState::STARTED:
        {
            return "WifiExtender started";
        }
        break;

        case WifiExtenderState::CONNECTING:
        {
            return "WifiExtender connecting";
        }
        break;

        case WifiExtenderState::RUNNING:
        {
            return "WifiExtender running";
        }
        break;

        case WifiExtenderState::STOPPED:
        {
            return "WifiExtender stopped";
        }
        break;

        case WifiExtenderState::STOPPING:
        {
            return "WifiExtender stopping";
        }
        break; 

        case WifiExtenderState::STA_CANNOT_CONNECT:
        {
            return "WifiExtender STA cannot connect";
        }
        break;

        case WifiExtenderState::NEW_CONFIGURATION_PENDING:
        {
            return "WifiExtender new configuration pending...";
        }
        break;
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

#endif