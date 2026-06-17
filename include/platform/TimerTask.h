#ifndef TIMER_TASK_H
#define TIMER_TASK_H

#include "Runnable.h"
#include "PosixSharedMemory.h"
#include "PosixSemaphore.h"
#include "PosixMessageQueue.h"
#include "MemoryPool.h"

class TimerTask : public Runnable
{
 public:
  void initialize (void* pArg);
  void run (void* pArg);  
  UINT32 getTaskPid () { return pid; }
  void   setTaskPid (UINT32 _pid) { pid = _pid; }
 private:
    UINT32 pid;
    PosixSharedMemory* pTimerMinHeapPool;
    PosixSemaphore* pTimerSemaphore;
    MemoryPool* pTimerPool;
    MemoryPool* pTestCallbackPool;
    PosixMessageQueue* pMessageQueueToMain;
};

#endif
