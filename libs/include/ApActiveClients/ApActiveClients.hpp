#pragma once

#include <stdint.h>
#include <string>
#include <set>

namespace Libs
{

class ApActiveClients
{
    public:

        static constexpr size_t MAC_ADDRESS_BYTES_LEN = 6;

        static uint64_t convertMacAddrArrToUint64(const uint8_t mac[MAC_ADDRESS_BYTES_LEN]);

        void addClient(const uint64_t macAddrUint64);

        void removeClient(const uint64_t macAddrUint64);

        bool isClientActive(const uint64_t macAddrUint64) const;

    private:

        std::string macToString(const uint64_t macAddrUint64);

        void printActiveClients();

        std::set<uint64_t> m_ActiveClientsSet;

        #ifdef ESP_PLATFORM
        static constexpr std::string_view ESP_LOG_TAG = "wifi softAP";
        #endif
};

}