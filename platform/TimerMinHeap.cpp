#include <math.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <semaphore.h>
#include "platform/TimerMinHeap.h"
#include "platform/TimerTaskShared.h"
#include "platform/PosixSemaphore.h"
#include "platform/PosixSharedMemory.h"
#include "common/Util.h"

PosixSharedMemory* TimerMinHeap::pTimerMinHeapPool = new PosixSharedMemory ((char*) "/timerMinheapPool", 
									    O_RDWR | O_CREAT | O_EXCL, 
									    0666, 
									    PROT_READ | PROT_WRITE, 
									    (sizeof (UINT32) + (sizeof (Timer*) * MAX_NUMBER_OF_TIMERS)));
TimerMinHeap* TimerMinHeap::instance = 0;
PosixSemaphore* TimerMinHeap::pHeapSemaphore = 0;

TimerMinHeap* TimerMinHeap::getInstance()
{
  if (!instance)
    {
      instance = new TimerMinHeap();

      if (!pHeapSemaphore)
        pHeapSemaphore = new PosixSemaphore ((char*) "/heapSemaphore",
                                             O_CREAT | O_RDWR | O_EXCL,
                                             0666, 1);

      TimerTaskShared* shared = static_cast<TimerTaskShared*> (pTimerMinHeapPool->getSharedMem ());
      if (shared->numberOfTimersBeingManaged > 0)
        {
          UINT32 count = shared->numberOfTimersBeingManaged;
          shared->numberOfTimersBeingManaged = 0;
          for (UINT32 i = 0; i < count; i++)
            shared->heap[i] = NULL;
        }
    }

  return instance;
}

TimerMinHeap::TimerMinHeap()
{
}

Result TimerMinHeap::addToHeap (Timer* pTimer)
{
  pHeapSemaphore->wait ();
  Result r = upHeapify (pTimer);
  pHeapSemaphore->post ();
  return r;
}

Result TimerMinHeap::removeFromHeap (Timer* pTimer)
{
  pHeapSemaphore->wait ();
  Result r = removeSpecificFromIndex (pTimer, 0);
  pHeapSemaphore->post ();
  return r;
}

Result TimerMinHeap::upHeapify (Timer* pTimer)
{
  UINT32 heaped = 0;
  Timer** minheap = getHeap ();
  UINT32 index = getNumberOfTimers ();
  minheap [index] = pTimer;

  if (index == 0)
    {
    }
  else
    {
      while (!heaped)
	{
	  UINT32 parent = floor((index - 1)/2);
	  UINT64 parentFiringTime = minheap [parent]->getWhenToFire ();
	  UINT64 indexFiringTime = minheap [index]->getWhenToFire ();
	  if (uint64LT (parentFiringTime, indexFiringTime))
	    {
	      heaped = 1;
	    }
	  else
	    {
	      Timer* tmp = minheap [index];
	      minheap [index] = minheap [parent];
	      minheap [parent] = tmp;
	      
	      index = parent;
	    }

	  if (parent == 0)
	    heaped = 1;	  
	}
    }
  incNumberOfTimers ();
  return OK;
}

Timer* TimerMinHeap::removeFromIndex (UINT32& index)
{
  Timer** minheap = getHeap ();
  UINT32 parent = index;
  Timer* pTimer = minheap [parent];

  minheap [parent] = minheap [getNumberOfTimers () - 1];
  decNumberOfTimers ();
  downHeapifyFromIndex (index);

  return pTimer;
}

Result TimerMinHeap::removeSpecificFromIndex (Timer* pTimer, UINT32 index)
{
  UINT32 parent = index;
  UINT32 left = parent * 2 + 1;
  UINT32 right = left + 1;
  Timer** minheap = getHeap ();
  UINT32 numberOfTimers = getNumberOfTimers ();

  if (minheap [parent] == pTimer)
    {
      minheap [parent] = minheap [getNumberOfTimers () - 1];
      decNumberOfTimers ();

      downHeapifyFromIndex (parent);
    }
  else if ((left < (numberOfTimers - 1)) && (minheap [left] == pTimer))
    {
      minheap [left] = minheap [getNumberOfTimers () - 1];
      decNumberOfTimers ();

      downHeapifyFromIndex (left);
    }
  else if ((right < (numberOfTimers - 1)) && (minheap [right] == pTimer))
    {
      minheap [right] = minheap [getNumberOfTimers () - 1];
      decNumberOfTimers ();
      
      downHeapifyFromIndex (right);
    }
  else if (left > numberOfTimers)
    {
      return ERROR;
    }
  else
    {
      return removeSpecificFromIndex (pTimer, parent + 1);
    }  
  return OK;
}


void TimerMinHeap::downHeapifyFromIndex (UINT32 index)
{
  UINT32 heaped = 0;
  UINT32 parent = index;

  while (!heaped)
    {
      Timer** minheap = getHeap ();
      UINT32 left = 2 * parent + 1;
      UINT32 right = left + 1;
      UINT64 parentFiringTime = minheap [parent]->getWhenToFire ();
      UINT64 leftFiringTime, rightFiringTime;

      if (left < getNumberOfTimers ())
	{
	  leftFiringTime = minheap [left]->getWhenToFire ();
	}
      else
	{
	  heaped = 1;
	}
	    
      if (right <= getNumberOfTimers ())
	rightFiringTime = minheap [right]->getWhenToFire ();
      else
	{
	  heaped = 1;
	}
      if ((left >= (getNumberOfTimers ())) || (right >= getNumberOfTimers ())) 
	{
	  heaped = 1;
	  return;
	}

      if (uint64LT (leftFiringTime, parentFiringTime) ||
	  uint64LT (rightFiringTime, parentFiringTime))
	{
	  if (uint64LT (leftFiringTime, rightFiringTime))
	    {
	      Timer* tmp = minheap [left];
	      minheap [left] = minheap [parent];
	      minheap [parent] = tmp;	      

	      parent = left;
	    }
	  else if (uint64GEQ (leftFiringTime, rightFiringTime))
	    {
	      Timer* tmp = minheap [right];
	      minheap [right] = minheap [parent];
	      minheap [parent] = tmp;	      
	      
	      parent = right;
	    }
	}
      else
	{
	  heaped = 1;
	}
    }
}
