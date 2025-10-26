#ifndef UTILS_MUTEX_LOCK_GUARD_HPP
#define UTILS_MUTEX_LOCK_GUARD_HPP

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

class MutexLockGuard
{
    public:
        MutexLockGuard (SemaphoreHandle_t mutex);

        ~MutexLockGuard();
    private:

        MutexLockGuard() = delete;

        SemaphoreHandle_t m_mutex;
};


#endif