#pragma once

#include <cstddef>
#include <string_view>

namespace DataStorage
{

class DataRawStorerIf
{
    public:

        enum class ReadStatus {
            NOK = 0,
            OK,
            NOT_FOUND
        };  

        virtual ~DataRawStorerIf() = default;

        virtual bool Write(const std::string_view key, const void * pArg, std::size_t size) = 0;

        virtual ReadStatus Read(const std::string_view key, void * pArg, std::size_t & size) = 0;

        virtual bool Remove(const std::string_view key) = 0;
};

}
