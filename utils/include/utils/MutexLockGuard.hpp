#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

class MutexLockGuard
{
    public:
        explicit MutexLockGuard (SemaphoreHandle_t mutex);

        ~MutexLockGuard();
    private:

        MutexLockGuard() = delete;

        SemaphoreHandle_t m_mutex;
        bool m_locked;
};
