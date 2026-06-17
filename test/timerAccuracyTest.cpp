#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdint.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <sstream>
#include "platform/Timer.h"
#include "test/TestCallback.h"
#include "common/Types.h"
#include "platform/TimerTask.h"
#include "platform/TaskID.h"
#include "platform/PosixMessageQueue.h"
#include "platform/PosixMessageQueueObject.h"
#include "platform/TimerConstants.h"
#include "common/Debug.h"

using namespace std;

static vector<int64_t> gJitter;
static pthread_mutex_t gStatsMutex = PTHREAD_MUTEX_INITIALIZER;
static uint32_t gFiredCount = 0;
static PosixMessageQueue* gMq = NULL;
static volatile bool gDone = 0;

static void usage (const char* prog)
{
  cerr << "Usage: " << prog << " -c <count> (-t <timeout_ms> | -p <period_ms> -f <times>) [-q]" << endl;
  cerr << "  -c    Number of timers (required, max 1000)" << endl;
  cerr << "  -t    Base timeout in ms (one-shot mode, max 1000 concurrent)" << endl;
  cerr << "  -p    Periodic: each timer fires every period_ms (use for high volume)" << endl;
  cerr << "  -f    Fires per timer (periodic mode, default 1)" << endl;
  cerr << "  -n    Total fires (periodic mode: overrides -f)" << endl;
  cerr << "  -q    Quiet: suppress per-timer output" << endl;
  cerr << "Examples:" << endl;
  cerr << "  " << prog << " -c 100 -t 50" << endl;
  cerr << "    100 one-shot timers at 50, 100, ... 5000 ms." << endl;
  cerr << "  " << prog << " -c 50 -p 200 -f 10" << endl;
  cerr << "    50 periodic timers x10 = 500 fires." << endl;
  cerr << "  " << prog << " -c 500 -p 100 -f 2000 -q" << endl;
  cerr << "    500 x 2000 = 1,000,000 fires (no MQ overflow at 100ms period)." << endl;
  exit (1);
}

static void recordJitter (int64_t jitter)
{
  pthread_mutex_lock (&gStatsMutex);
  gJitter.push_back (jitter);
  gFiredCount++;
  pthread_mutex_unlock (&gStatsMutex);
}

static double computeStddev (const vector<int64_t>& values, double mean)
{
  if (values.size() < 2) return 0.0;
  double sumSq = 0.0;
  for (size_t i = 0; i < values.size(); i++)
  {
    double diff = (double)values[i] - mean;
    sumSq += diff * diff;
  }
  return sqrt (sumSq / (values.size() - 1));
}

static int64_t percentile (const vector<int64_t>& sorted, double pct)
{
  if (sorted.empty ()) return 0;
  size_t idx = (size_t)ceil (pct / 100.0 * sorted.size()) - 1;
  if (idx >= sorted.size()) idx = sorted.size() - 1;
  return sorted[idx];
}

class AccuracyCallback : public TimerCallback
{
public:
  AccuracyCallback (uint64_t _startMs, uint64_t _timeoutMs, bool _isPeriodic)
    : startMs (_startMs), timeoutMs (_timeoutMs), isPeriodic (_isPeriodic), fireIndex (0) {}

  virtual void triggered ()
  {
    struct timeval now;
    gettimeofday (&now, NULL);
    uint64_t actualUs = (uint64_t)now.tv_sec * 1000000ULL + (uint64_t)now.tv_usec;

    uint64_t expectedUs = (startMs + timeoutMs * (fireIndex + 1)) * 1000ULL;
    int64_t jitterUs = (int64_t)(actualUs - expectedUs);

    recordJitter (jitterUs);
    fireIndex++;
  }

  uint32_t getFireIndex () const { return fireIndex; }

private:
  uint64_t startMs;
  uint64_t timeoutMs;
  bool isPeriodic;
  uint32_t fireIndex;
};

