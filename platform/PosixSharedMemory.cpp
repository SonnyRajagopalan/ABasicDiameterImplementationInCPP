#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "platform/PosixSharedMemory.h"
#include <errno.h>

PosixSharedMemory::PosixSharedMemory (CHAR* _pathname, UINT32 _createFlag, UINT32 _mode, UINT32 _mapFlag, UINT32 _size) :
  createFlag (_createFlag),
  mode (_mode),
  mapFlag (_mapFlag),
  size (_size)
{
  bool triedExclusive = false;
  bool creator = false;

  strcpy (pathname, _pathname);
  shmFd = shm_open (pathname, createFlag, mode);

  if (shmFd == -1 && (createFlag & O_EXCL))
    {
      if (errno == EEXIST)
        {
          triedExclusive = true;
          shmFd = shm_open (pathname, (createFlag & ~O_EXCL), mode);
        }
    }

  if (shmFd == -1)
    {
      std::cout << "[" << getpid () << "] Failure opening POSIX shared memory location (" << pathname << ")" 
		<< " with createFlag = " << std::hex << createFlag << std::oct << ", and mode = " << mode << std::dec
		<< ". errno=" << errno << " (" << strerror(errno) << ")" << std::endl;
      sharedMem = NULL;
      return;
    }

  if ((createFlag & O_CREAT) && !triedExclusive)
    creator = true;

  if (creator)
    {
      if (ftruncate (shmFd, size) == -1)
        {
          std::cout << "[" << getpid() << "] ftruncate failed for " << pathname << ": errno=" << errno << " (" << strerror(errno) << ")" << std::endl;
          close(shmFd);
          sharedMem = NULL;
          return;
        }
    }

  sharedMem = mmap (NULL, size, mapFlag, MAP_SHARED, shmFd, 0);
  if (sharedMem == MAP_FAILED)
    {
      std::cout << "[" << getpid() << "] mmap failed for " << pathname << ": errno=" << errno << " (" << strerror(errno) << ")" << std::endl;
      sharedMem = NULL;
      close(shmFd);
      return;
    }

  shm_created_by_me = creator;
}

PosixSharedMemory::~PosixSharedMemory ()
{
  if (sharedMem)
    munmap (sharedMem, size);

  if (shm_created_by_me)
    {
      shm_unlink (pathname);
    }
}
