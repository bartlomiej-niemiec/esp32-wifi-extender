#pragma once

#include "data_raw_storer_if.hpp"
#include <assert.h>
#include <memory>

namespace DataStorage
{

template <typename T>
class DataEntry
{

    public:

        using ReadStatus = DataRawStorerIf::ReadStatus;

        DataEntry(std::shared_ptr<DataRawStorerIf> pRawStorer, std::string_view key):
            m_pRawStorer(pRawStorer),
            m_key(key)
        {
            assert(m_pRawStorer != nullptr);
        };

        ReadStatus GetData(T & t) const
        {
            size_t size = 0;
            ReadStatus status = m_pRawStorer->Read(m_key, &t, size);
            if (status == ReadStatus::OK && size == sizeof(T))
            {
                return ReadStatus::OK;
            }
            
            return status;
        }

        bool SetData(const T & t)
        {
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