void* getMessages (void* pArg)
{
  gMq = new PosixMessageQueue ((char*) "/toMain", O_RDWR | O_EXCL | O_CREAT, 0, 0666, 10, 4096);

  while (1)
  {
    SINT32 retVal = gMq->receive ();
    if (retVal == -1) continue;                     // error, retry
    if (retVal == 1 && gDone) break;                // dummy wake-up, exit
    if (retVal < (SINT32)sizeof (PosixMessageQueueObject)) continue;

    PosixMessageQueueObject pMQO;
    char* buf = gMq->getMsg ();
    pMQO.deserializeFromCharBuf (buf);
    Timer* pTimer = Timer::getTimerAtIndex (pMQO.getShmIndex ());
    Timer::TimerType timerType = pTimer->getTimerType ();
    TimerCallback* pCb = pTimer->getCallback ();
    pCb->triggered ();

    if (timerType == Timer::PERIODIC &&
        pTimer->getNumberOfTimesFired () < pTimer->getHowManyTimesToFire ())
    {
      UINT64 timerTimeout = pTimer->getTimeout ();
      pTimer->changeFiringTime (timerTimeout);
    }
    else
    {
      delete pTimer;
    }
  }
  return NULL;
}

int main (int argc, char* argv[])
{
  uint32_t timerCount = 0;
  uint64_t timeoutMs = 0;
  uint64_t periodMs = 0;
  uint32_t times = 1;
  uint64_t totalFiresTarget = 0;
  bool periodicMode = false;
  bool quiet = false;
  int opt;

  while ((opt = getopt (argc, argv, "c:t:p:f:n:q")) != -1)
  {
    switch (opt)
    {
    case 'c':
      timerCount = (uint32_t)atoi (optarg);
      break;
    case 't':
      timeoutMs = (uint64_t)atoi (optarg);
      break;
    case 'p':
      periodMs = (uint64_t)atoi (optarg);
      periodicMode = true;
      break;
    case 'f':
      times = (uint32_t)atoi (optarg);
      break;
    case 'n':
      totalFiresTarget = (uint64_t)atol (optarg);
      break;
    case 'q':
      quiet = true;
      break;
    default:
      usage (argv[0]);
    }
  }

  if (timerCount == 0) usage (argv[0]);
  if (periodicMode && periodMs == 0) usage (argv[0]);
  if (!periodicMode && timeoutMs == 0) usage (argv[0]);

  if (timerCount == 0 || timerCount > 1000)
  {
    cerr << "timerAccuracyTest: count must be between 1 and 1000" << endl;
    return 1;
  }

  if (times == 0) times = 1;

  if (totalFiresTarget > 0)
  {
    if (!periodicMode)
    {
      cerr << "timerAccuracyTest: -n only supported with -p (periodic mode)" << endl;
      cerr << "  One-shot mode is limited to 1000 concurrent timers." << endl;
      cerr << "  Use periodic mode for high-volume testing." << endl;
      return 1;
    }
    if (timerCount > 0)
      times = (uint32_t)((totalFiresTarget + timerCount - 1) / timerCount);
  }

  uint64_t totalExpectedFires = periodicMode ? (uint64_t)timerCount * times : timerCount;

  cout << "timerAccuracyTest: " << timerCount << " timers";
  if (periodicMode)
    cout << ", periodic every " << periodMs << " ms, " << times << " fires each";
  else
    cout << " at varying timeouts (" << timeoutMs << " to " << (timeoutMs * timerCount) << " ms)";
  cout << ", total expected fires = " << totalExpectedFires << endl;
  if (quiet) cout << "  (quiet mode)" << endl;

  gJitter.reserve ((size_t)totalExpectedFires);

  pthread_t messageQueueThread;
  pthread_create (&messageQueueThread, NULL, getMessages, NULL);
  usleep (200000);

  TimerTask tt;
  tt.initialize (NULL);
  uint32_t pid = tt.getTaskPid ();
  if (!quiet)
    cout << "[" << getpid () << "] Forked timer child PID=" << pid << endl;

  uint64_t maxEndMs = 0;
  uint32_t printInterval = (timerCount > 20 && !quiet) ? (timerCount / 10) : 1;

  for (uint32_t j = 0; j < timerCount; j++)
  {
    struct timeval now;
    gettimeofday (&now, NULL);
    uint64_t scheduleMs = (uint64_t)now.tv_sec * 1000ULL + (uint64_t)now.tv_usec / 1000;

    if (periodicMode)
    {
      AccuracyCallback* pCb = new AccuracyCallback (scheduleMs, periodMs, true);
      UINT64 period;
      period.uint64 = periodMs;
      UINT32 timesU32 = times;
      Timer* t = new Timer ();
      t->schedulePeriodicAtTimeout (pCb, period, timesU32);
      if (!quiet && (j % printInterval == 0 || j == timerCount - 1 || j == 0 || timerCount <= 20))
        cout << "  [" << getpid () << "] Scheduled periodic timer " << (j + 1)
             << " / " << timerCount << endl;
      uint64_t endMs = scheduleMs + periodMs * times + 2000;
      if (endMs > maxEndMs) maxEndMs = endMs;
    }
    else
    {
      uint64_t relativeMs = timeoutMs * (j + 1);
      AccuracyCallback* pCb = new AccuracyCallback (scheduleMs, relativeMs, false);
      UINT64 relTimeout;
      relTimeout.uint64 = relativeMs;
      Timer* t = new Timer ();
      t->scheduleOneShotAtTimeout (pCb, relTimeout);
      if (!quiet && (j % printInterval == 0 || j == 0 || timerCount <= 20 || j == timerCount - 1))
        cout << "  [" << getpid () << "] Scheduled one-shot " << (j + 1)
             << " / " << timerCount << " at +" << relativeMs << " ms" << endl;
      uint64_t endMs = scheduleMs + relativeMs + 2000;
      if (endMs > maxEndMs) maxEndMs = endMs;
    }
  }

  struct timeval now;
  gettimeofday (&now, NULL);
  uint64_t nowMs = (uint64_t)now.tv_sec * 1000ULL + (uint64_t)now.tv_usec / 1000;
  uint32_t waitSec = (nowMs < maxEndMs)
    ? (uint32_t)((maxEndMs - nowMs) / 1000) + 3
    : 5;

  cout << "[" << getpid () << "] Waiting up to " << waitSec
       << " seconds (" << totalExpectedFires << " fires expected)..." << endl;

  uint32_t slept = 0;
  uint32_t lastFired = 0;
  while (slept < waitSec)
  {
    sleep (1);
    slept++;
    {
      pthread_mutex_lock (&gStatsMutex);
      bool allDone = (gFiredCount >= totalExpectedFires);
      uint32_t fired = gFiredCount;
      pthread_mutex_unlock (&gStatsMutex);
      if (!quiet && totalExpectedFires > 1000 && fired > lastFired)
      {
        uint32_t pct = (uint32_t)((double)fired / totalExpectedFires * 100);
        cout << "\r  Progress: " << fired << " / " << totalExpectedFires
             << " fires (" << pct << "%)      " << flush;
        lastFired = fired;
      }
      if (allDone) break;
    }
  }
  if (!quiet && lastFired > 0) cout << endl;

  // Kill the child first to stop new timer firings, preventing MQ contention.
  kill (pid, SIGTERM);
  int status = 0;
  waitpid (pid, &status, 0);

  // Now drain any messages the child already sent.
  // The MQ thread will process all real messages and only exit on the dummy.
  gDone = true;
  PosixMessageQueue signalMq ((char*) "/toMain", O_RDWR, 0, 0666, 10, 4096);
  char dummy = 0;
  signalMq.send (&dummy, 1);
  pthread_join (messageQueueThread, NULL);

  // Drain any lingering messages (shouldn't happen, but be safe).
  gMq->setNonBlockingMode ();
  SINT32 drainRet;
  while ((drainRet = gMq->receive ()) > 0)
  {
    if (drainRet < (SINT32)sizeof (PosixMessageQueueObject))
      continue;
    PosixMessageQueueObject pMQO;
    char* buf = gMq->getMsg ();
    pMQO.deserializeFromCharBuf (buf);
    Timer* pTimer = Timer::getTimerAtIndex (pMQO.getShmIndex ());
    TimerCallback* pCb = pTimer->getCallback ();
    pCb->triggered ();
    delete pTimer;
  }

  pthread_mutex_lock (&gStatsMutex);
  uint32_t actualFires = gFiredCount;
  vector<int64_t> jitterCopy = gJitter;
  pthread_mutex_unlock (&gStatsMutex);

  sort (jitterCopy.begin (), jitterCopy.end ());

  cout << fixed; cout.precision (3);

  double minJms = jitterCopy.empty () ? 0.0 : (double)jitterCopy.front () / 1000.0;
  double maxJms = jitterCopy.empty () ? 0.0 : (double)jitterCopy.back () / 1000.0;
  double sum = 0;
  for (size_t i = 0; i < jitterCopy.size (); i++)
    sum += (double)jitterCopy[i];
  double avgUs = jitterCopy.empty () ? 0.0 : sum / jitterCopy.size ();
  double stddevUs = computeStddev (jitterCopy, avgUs);
  double avgMs = avgUs / 1000.0;
  double stddevMs = stddevUs / 1000.0;

  int64_t p50Us = percentile (jitterCopy, 50);
  int64_t p90Us = percentile (jitterCopy, 90);
  int64_t p95Us = percentile (jitterCopy, 95);
  int64_t p99Us = percentile (jitterCopy, 99);

  uint32_t negCount = 0, posCount = 0, zeroCount = 0;
  for (size_t i = 0; i < jitterCopy.size (); i++)
  {
    if (jitterCopy[i] < 0) negCount++;
    else if (jitterCopy[i] > 0) posCount++;
    else zeroCount++;
  }

  cout << endl << "=== Timer Accuracy Results ===" << endl;
  cout << "  Total expected fires: " << totalExpectedFires << endl;
  cout << "  Actual fires:         " << actualFires << endl;
  cout << "  Lost fires:           "
       << (actualFires < totalExpectedFires ? totalExpectedFires - actualFires : 0) << endl;
  cout << "  Jitter (ms, deviation from expected):" << endl;
  cout << "    Count:  " << jitterCopy.size () << endl;
  cout << "    Min:    " << minJms << endl;
  cout << "    Max:    " << maxJms << endl;
  cout << "    Mean:   " << avgMs << endl;
  cout << "    StdDev: " << stddevMs << endl;
  cout << "    p50:    " << ((double)p50Us / 1000.0) << endl;
  cout << "    p90:    " << ((double)p90Us / 1000.0) << endl;
  cout << "    p95:    " << ((double)p95Us / 1000.0) << endl;
  cout << "    p99:    " << ((double)p99Us / 1000.0) << endl;
  cout << "  Direction:" << endl;
  cout << "    Early (neg): " << negCount << endl;
  cout << "    On time:     " << zeroCount << endl;
  cout << "    Late (pos):  " << posCount << endl;

  if (!jitterCopy.empty () && jitterCopy.size () <= 20)
  {
    cout << "  All jitter values:" << endl;
    for (size_t i = 0; i < jitterCopy.size (); i++)
      cout << "    [" << i << "] " << ((double)jitterCopy[i] / 1000.0) << " ms" << endl;
  }
  else if (!jitterCopy.empty ())
  {
    cout << "  Sample jitter (first 5, last 5):" << endl;
    for (size_t i = 0; i < 5 && i < jitterCopy.size (); i++)
      cout << "    [" << i << "] " << ((double)jitterCopy[i] / 1000.0) << " ms" << endl;
    if (jitterCopy.size () > 10)
      cout << "    ..." << endl;
    size_t start = jitterCopy.size () > 5 ? jitterCopy.size () - 5 : 0;
    for (size_t i = start; i < jitterCopy.size (); i++)
      cout << "    [" << i << "] " << ((double)jitterCopy[i] / 1000.0) << " ms" << endl;
  }

  return (actualFires == totalExpectedFires) ? 0 : 1;
}
