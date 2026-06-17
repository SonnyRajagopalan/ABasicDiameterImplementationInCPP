#include <iostream>
#include <unistd.h>
#include "platform/Thread.h"
#include "platform/Runnable.h"

Thread::Thread (Runnable* _task, ThreadType _type) : 
  task (_task),
  type (_type)
{
  pthread_attr_init (&attr);
  if (type == JOINABLE)
    pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_JOINABLE);
  else
    pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);
}

Thread::~Thread ()
{
  if (type == JOINABLE)
    {
      join ();
      exit ();
    }
  else
    {
      exit ();
    }
}

Result Thread::create ()
{
  UINT32 retVal = pthread_create (&thread, &attr, &internalThreadEntryFunction, ((void *)this));
  if (retVal < 0)
    {
      std::cout << "[" << getpid() << "] Thread create failed" << std::endl;
      return ERROR;
    }

  pthread_attr_destroy (&attr);
  
  if (type == DETACHED)
    {
      retVal = pthread_detach (thread);
      if (retVal < 0)
	std::cout << "[" << getpid() << "] Thread detach failed" << std::endl;
    }

  return OK;
}

Result Thread::join ()
{
  if (type == DETACHED)
    {
      std::cout << "[" << getpid() << "] Cannot join detached thread" << std::endl;
      return ERROR;
    }

  if (pthread_join(thread, NULL) < 0)
    return ERROR;

  return OK;
}

Result Thread::exit ()
{
  pthread_exit (NULL);
  return OK;
}

void* Thread::internalThreadEntryFunction (void* pArg)
{
  Thread* pThr = (Thread*) pArg;
  ((Runnable*) (pThr->getTask()))->run (pArg);
  return NULL;
}
