#include <iostream>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string>
#include <string.h>
#include "platform/PosixSemaphore.h"

PosixSemaphore::PosixSemaphore (CHAR* _name, UINT32 _flags, UINT32 _mode, UINT32 _value):
  flags (_flags),
  mode (_mode),
  value (_value)

{
  strcpy (name, _name);
  semaphore = sem_open (name, flags, mode, value);
  if (semaphore == SEM_FAILED && (flags & O_EXCL) && errno == EEXIST)
    {
      flags &= ~O_EXCL;
      semaphore = sem_open (name, flags, mode, value);
    }
  if (semaphore == SEM_FAILED)
    {
      std::cout << "[" << getpid () << "] Fatal: PosixSemaphore (" << name << ") failed: errno=" << errno << " (" << strerror(errno) << ")" << std::endl;
      abort ();
    }
}

PosixSemaphore::~PosixSemaphore ()
{
  sem_close (semaphore);
}
