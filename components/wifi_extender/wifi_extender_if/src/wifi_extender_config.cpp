#include "wifi_extender_if/wifi_extender_config.hpp"

namespace WifiExtender
{

AccessPointConfig::AccessPointConfig(std::string ssid, std::string password, int max_clients):
    ssid(std::move(ssid)), password(std::move(password)), max_clients(max_clients)
{}

    
AccessPointConfig::AccessPointConfig():
    ssid(),
    password(),
    max_clients()
{}

bool AccessPointConfig::operator==(AccessPointConfig const& apconfig) const
{
    return ((apconfig.ssid == this->ssid) && (apconfig.password == this->password));
};

bool AccessPointConfig::operator!=(AccessPointConfig const& apconfig) const
{   
    return !this->operator==(apconfig);
};

bool AccessPointConfig::IsValid() const {
    return !ssid.empty() && !password.empty();
}


StaConfig::StaConfig(std::string ssid, std::string password):
    ssid(std::move(ssid)), password(std::move(password))
{}

StaConfig::StaConfig():
    ssid(),
    password()
{}

bool StaConfig::operator==(StaConfig const& staconfig) const
{
    return ((staconfig.ssid == this->ssid) && (staconfig.password == this->password));
};

bool StaConfig::operator!=(StaConfig const& staconfig) const
{   
    return !this->operator==(staconfig);
};

bool StaConfig::IsValid() const {
    return !ssid.empty() && !password.empty();
}


WifiExtenderConfig::WifiExtenderConfig(const AccessPointConfig& ap, const StaConfig& sta):
    apConfig(ap), staConfig(sta)
{}

WifiExtenderConfig::WifiExtenderConfig():
    apConfig(),
    staConfig()
{}

bool WifiExtenderConfig::operator==(const WifiExtenderConfig& other) const {
    return apConfig == other.apConfig && staConfig == other.staConfig;
}

bool WifiExtenderConfig::operator!=(const WifiExtenderConfig& other) const {
    return !(*this == other);
}


};