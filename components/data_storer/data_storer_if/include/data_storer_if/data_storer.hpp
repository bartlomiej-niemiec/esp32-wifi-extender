#pragma once

#include <optional>

#include "data_entry.hpp"
#include <string_view>
#include <cassert>
#include <memory>

namespace DataStorage
{

class DataStorer
{
    public:

        static void Init();

        static DataStorer & GetInstance()
        {
            assert(m_pDataRawStorerIf != nullptr);
            static DataStorer instance;
            return instance;
        }

        template <typename T>
        DataEntry<T> GetDataEntry(std::string_view key)
        {
            assert(m_pDataRawStorerIf != nullptr);
            return DataEntry<T>(m_pDataRawStorerIf, key);
        }

    private:

        DataStorer() = default;

        ~DataStorer() = default;

        static std::shared_ptr<DataRawStorerIf> m_pDataRawStorerIf;

};

}
