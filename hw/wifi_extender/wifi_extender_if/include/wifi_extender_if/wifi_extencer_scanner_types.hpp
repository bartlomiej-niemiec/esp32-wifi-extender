#ifndef HW_WIFIEXTENDER_WIFIEXTENDERSCANNER_TYPES_HPP
#define HW_WIFIEXTENDER_WIFIEXTENDERSCANNER_TYPES_HPP

#include <stdint.h>
#include <string>
#include <cstring>
#include <array>
#include <functional>
#include "esp_log.h"

namespace WifiExtender
{

enum class ScannerState { Idle, Scanning, Done, Cancelled, Error };

enum class AuthMode : uint8_t { Open, WEP, WPA_PSK, WPA2_PSK, WPA_WPA2_PSK, WPA3_PSK, Unknown };

struct WifiNetwork {
    static constexpr int MAX_SSID_SIZE = 34;
    static constexpr int MAX_BSSID_SIZE = 34;

    std::array<char, MAX_SSID_SIZE>   ssid{};
    std::array<uint8_t, MAX_BSSID_SIZE> bssid{};
    int8_t                rssi{};
    uint8_t               channel{};
    AuthMode              auth{};

    WifiNetwork(uint8_t * pssid,
        uint8_t ssid_size,
        uint8_t * pbssid,
        uint8_t bssid_size,
        int8_t rssi,
        uint8_t channel,
        AuthMode auth
    )
    {
        assert(ssid_size <= MAX_SSID_SIZE);
        memcpy(ssid.data(), pssid, ssid_size);

        assert(bssid_size <= MAX_BSSID_SIZE);
        memcpy(bssid.data(), pbssid, bssid_size);

        this->rssi = rssi;
        this->channel = channel;
        this->auth = auth;
    }

};

static void printNetwork(const WifiNetwork & netowrk)
{
    ESP_LOGI("WifiScanner", "Network SSID: %s", netowrk.ssid.data());
    ESP_LOGI("WifiScanner", "Network BSSID: %s", netowrk.bssid.data());
    ESP_LOGI("WifiScanner", "Network rssi: %i", netowrk.rssi);
    ESP_LOGI("WifiScanner", "Network auth mode: %i", static_cast<int>(netowrk.auth));
}

static const std::string_view getScannerStateString(ScannerState state)
{
    switch (state)
    {
        case ScannerState::Idle: return "Idle";
        case ScannerState::Done: return "Done";
        case ScannerState::Cancelled: return "Cancelled";
        case ScannerState::Error: return "Error";
        case ScannerState::Scanning: return "Scanning";
    };
    return "Unknown Scanner State";
}

struct ScanOptions {
    bool     passive    = true;
    uint16_t dwell_ms   = 120;
    bool     show_hidden= true;
};

using ScanFinishedCallback = std::function<void()>;
 
}

#endif