#include "ApActiveClients/ApActiveClients.hpp"
#include <cstring>

#define DEBUG_PRINTS

#ifdef ESP_PLATFORM
#include "esp_log.h"

#endif


namespace Libs
{

uint64_t ApActiveClients::convertMacAddrArrToUint64(const uint8_t mac[MAC_ADDRESS_BYTES_LEN])
{
    uint64_t mac_address = 0;
    memcpy(&mac_address, mac, 6);
    return mac_address;
}

void ApActiveClients::addClient(const uint64_t macAddrUint64)
{
    m_ActiveClientsSet.insert(macAddrUint64);
    #ifdef DEBUG_PRINTS
    printActiveClients();
    #endif
}

void ApActiveClients::removeClient(const uint64_t macAddrUint64)
{
    m_ActiveClientsSet.erase(macAddrUint64);
    #ifdef DEBUG_PRINTS
    printActiveClients();
    #endif
}

bool ApActiveClients::isClientActive(const uint64_t macAddrUint64) const
{
    return m_ActiveClientsSet.find(macAddrUint64) != m_ActiveClientsSet.end();
}

std::string ApActiveClients::macToString(const uint64_t macAddrUint64)
{
    uint8_t m[6];
    memcpy(m, &macAddrUint64, 6);
    char buf[18];
    snprintf(buf, sizeof(buf), "%02X:%02X:%02X:%02X:%02X:%02X",
                m[0], m[1], m[2], m[3], m[4], m[5]);
    return std::string(buf);
}

void ApActiveClients::printActiveClients()
{
    for (const uint64_t & client : m_ActiveClientsSet)
    {   
        #ifdef ESP_PLATFORM
        ESP_LOGI(ESP_LOG_TAG.data(), "Wifi AP Client: %s", macToString(client).c_str());
        #endif 
    }
}

}