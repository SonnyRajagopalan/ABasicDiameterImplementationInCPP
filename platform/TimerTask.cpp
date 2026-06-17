#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <signal.h>
#include <fcntl.h>
#include <assert.h>
#include "platform/Timer.h"
#include "platform/TimerTaskShared.h"
#include "platform/TimerUtils.h"
#include "platform/TimerTask.h"
#include "platform/TimerMinHeap.h"
#include "platform/TimerExpiryMessage.h"
#include "platform/PosixMessageQueueObject.h"
#include "test/TestCallback.h"
#include "common/Util.h"
#include <cstring>

void TimerTask::initialize (void* pArg)
{
	TimerMinHeap::getInstance ();

	pid_t _pid = fork ();
	setTaskPid ((UINT32)_pid);
	assert (_pid >= 0);

	switch (_pid)
    {
    case 0: /* Child process */
      {
	pMessageQueueToMain = new PosixMessageQueue ((char *) "/toMain", O_RDWR, 0, 0666, 10, 4096);

	run (pArg);
	break;
      }
    case -1: /* Error */
      {
	assert (0);
      }
    default: /* Parent */
      {
	break;
      }
    }
}

void TimerTask::run (void* pArg)
{
  prctl (PR_SET_PDEATHSIG, SIGTERM);

  TimerMinHeap* heap = TimerMinHeap::getInstance ();
  while (1)
    {
      if (heap->getNumberOfTimers () == 0)
	{
	  usleep (200);
	  continue;
	}

      Timer** minheap = heap->getHeap ();
      Timer* top = minheap [0];
      UINT64 topFiringTime = top->getWhenToFire ();
      Timer::TimerState topState = top->getTimerState ();
      UINT64 topTimeout = top->getTimeout ();
      UINT64 current = getCurrentTimeInMsec ();

      if (uint64LEQ (topFiringTime, current))
	{
	  switch (topState)
	    {
	    case Timer::ACTIVE:
	      {
		UINT64 in = Timer::getIndex (top);
		PosixMessageQueueObject pMQO (TaskID (TaskID::TIMER_TASK), 0, in);
		CHAR* buf = new CHAR [sizeof (PosixMessageQueueObject)];

		top->incNumberOfTimesFired ();
		heap->removeFromHeap (top);
		top->setTimerState (Timer::DONE);

		pMQO.serializeToCharBuf (buf);
		pMessageQueueToMain->send (buf, sizeof (PosixMessageQueueObject));

		break;
	      }
	    case Timer::IDLE:
	      {
		top->changeFiringTime (topTimeout);
		break;
	      }
	    case Timer::DONE:
	      {
		break;
	      }
	    default:
	      {
		break;
	      }
	    }
	}
    }
}
