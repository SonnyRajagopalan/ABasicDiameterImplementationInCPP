#ifndef TIMER_MIN_HEAP
#define TIMER_MIN_HEAP

#include "common/Types.h"
#include "Timer.h"
#include "PosixSemaphore.h"
#include "PosixSharedMemory.h"
#include "TimerTaskShared.h"
#include "common/Debug.h"

class TimerScheduler;
class TimerMinHeap
{
 public:
  friend class TimerScheduler;
  friend class TimerTask;

  static TimerMinHeap* getInstance ();

  Result addToHeap (Timer* pTimer);
  Result removeFromHeap (Timer* pTimer);
  UINT32 getNumberOfTimers () { return ((TimerTaskShared*) pTimerMinHeapPool->getSharedMem ())->numberOfTimersBeingManaged; }
  void   setNumberOfTimers (UINT32 _numberOfTimersBeingManaged) { ((TimerTaskShared*) pTimerMinHeapPool->getSharedMem ())->numberOfTimersBeingManaged = _numberOfTimersBeingManaged; }
  void   incNumberOfTimers () { ((TimerTaskShared*) pTimerMinHeapPool->getSharedMem ())->numberOfTimersBeingManaged++; };
  void   decNumberOfTimers () { ((TimerTaskShared*) pTimerMinHeapPool->getSharedMem ())->numberOfTimersBeingManaged--; };
  Timer** getHeap () { return ((TimerTaskShared*) pTimerMinHeapPool->getSharedMem ())->heap; }
 private:
  TimerMinHeap ();
  static TimerMinHeap* instance; // Singleton access *per process*!
  static PosixSharedMemory* pTimerMinHeapPool; // Shared access across processes
  static PosixSemaphore* pHeapSemaphore; // Cross-process lock for heap operations

  Timer* removeFromIndex (UINT32& index);
  Result removeSpecificFromIndex (Timer* pTimer, UINT32 index);
  void   downHeapifyFromIndex (UINT32 index);
  Result upHeapify (Timer* pTimer);
};

#endif
