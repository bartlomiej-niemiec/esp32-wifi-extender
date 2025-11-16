#include "nvs_data_storage_impl.hpp"
#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"

#include "utils/MutexLockGuard.hpp"

namespace DataStorage::NvsImpl
{

NvsDataStorer::NvsDataStorer():
    m_Semaphore(nullptr)
{
    m_Semaphore = xSemaphoreCreateMutex();
    assert(nullptr != m_Semaphore);

    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

NvsDataStorer::~NvsDataStorer()
{
    vSemaphoreDelete(m_Semaphore);
}

bool NvsDataStorer::Write(const std::string_view key, const void * pArg, std::size_t size)
{
    MutexLockGuard lockGuard(m_Semaphore);
    nvs_handle_t nvs_handle{};

    esp_err_t err = nvs_open(NVS_NAMESPACE.data(), NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGE(LOGGER_TAG.data(), "Error (%s) opening NVS handle!", esp_err_to_name(err));
        return false;
    }

    ESP_LOGI(LOGGER_TAG.data(), "Saving data blob...");
    err = nvs_set_blob(nvs_handle, key.data(), pArg, size);
    if (err != ESP_OK) {
        ESP_LOGE(LOGGER_TAG.data(), "Failed to write data blob!");
        nvs_close(nvs_handle);
        return false;
    }

    err = nvs_commit(nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGE(LOGGER_TAG.data(), "Failed to commit data");
    }

    nvs_close(nvs_handle);
    return err;
}

DataRawStorerIf::ReadStatus NvsDataStorer::Read(const std::string_view key, void * pArg, std::size_t & size)
{
    MutexLockGuard lockGuard(m_Semaphore);
    nvs_handle_t nvs_handle{};

    esp_err_t err = nvs_open(NVS_NAMESPACE.data(), NVS_READONLY, &nvs_handle);
    if (err != ESP_OK) return ReadStatus{};

    ESP_LOGI(LOGGER_TAG.data(), "Reading data blob:");
    err = nvs_get_blob(nvs_handle, key.data(), pArg, &size);

    nvs_close(nvs_handle);

    if (err == ESP_OK) return ReadStatus::OK;
    else if(err == ESP_ERR_NVS_NOT_FOUND ) return ReadStatus::NOT_FOUND;
    
    return ReadStatus{};
}

bool NvsDataStorer::Remove(const std::string_view key)
{
    MutexLockGuard lockGuard(m_Semaphore);
    nvs_handle_t nvs_handle{};

    esp_err_t err = nvs_open(NVS_NAMESPACE.data(), NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) return false;

    err = nvs_erase_key(nvs_handle, key.data());

    nvs_commit(nvs_handle);
    nvs_close(nvs_handle);

    return ((err == ESP_OK) || (err == ESP_ERR_NVS_NOT_FOUND ));
}

}