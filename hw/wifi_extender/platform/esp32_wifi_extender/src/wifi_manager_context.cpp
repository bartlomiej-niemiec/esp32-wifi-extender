#include "wifi_manager_context.hpp"
#include "esp_log.h"

namespace Hw
{

namespace Platform
{

namespace WifiExtender
{

WifiManagerContext::WifiManagerContext():
        m_ApClientsCounter(),
        m_WifiAp(),
        m_WifiSta(),
        m_WifiManagerState(WifiExtenderState::STOPPED),
        m_PendingNewConfiguration(),
        m_StaConfigurationValid(false)
{
};

WifiManagerContext::~WifiManagerContext()
{
}

void WifiManagerContext::SetStaConfigurationValid(const bool isStaConfValid){
    m_StaConfigurationValid = isStaConfValid;
}

void WifiManagerContext::OnApStart()
{
    ESP_LOGI("WifiAp", "ApStartEvent");
    m_WifiAp.SetState(WifiAp::State::STARTED);
}

void WifiManagerContext::OnApStop()
{
    ESP_LOGI("WifiAp", "ApStoptEvent");
    m_WifiAp.SetState(WifiAp::State::STOPPED);
    m_ApClientsCounter = 0;
}

void WifiManagerContext::OnApStaConnected()
{
    ESP_LOGI("WifiSta", "ApStaConnectedEvent");
    m_ApClientsCounter++;
    m_WifiAp.SetState(WifiAp::State::CLIENTS_CONNECTED);
}

void WifiManagerContext::OnApStaDisconnected()
{
    ESP_LOGI("WifiSta", "ApStaDisconnectedEvent");
    m_ApClientsCounter--;
    if (m_ApClientsCounter < 0)
    {
        m_ApClientsCounter = 0;
    }

    if (m_ApClientsCounter == 0 && !m_PendingNewConfiguration)
    {
        m_WifiAp.SetState(WifiAp::State::STARTED);
    }
}

void WifiManagerContext::OnStaConnected()
{
    ESP_LOGI("WifiSta", "StaConnectedEvent");
    m_WifiSta.SetState(WifiSta::State::CONNECTED);
}

void WifiManagerContext::OnStaDisconnected()
{   
    ESP_LOGI("WifiSta", "StaDisconnectEvent");
    if (m_PendingNewConfiguration)
    {
        m_WifiSta.SetState(WifiSta::State::STOPPED);
    }
    else
    {
        if (m_WifiSta.GetState() == WifiSta::State::CONNECTED)
        {
            m_WifiSta.SetState(WifiSta::State::DISCONNECTED);
        }
    }
}

void WifiManagerContext::OnStaStart()
{
    ESP_LOGI("WifiSta", "StaStartEvent");
    if (m_StaConfigurationValid == true)
    {
        m_WifiSta.SetState(WifiSta::State::CONNECTING);
    }
    else
    {
        m_WifiSta.SetState(WifiSta::State::CONFIGURED);
    }
}

void WifiManagerContext::OnStaStop()
{
    ESP_LOGI("WifiSta", "StaStopEvent");
    m_WifiSta.SetState(WifiSta::State::STOPPED);
}

void WifiManagerContext::OnStaGotIp()
{
    ESP_LOGI("WifiSta", "GotIpEvent");
    m_WifiSta.SetState(WifiSta::State::CONNECTED_AND_GOT_IP);
}

void WifiManagerContext::OnStaLostIp()
{
    ESP_LOGI("WifiSta", "LostIpEvent");
    if (m_WifiSta.GetState() != WifiSta::State::STOPPED)
    {
        m_WifiSta.SetState(WifiSta::State::DISCONNECTED);
    }
}


}


}


}