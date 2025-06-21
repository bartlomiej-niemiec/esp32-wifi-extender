#ifndef OS_INTERFACE_OSTHREAD_HPP
#define OS_INTERFACE_OSTHREAD_HPP

#include <stdint.h>

namespace Os
{

namespace Thread
{

class OsThread{

    typedef void (*TaskFunction)(void *pArg);

    typedef uint32_t TaskDescriptor;

    typedef enum{
        OK,
        NOK
    } Status; 

    virtual Status create(TaskFunction pTaskFunction,
        const char *pTaskName,
        int stackSize, void *pArg, int taskPriority, TaskDescriptor* pTaskDesciptor) = 0;

};

}

}

#endif