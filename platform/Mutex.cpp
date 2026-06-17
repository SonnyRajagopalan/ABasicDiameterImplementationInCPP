#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include "platform/Mutex.h"
#include "common/Types.h"

Mutex::Mutex ()
{
  if (pthread_mutex_init (&mutex, NULL) < 0)
    std::cout << "[" << getpid() << "] Mutex init failed" << std::endl;
}

Mutex::Mutex (MutexType _type, ProcessSharedAttrType _psatype)
{
  if (pthread_mutexattr_init (&mutexAttr) < 0)
    {
      std::cout << "[" << getpid() << "] Mutex attr init failed" << std::endl;
      return;
    }

  switch (_type)
    {
    case NORMAL:
      pthread_mutexattr_settype (&mutexAttr, PTHREAD_MUTEX_NORMAL);
      break;
    case ERRORCHECK:
      pthread_mutexattr_settype (&mutexAttr, PTHREAD_MUTEX_ERRORCHECK);
      break;
    case RECURSIVE:
      pthread_mutexattr_settype (&mutexAttr, PTHREAD_MUTEX_RECURSIVE);
      break;
    default:
      pthread_mutexattr_settype (&mutexAttr, PTHREAD_MUTEX_DEFAULT);
      break;
    }

  switch (_psatype)
    {
    case SHARED:
      pthread_mutexattr_setpshared (&mutexAttr, PTHREAD_PROCESS_SHARED);
      break;
    default:
      pthread_mutexattr_setpshared (&mutexAttr, PTHREAD_PROCESS_PRIVATE);
      break;
    }

  if (pthread_mutex_init (&mutex, &mutexAttr) < 0)
    std::cout << "[" << getpid() << "] Mutex init (typed) failed" << std::endl;
}

Mutex::~Mutex ()
{
  pthread_mutex_destroy (&mutex);
}

void Mutex::lock ()
{
  if (pthread_mutex_lock (&mutex) < 0)
    std::cout << "[" << getpid() << "] Mutex lock failed" << std::endl;
}

void Mutex::unlock ()
{
  if (pthread_mutex_unlock (&mutex) < 0)
    std::cout << "[" << getpid() << "] Mutex unlock failed" << std::endl;
}

void Mutex::yield ()
{
  if (pthread_yield () < 0)
    std::cout << "[" << getpid() << "] pthread_yield failed" << std::endl;
}

void Mutex::destroy ()
{
  pthread_mutex_destroy (&mutex);
}
