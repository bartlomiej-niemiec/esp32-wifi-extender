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

        bool Init();

        bool SetConfig(const Hw::WifiExtender::StaConfig &sta_config);

        esp_netif_dns_info_t GetDnsInfo();

        enum class State {
            NOT_INITIALIZED,
            INITIALIZED,
            CONFIGURED,
            CONNECTING,
            CONNECTED,
            DISCONNECTED,
            STOPPED,
            ERROR
        };

        void SetState(WifiSta::State state);

        WifiSta::State GetState();

    private:

        esp_netif_t *m_sta_netif;

        State m_State;
};


}

}

}


#endif