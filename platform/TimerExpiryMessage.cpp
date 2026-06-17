#include "platform/TimerExpiryMessage.h"
#include "platform/Timer.h"
#include "platform/TimerMinHeap.h"
#include "platform/TimerCallback.h"
#include "common/Debug.h"

TimerExpiryMessage::TimerExpiryMessage (Timer* _pTimer) :
  pTimer (_pTimer)
{
}

TimerExpiryMessage::~TimerExpiryMessage()
{
  /**
     Don't delete pTimer--if the expiry message was for a peridic
     timer, deleteing the timer will cause untold misery!
  */
}

void TimerExpiryMessage::process ()
{
  TimerMinHeap* heap = TimerMinHeap::getInstance ();
  Timer::TimerType timerType = pTimer->getTimerType (); 
  
  cout << "Timer " << hex << pTimer << dec << endl;
  switch (timerType)
    {
    case Timer::ONE_SHOT:
      {
	// Execute the callback
	TimerCallback* pCb = pTimer->getCallback ();
	pCb->triggered ();
	// Delete the timer
	delete pTimer;
	break;
      }
    case Timer::PERIODIC:
      {
	UINT64 timerTimeout = pTimer->getTimeout ();

	if (pTimer->getNumberOfTimesFired () < pTimer->getHowManyTimesToFire ())
	  {
	    pTimer->changeFiringTime (timerTimeout);
	  }
	else
	  {
	    // Remove the timer object
	    delete pTimer;
	  }
	
	break;
      }
    default:
      {
	/* Error */
      }
    }
}
