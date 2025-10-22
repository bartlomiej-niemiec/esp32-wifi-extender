#ifndef HW_WIFIEXTENDER_WIFISTA_HPP
#define HW_WIFIEXTENDER_WIFISTA_HPP

#include "wifi_extender_if/wifi_extender_config.hpp"
#include "esp_netif.h"

namespace Hw
{

namespace Platform
{

namespace WifiExtender
{

class WifiSta{

    public:

        WifiSta();

        ~WifiSta();

        enum class State {
            NOT_INITIALIZED,
            INITIALIZED,
            CONFIGURED,
            CONNECTING,
            CONNECTED,
            CONNECTED_AND_GOT_IP,
            DISCONNECTED,
            STOPPED,
            ERROR
        };

        bool Init();

        bool SetConfig(const Hw::WifiExtender::StaConfig &sta_config);

        esp_netif_dns_info_t GetDnsInfo();

        esp_netif_ip_info_t GetIpInfo();

        void SetState(WifiSta::State state);

        void SetDefaultNetIf();

        WifiSta::State GetState() const;

    private:

        esp_netif_t *m_sta_netif;

        State m_State;
};


}

}

}


#endif