#include "thread.hpp"

namespace Os
{

namespace Thread
{

OsThread::Status OsThread::create(TaskFunction pTaskFunction, const char *pTaskName, int stackSize, void *pArg, int taskPriority, TaskDescriptor* pTaskDesciptor)
{
    return Status::OK;
}

}

}