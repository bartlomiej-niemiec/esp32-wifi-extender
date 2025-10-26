#ifndef HW_WIFIEXTENDER_WIFIEXTENDERSCANNER_TYPES_HPP
#define HW_WIFIEXTENDER_WIFIEXTENDERSCANNER_TYPES_HPP

#include <stdint.h>
#include <string>
#include <array>
#include <functional>

namespace WifiExtender
{

enum class ScannerState { Idle, Scanning, HaveResults, Error };

enum class AuthMode : uint8_t { Open, WEP, WPA_PSK, WPA2_PSK, WPA_WPA2_PSK, WPA3_PSK, Unknown };

struct WifiNetwork {
    static constexpr int MAX_SSID_SIZE = 32;
    static constexpr int MAX_BSSID_SIZE = 32;

    std::array<char, MAX_SSID_SIZE>   ssid{};
    std::array<uint8_t, MAX_BSSID_SIZE> bssid{};
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

#endif