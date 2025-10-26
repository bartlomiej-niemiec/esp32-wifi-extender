#include "utils/MutexLockGuard.hpp"
#include "assert.h"

MutexLockGuard::MutexLockGuard (SemaphoreHandle_t mutex)
{
    assert(nullptr != mutex);
    m_mutex = mutex;
    xSemaphoreTake(m_mutex, portMAX_DELAY);
}

MutexLockGuard::~MutexLockGuard()
{
    xSemaphoreGive(m_mutex);
}