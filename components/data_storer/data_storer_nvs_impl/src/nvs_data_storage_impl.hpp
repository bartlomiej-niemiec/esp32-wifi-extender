#pragma once

#include "data_storer_if/data_raw_storer_if.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

namespace DataStorage::NvsImpl
{

class NvsDataStorer:
    public DataRawStorerIf
{
    public:

        NvsDataStorer();

        ~NvsDataStorer();

        bool Write(const std::string_view key, const void * pArg, std::size_t size) final;

        DataRawStorerIf::ReadStatus Read(const std::string_view key, void * pArg, std::size_t & size) final;

        bool Remove(const std::string_view key) final;

    private:

        static constexpr std::string_view NVS_NAMESPACE = "config";

        static constexpr std::string_view LOGGER_TAG = "config_storer";

        SemaphoreHandle_t m_Semaphore;
};

}