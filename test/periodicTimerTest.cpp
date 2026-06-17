#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <pthread.h>
#include <unistd.h>
#include "platform/Timer.h"
#include "test/TestCallback.h"
#include "common/Types.h"
#include "platform/TimerTask.h"
#include "platform/TaskID.h"
#include "platform/PosixMessageQueue.h"
#include "platform/PosixMessageQueueObject.h"
#include "common/Debug.h"

using namespace std;

static void usage (const char* prog)
{
  cerr << "Usage: " << prog << " -c <count> -t <timeout_ms>" << endl;
  cerr << "  -c    Number of times each periodic timer fires (required)" << endl;
  cerr << "  -t    Base timeout in milliseconds (required)" << endl;
  cerr << "Example: " << prog << " -c 5 -t 2000" << endl;
  cerr << "  Creates a 2-second periodic timer that fires 5 times," << endl;
  cerr << "  then a 20-second periodic timer that fires 5 times." << endl;
  exit (1);
}

void* getMessages (void* pArg)
{
  SINT32 retVal = 0;
  PosixMessageQueue* pMq = new PosixMessageQueue ((char *) "/toMain", O_RDWR | O_EXCL | O_CREAT, 0, 0666, 10, 4096);

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
  UINT32 numberOfTimesToFire = 0;
  UINT64 timeout;
  timeout.uint64 = 0;
  int opt;

  while ((opt = getopt (argc, argv, "c:t:")) != -1)
    {
      switch (opt)
	{
	case 'c':
	  numberOfTimesToFire = atoi (optarg);
	  break;
	case 't':
	  timeout.uint64 = atoi (optarg);
	  break;
	default:
	  usage (argv[0]);
	}
    }

  if (numberOfTimesToFire == 0 || timeout.uint64 == 0)
    usage (argv[0]);

  cout << "periodicTimerTest: firing " << numberOfTimesToFire
       << " times every " << timeout.uint64 << " ms" << endl;

  TimerTask tt;
  UINT32 pid = 0;
  Timer* pobj[15];
  TestCallback* pCb;
  int status = 0;
  pthread_t messageQueueThread;

  pthread_create(&messageQueueThread, NULL, getMessages, NULL);
  tt.initialize (NULL);
  pid = tt.getTaskPid ();

  cout << "[" << getpid() << "] Forked timer child PID=" << pid << endl;

  cout << "[" << getpid() << "] Creating a periodic timer every "
       << timeout.uint64 << " ms, firing " << numberOfTimesToFire << " times" << endl;

  for (int j = 0; j < 1; j++)
    {
      pCb = new TestCallback ();
      pobj[j] = new Timer ();
      pobj[j]->schedulePeriodicAtTimeout (pCb, timeout, numberOfTimesToFire);
    }

  UINT32 sleepSec = (timeout.uint64 * numberOfTimesToFire) / 1000 + 5;
  cout << "[" << getpid() << "] Sleeping " << sleepSec << " seconds..." << endl;
  sleep (sleepSec);

  UINT64 timeout2;
  timeout2.uint64 = timeout.uint64 * 10;
  pCb = new TestCallback ();
  pobj[1] = new Timer ();
  pobj[1]->schedulePeriodicAtTimeout (pCb, timeout2, numberOfTimesToFire);

  cout << "[" << getpid() << "] Creating a second periodic timer every "
       << timeout2.uint64 << " ms, firing " << numberOfTimesToFire << " times" << endl;

  pthread_join(messageQueueThread, NULL);
  wait (&status);
}
