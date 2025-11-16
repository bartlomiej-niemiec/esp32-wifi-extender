#pragma once

#include <stdint.h>
#include <string>
#include <string_view>

namespace WifiExtender
{

enum class WifiExtenderState : uint8_t {
    STOPPED = 0,
    STARTED,
    CONNECTING,
    RUNNING,
    STOPPING,
    NEW_CONFIGURATION_PENDING,
    STA_CANNOT_CONNECT
};

struct AccessPointConfig {
    AccessPointConfig();
    AccessPointConfig(std::string ssid, std::string password, int max_clients = 1);
    AccessPointConfig(const AccessPointConfig & config) = default;
    AccessPointConfig(AccessPointConfig & config) = default;
    AccessPointConfig & operator=(AccessPointConfig & apconfig) = default;
    AccessPointConfig & operator=(const AccessPointConfig & apconfig) = default;

    bool operator==(AccessPointConfig const& apconfig) const;

    bool operator!=(AccessPointConfig const& apconfig) const;

    bool IsValid() const;

    std::string ssid;
    std::string password;
    int max_clients;

};

struct StaConfig {

    std::string ssid;
    std::string password;

    StaConfig();
    StaConfig(std::string ssid, std::string password);
    StaConfig(StaConfig & config) = default;
    StaConfig(const StaConfig & config) = default;
    StaConfig & operator=(StaConfig & staconfig) = default;
    StaConfig & operator=(const StaConfig & staconfig) = default;

    bool operator==(StaConfig const& staconfig) const;

    bool operator!=(StaConfig const& staconfig) const;

    bool IsValid() const;

};

struct WifiExtenderConfig {
    
    AccessPointConfig apConfig;
    StaConfig         staConfig;

    WifiExtenderConfig();
    WifiExtenderConfig(const AccessPointConfig& ap, const StaConfig& sta);
    WifiExtenderConfig(const WifiExtenderConfig&)            = default;
    WifiExtenderConfig(WifiExtenderConfig&&)                 = default;
    WifiExtenderConfig& operator=(const WifiExtenderConfig&) = default;
    WifiExtenderConfig& operator=(WifiExtenderConfig&&)      = default;

    bool operator==(const WifiExtenderConfig& other) const;

    bool operator!=(const WifiExtenderConfig& other) const;
};


class WifiExtenderHelpers
{
public:

    static constexpr std::string_view WifiExtenderStaToString(const WifiExtenderState & state)
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


}