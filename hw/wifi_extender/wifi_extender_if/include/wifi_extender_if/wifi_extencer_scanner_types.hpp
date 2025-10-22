#ifndef HW_WIFIEXTENDER_WIFIEXTENDERSCANNER_TYPES_HPP
#define HW_WIFIEXTENDER_WIFIEXTENDERSCANNER_TYPES_HPP

#include <stdint.h>
#include <string>
#include <array>
#include <functional>

namespace Hw
{

namespace WifiExtender
{

enum class ScannerState { Idle, Scanning, HaveResults, Cancelled, Error };

enum class AuthMode : uint8_t { Open, WEP, WPA_PSK, WPA2_PSK, WPA_WPA2_PSK, WPA3_PSK, Unknown };

struct WifiNetwork {
    static constexpr int MAX_SSID_LEN = 33;
    static constexpr int MAX_BSSID_LEN = 33;

    std::array<char, MAX_SSID_LEN>   ssid{};
    std::array<uint8_t, MAX_BSSID_LEN> bssid{};
    int8_t                rssi{};
    uint8_t               channel{};
    AuthMode              auth{};
};

struct ScanOptions {
    bool     passive    = true;
    uint16_t dwell_ms   = 120;
    bool     show_hidden= true;
};

using WifiNetworkCallback = std::function<void(const WifiNetwork&)>;
using ScanFinishedCallback = std::function<void()>;
 
}

}

#endif