#pragma once

#include "data_raw_storer_if.hpp"
#include <cassert>
#include <memory>

namespace DataStorage
{

template <typename T>
class DataEntry
{

    public:

        using ReadStatus = DataRawStorerIf::ReadStatus;

        DataEntry(std::shared_ptr<DataRawStorerIf> pRawStorer, std::string_view key):
            m_pRawStorer(std::move(pRawStorer)),
            m_key(key)
        {
            assert(m_pRawStorer != nullptr);
        };

        ReadStatus GetData(T & t) const
        {
            static_assert(std::is_trivially_copyable_v<T>,
            "T must be trivially copyable (no std::string, std::vector, etc.)");

            std::size_t size = sizeof(T);
            auto status = m_pRawStorer->Read(m_key, &t, size);

            if (status != ReadStatus::OK) return status;
            return (size == sizeof(T)) ? ReadStatus::OK : ReadStatus::NOK;
        }

        bool SetData(const T & t)
        {
            static_assert(std::is_trivially_copyable_v<T>,
            "T must be trivially copyable");

            return m_pRawStorer->Write(m_key, &t, sizeof(T));
        }

        bool Remove()
        {
            return m_pRawStorer->Remove(m_key);
        }

    private:

        std::shared_ptr<DataRawStorerIf> m_pRawStorer;
        std::string_view m_key;

};

}