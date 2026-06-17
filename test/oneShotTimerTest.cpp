#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h>
#include "platform/Timer.h"
#include "test/TestCallback.h"
#include "common/Types.h"
#include "platform/TimerTask.h"
#include "platform/TimerConstants.h"
#include "platform/TaskID.h"
#include "platform/TimerMinHeap.h"
#include "platform/PosixMessageQueue.h"
#include "platform/PosixMessageQueueObject.h"
#include "common/Debug.h"
#include "platform/TimerExpiryMessage.h"
#include "platform/MemoryPool.h"
#include <pthread.h>
#include <string.h>
#include <vector>
#include "common/Util.h"
#include <sstream>
using namespace std;

static void usage (const char* prog)
{
  cerr << "Usage: " << prog << " -c <count> -t <timeout_ms>" << endl;
  cerr << "  -c    Number of one-shot timers to create (required)" << endl;
  cerr << "  -t    Base timeout in milliseconds (required)" << endl;
  cerr << "Example: " << prog << " -c 5 -t 1000" << endl;
  cerr << "  Creates 5 one-shot timers firing at 1s, 2s, 3s, 4s, 5s." << endl;
  exit (1);
}

void* getMessages (void* pArg)
{
  SINT32 retVal = 0;
  PosixMessageQueue* pMq = new PosixMessageQueue ((char *) "/toMain", O_RDWR | O_EXCL | O_CREAT, 0, 0666, 10, 4096);

  cout << "Thread for catching messages to pMq created..." << endl;

  while (1)
    {
      retVal = pMq->receive ();
      if (retVal != -1)
	{
	  PosixMessageQueueObject pMQO;
	  CHAR* buf = pMq->getMsg ();
	  Timer* pTimer;
	  Timer::TimerType timerType;
	  pMQO.deserializeFromCharBuf (buf);
	  pTimer = Timer::getTimerAtIndex(pMQO.getShmIndex ());
	  timerType = pTimer->getTimerType ();

	  cout << "[" << getpid() << "] Timer fired: taskID=" << (pMQO.getTaskID()).getID ()
	       << " shmIdx=" << (pMQO.getShmIndex ()).uint64 << " timer=" << hex << pTimer << dec << endl;

	  switch (timerType)
	    {
	    case Timer::ONE_SHOT:
	      {
		TimerCallback* pCb = pTimer->getCallback ();
		pCb->triggered ();
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
		    delete pTimer;
		  }

		break;
	      }
	    default:
	      {
	      }
	    }
	}
      else
	{
	  cout << "Bad message received!" << endl;
	}
    }
}

int main (int argc, char* argv[])
{
  UINT32 timerCount = 0;
  UINT64 baseTimeout;
  baseTimeout.uint64 = 0;
  int opt;

  while ((opt = getopt (argc, argv, "c:t:")) != -1)
    {
      switch (opt)
	{
	case 'c':
	  timerCount = atoi (optarg);
	  break;
	case 't':
	  baseTimeout.uint64 = atoi (optarg);
	  break;
	default:
	  usage (argv[0]);
	}
    }

  if (timerCount == 0 || baseTimeout.uint64 == 0)
    usage (argv[0]);

  if (timerCount > 14)
    {
      cerr << "timerPoolTest: count must be <= 14" << endl;
      return 1;
    }

  cout << "oneShotTimerTest: creating " << timerCount
       << " one-shot timers, base timeout = " << baseTimeout.uint64 << " ms" << endl;

  TimerTask tt;
  UINT32 pid = 0;
  Timer* pobj[15];
  TestCallback* pCb;
  UINT64 timeout;
  int status = 0;
  pthread_t messageQueueThread;

  pthread_create (&messageQueueThread, NULL, getMessages, NULL);

  tt.initialize (NULL);
  pid = tt.getTaskPid ();

  cout << "[" << getpid() << "] Forked timer child PID=" << pid << endl;

  sleep (1);
  cout << "[" << getpid() << "] Allocating " << timerCount
       << " one-shot timers at " << baseTimeout.uint64 << " ms intervals" << endl;

  for (UINT32 j = 0; j < timerCount; j++)
    {
      pCb = new TestCallback ();
      timeout.uint64 = baseTimeout.uint64 * (j + 1);
      pobj[j] = new Timer ();
      pobj[j]->scheduleOneShotAtTimeout (pCb, timeout);
    }

  if (timerCount > 2)
    {
      cout << "[" << getpid() << "] Cancelling timer at index 2" << endl;
      delete pobj[2];
    }
  if (timerCount > 5)
    {
      cout << "[" << getpid() << "] Cancelling timer at index 5" << endl;
      delete pobj[5];
    }

  cout << "[" << getpid() << "] Waiting for timers to fire..." << endl;

  pthread_join(messageQueueThread, NULL);
  wait (&status);
}
