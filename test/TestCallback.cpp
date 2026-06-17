#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include "platform/TimerConstants.h"
#include "test/TestCallback.h"

MemoryPool* TestCallback::testCallbackPool =  new MemoryPool ((char*) "testCallbackPool",
							      (char*) "/testCallbackPool",
							      O_RDWR | O_CREAT | O_EXCL, PROT_READ | PROT_WRITE, 0666, 
							      sizeof (TestCallback), MAX_NUMBER_OF_TIMERS);

PosixSemaphore* TestCallback::pTestCallbackSemaphore = new PosixSemaphore ((char*) "/testCallbackSemaphore", 
									   O_CREAT | O_RDWR | O_EXCL, 
									   0666, 
									   1);
TestCallback::TestCallback ()
{
}

void TestCallback::triggered ()
{
  return;
}

TestCallback::~TestCallback ()
{
}

void* TestCallback::operator new (size_t size)
{
  void* location = NULL;

  pTestCallbackSemaphore->wait ();

  location = testCallbackPool->getFromPool ();

  pTestCallbackSemaphore->post ();

  return location;
}

void TestCallback::operator delete (void* object, size_t size)
{
  pTestCallbackSemaphore->wait ();

  testCallbackPool->removeFromPool (object);

  pTestCallbackSemaphore->post ();
}
