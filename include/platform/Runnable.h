#ifndef RUNNABLE_H
#define RUNNABLE_H

#include "common/Types.h"

class Runnable
{
 public:

  Runnable ();
  ~Runnable ();

  UINT32 getNumberOfRunnables () { return numberOfRunnables; }

  virtual void initialize (void* pArg) {};
  virtual void run (void* pArg) = 0;
  virtual void exit (void* pArg) {};

 private:

  static UINT32 numberOfRunnables;
};
#endif
