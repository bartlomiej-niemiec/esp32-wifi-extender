#ifndef HW_WIFIEXTENDER_WIFIMAP_HPP
#define HW_WIFIEXTENDER_WIFIMAP_HPP

#include "wifi_extender_if/wifi_extender_config.hpp"
#include "esp_netif_types.h"

namespace Hw
{

namespace Platform
{

namespace WifiExtender
{

class WifiAp{

    public:
    
        WifiAp();

        bool Init();

        bool SetConfig(const Hw::WifiExtender::AccessPointConfig &ap_config);

        bool EnableNat();

        bool DisableNat();

        void SetUpDnsOnDhcpServer(esp_netif_dns_info_t  dnsInfo);

        enum class State {
            NOT_INITIALIZED,
            INITIALIZED,
            CONFIGURED,
            STARTED,
            CLIENTS_CONNECTED,
            STOPPED,
            ERROR
        };

        void SetState(WifiAp::State state);

        WifiAp::State GetState();

    private:

        esp_netif_t *m_ap_netif;

        State m_State;

        static constexpr int DHCPS_OFFER_DNS = 0x02;
};


}

}

}

#endif