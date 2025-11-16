#include "wifi_extender_impl.hpp"
#include "utils/MutexLockGuard.hpp"
#include "nvs_flash.h"

namespace WifiExtender
{

WifiExtenderImpl::WifiExtenderImpl():
        m_CurrentConfig(),
        m_WifiManager(),
        m_Semaphore()
{
    m_Semaphore = xSemaphoreCreateMutex();
    assert(nullptr != m_Semaphore);
}

WifiExtenderImpl::~WifiExtenderImpl()
{
    vSemaphoreDelete(m_Semaphore);
}


bool WifiExtenderImpl::Init()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    return ret == ESP_OK;
}

bool WifiExtenderImpl::Startup(const WifiExtenderConfig & config)
{
    MutexLockGuard lockGuard(m_Semaphore);
    if (m_WifiManager.IsStartupPossible())
    {
        if (m_WifiManager.Startup(config))
        {
            m_CurrentConfig = config;
            return true;
        }
    }
    return false;
}

bool WifiExtenderImpl::RegisterListener(EventListener * pEventListener)
{
    return m_WifiManager.RegisterListener(pEventListener);
}

bool WifiExtenderImpl::Shutdown()
{
    MutexLockGuard lockGuard(m_Semaphore);
    if (m_WifiManager.IsShutdownPossible())
    {
        return m_WifiManager.Shutdown();
    }
    return false;
}

bool WifiExtenderImpl::UpdateConfig(const WifiExtenderConfig & config)
{
    MutexLockGuard lockGuard(m_Semaphore);
    if (m_WifiManager.IsUpdateConfigPossible() == false)
    {
        return false;
    }

    bool isApNewConfig = config.apConfig != m_CurrentConfig.apConfig;
    bool isStaNewConfig = config.staConfig != m_CurrentConfig.staConfig;
    
    if (isApNewConfig || isStaNewConfig)
    {
        AccessPointConfig newApConfig = isApNewConfig ? config.apConfig : m_CurrentConfig.apConfig;
        StaConfig newStaConfig = isStaNewConfig ? config.staConfig : m_CurrentConfig.staConfig;

        WifiExtenderConfig newConfig(newApConfig, newStaConfig);

        if (m_WifiManager.UpdateConfig(newConfig))
        {
            m_CurrentConfig = newConfig;
            return true;
        }
    } 

    return false;
}

bool WifiExtenderImpl::TryToReconnect()
{
    MutexLockGuard lockGuard(m_Semaphore);
    if (GetState() == WifiExtenderState::STA_CANNOT_CONNECT)
    {
        if (m_WifiManager.TryToReconnect())
        {
            return m_WifiManager.Shutdown();
        }
    }
    return false;
}

WifiExtenderState WifiExtenderImpl::GetState() const
{
    return m_WifiManager.GetState();
}

}
