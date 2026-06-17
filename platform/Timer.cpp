#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdlib.h>
#include "platform/Timer.h"
#include "platform/TimerCallback.h"
#include "platform/TimerMinHeap.h"
#include "platform/TimerUtils.h"

MemoryPool* Timer::pTimerPool = new MemoryPool ((char*) "timerObjectPool",
						(char*) "/timerObjectPool",
						O_RDWR | O_CREAT | O_EXCL, PROT_READ | PROT_WRITE, 0666, 
						sizeof (Timer), MAX_NUMBER_OF_TIMERS);

PosixSemaphore* Timer::pTimerSemaphore = new PosixSemaphore ((char*) "/timerSemaphore", 
							     O_CREAT | O_RDWR | O_EXCL, 
							     0666, 
							     1);
Timer::Timer () :
  type (INVALID),
  state (IDLE),
  pCb (NULL)
{
  first.uint64 = 0;
  timeout.uint64 = 0;
}

void Timer::scheduleOneShotAtAbsoluteTime (TimerCallback* _pCb, UINT64& _absoluteTime)
{
  UINT64 dummy;
  UINT32 zeroTimes = 0;

  schedule (_pCb, 
	    _absoluteTime, true, 
	    dummy, false,
	    ONE_SHOT,
	    zeroTimes, false);
}

void Timer::scheduleOneShotAtTimeout (TimerCallback* _pCb, UINT64& _timeout)
{
  UINT64 dummy;
  UINT32 zeroTimes = 0;

  schedule (_pCb,
	    _timeout, false,
	    dummy, false,
	    ONE_SHOT,
	    zeroTimes, false);
}

void Timer::scheduleOneShotBetweenAbsoluteTimes (TimerCallback* _pCb, UINT64& _absoluteTime1, UINT64& _absoluteTime2)
{
  UINT32 zeroTimes = 0;

  schedule (_pCb,
	    _absoluteTime1, true,
	    _absoluteTime2, true,
	    ONE_SHOT,
	    zeroTimes, true);
}

void Timer::scheduleOneShotBetweenTimeouts (TimerCallback* _pCb, UINT64& _timeout1, UINT64& _timeout2)
{
  UINT32 zeroTimes = 0;

  schedule (_pCb,
	    _timeout1, false,
	    _timeout2, false,
	    ONE_SHOT,
	    zeroTimes, true);
}

void Timer::schedulePeriodicAtAbsoluteTime (TimerCallback* _pCb, UINT64& _absoluteTime, UINT64& _timeout, UINT32& _howManyTimesToFire)
{
  schedule (_pCb,
	    _absoluteTime, true,
	    _timeout, false,
	    PERIODIC,
	    _howManyTimesToFire, false);
}

void Timer::schedulePeriodicAtTimeout (TimerCallback* _pCb, UINT64& _timeout, UINT32& _howManyTimesToFire)
{
  schedule (_pCb,
	    _timeout, false,
	    _timeout, false,
	    PERIODIC,
	    _howManyTimesToFire, false);
}

void Timer::schedule (TimerCallback* _pCb,
		      UINT64& _first, bool _firstIsAbsolute,
		      UINT64& _timeout,  bool _timeoutIsAbsolute,
		      TimerType _type,
		      UINT32& _howManyTimesToFire, bool _isFuzzyOneShot)
{
  TimerMinHeap* heap = TimerMinHeap::getInstance ();

  pTimerSemaphore->wait ();

  if (!pCb)
    {
      pCb = _pCb;
    }

  state = ACTIVE;
  type = _type;
  howManyTimesToFire = _howManyTimesToFire;

  if (_isFuzzyOneShot)
    {      
      UINT64 x1, x2, randomTimeout;

      if (_firstIsAbsolute)
	  x1 = convertAbsoluteTimeSinceEpochToRelative (_first);
      else
	x1 = _first;

      if (_timeoutIsAbsolute)
	x2 = convertAbsoluteTimeSinceEpochToRelative (_timeout);
      else
	x2 = _timeout;

      if (x2.uint64 >= x1.uint64)
	{
	  randomTimeout.uint64 = x1.uint64 + 1 + (rand () % (x2.uint64 - x1.uint64));
	  whenToFire = convertTimerTimeoutToAbsoluteTimeSinceEpoch (randomTimeout);
	}
    }
  else if (_firstIsAbsolute)
    {
      whenToFire = first;
    }
  else
    {      
      first.uint64 = _first.uint64;
      whenToFire = convertTimerTimeoutToAbsoluteTimeSinceEpoch (first);
    }

  if (_timeoutIsAbsolute)
    {
      timeout = convertAbsoluteTimeSinceEpochToRelative (_timeout);
    }
  else
    {
      timeout.uint64 = _timeout.uint64;      
    }

  numberOfTimesFired = 0;

  if (heap != NULL)
    {
      heap->addToHeap (this);
    }

  pTimerSemaphore->post ();
}


void Timer::cancel ()
{
  pTimerSemaphore->wait ();

  if (pCb)
    {
      delete pCb;
      pCb = NULL;
    }

  pTimerSemaphore->post ();
}

void Timer::changeFiringTime (UINT64 _newTimeout)
{
  TimerMinHeap* heap = TimerMinHeap::getInstance ();
  UINT64 nextFiringTime;

  pTimerSemaphore->wait ();

  if (state != DONE)
    heap->removeFromHeap (this);
  state = ACTIVE;
  nextFiringTime = convertTimerTimeoutToAbsoluteTimeSinceEpoch (_newTimeout);
  this->setTimeout (_newTimeout);
  this->setWhenToFire (nextFiringTime);
  heap->addToHeap (this);

  pTimerSemaphore->post ();
}

Timer::~Timer ()
{
  TimerMinHeap* heap = TimerMinHeap::getInstance ();

  pTimerSemaphore->wait ();

  if ((type != INVALID) && (state != DONE))
    {
      heap->removeFromHeap (this);
    }

  if (pCb)
    {
      delete pCb;
    }

  pTimerSemaphore->post ();
}

void* Timer::operator new (size_t size)
{
  void* newObject;

  pTimerSemaphore->wait ();

  newObject = pTimerPool->getFromPool ();

  pTimerSemaphore->post ();

  return newObject;
}

void Timer::operator delete (void* object, size_t size)
{
  pTimerSemaphore->wait ();

  pTimerPool->removeFromPool (object);

  pTimerSemaphore->post ();
}
