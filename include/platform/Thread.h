#ifndef THREAD_H
#define THREAD_H

#include "common/Types.h"
#include "Runnable.h"
#include <pthread.h>
#include <iostream>

class Thread
{
 public:
  
  enum ThreadType
    {
      JOINABLE,
      DETACHED,
    };

  Thread (Runnable* _task, ThreadType _type);
  ~Thread ();
  Runnable* getTask () { return task; }
  void printDebug () { std::cout << "From " << thread << std::endl; }
  //protected:

  Result create ();
  Result join ();
  Result exit ();
  Runnable* task; // Need access from derived classes
  
 private:

  pthread_t thread;
  pthread_attr_t attr;
  ThreadType type;
  static void* internalThreadEntryFunction (void* pArg);
};
#endif
