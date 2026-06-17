#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "platform/MemoryPool.h"

MemoryPool::MemoryPool (CHAR* _name, CHAR* _pathname, UINT32 _createFlag, UINT32 _mapFlag, UINT32 _mode,
			UINT32 _objectSize, UINT32 _maxNumberOfObjects) :
  createFlag (_createFlag),
  mapFlag (_mapFlag),
  mode (_mode),
  objectSize (_objectSize),
  maxNumberOfObjects (_maxNumberOfObjects)
{
  CHAR* byteifiedPShm;

  sizeOfSharedManagementArea = 
    (sizeof (UINT32)) + // Number of objects currently allocated
    (sizeof (UINT64)) + // First free location in memory pool
    (sizeof (UINT64) * _maxNumberOfObjects); // Total space reserved for the freelist management
  
  totalSize = sizeOfSharedManagementArea + (maxNumberOfObjects * objectSize);
  strcpy (name, _name);
  strcpy (pathname, _pathname);
  pShm = new PosixSharedMemory (pathname, createFlag, mode, mapFlag, totalSize);
  if (pShm->getSharedMem() == NULL)
    {
      std::cout << "[" << getpid() << "] Fatal: MemoryPool failed to map shared memory for " << name << " (" << pathname << ")" << std::endl;
      abort();
    }
      
  /**
     Set up the shared management area--critical for consistent cross-proces pool management!
  */
  
  byteifiedPShm = reinterpret_cast<char*> (pShm->getSharedMem ());
  pNumberAllocated = reinterpret_cast<UINT32*> (byteifiedPShm);
  pFirstFree       = reinterpret_cast<UINT64*> (byteifiedPShm + sizeof (UINT32));
  pFreeList        = reinterpret_cast<UINT64*> (byteifiedPShm + sizeof (UINT32) + sizeof (UINT64));
  
  if (createFlag & O_EXCL) // The following is done only for the creator of the memory pool
    {
      *pNumberAllocated = 0;
      pFirstFree->uint64 = 0;
      for (UINT32 i = 0; i < maxNumberOfObjects; i++)
	{
	  pFreeList[i].uint64 = i + 1;
	}
    }
}

MemoryPool::~MemoryPool ()
{
  delete pShm;
}

void* MemoryPool::getFromPool ()
{
  void* thisObject;
  UINT64 next = pFreeList [pFirstFree->uint64];

  if (*pNumberAllocated == maxNumberOfObjects)
    {
      return NULL;
    }

  thisObject = getMemoryLocationFromIndex (*pFirstFree);
  *pFirstFree = next;
  (*pNumberAllocated) ++;

  return thisObject;
}

void MemoryPool::removeFromPool (void* object)
{
  UINT64 index = getIndexFromMemoryLocation (object);
  if (*pNumberAllocated == 0)
    {
      return;
    }
  pFreeList [index.uint64] = *pFirstFree;
  *pFirstFree = index;

  (*pNumberAllocated) --;
}

void* MemoryPool::getMemoryLocationFromIndex (UINT64 index)
{
  void* location = NULL;
  char* byteifiedPShm = reinterpret_cast<char*> (pShm->getSharedMem ());

  location = reinterpret_cast<void*> (byteifiedPShm + sizeOfSharedManagementArea + (index.uint64 * objectSize));

  return location;

}

UINT64 MemoryPool::getIndexFromMemoryLocation(void* location)
{
  UINT64 index;
  char* byteifiedPShm = reinterpret_cast<char*> (pShm->getSharedMem ());
  char* byteifiedLocn = reinterpret_cast<char*> (location);

  index.uint64 = (byteifiedLocn - byteifiedPShm - sizeOfSharedManagementArea)/objectSize;

  if (index.uint64 < 0)
    {
      std::cout << "Trying to delete an object not in the pool. Error!" << std::endl;
      location = 0;
    }

  return index;
}
