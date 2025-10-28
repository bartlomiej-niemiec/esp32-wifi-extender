#include "utils/MutexLockGuard.hpp"
#include "assert.h"

MutexLockGuard::MutexLockGuard (SemaphoreHandle_t mutex):
    m_mutex(mutex),
    m_locked(false)
{
    assert(nullptr != mutex);
    m_locked = xSemaphoreTake(m_mutex, portMAX_DELAY) == pdTRUE;
}

MutexLockGuard::~MutexLockGuard()
{
    if (m_locked)
    {
        xSemaphoreGive(m_mutex);
    }
}