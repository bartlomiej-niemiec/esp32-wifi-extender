#include "data_storer_if/data_storer.hpp"
#include "nvs_data_storage_impl.hpp"

namespace DataStorage
{

std::shared_ptr<DataRawStorerIf> DataStorer::m_pDataRawStorerIf = nullptr;

void DataStorer::Init()
{
    assert(m_pDataRawStorerIf == nullptr);
    m_pDataRawStorerIf = std::make_shared<NvsImpl::NvsDataStorer>();
}

}