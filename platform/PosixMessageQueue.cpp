#include <iostream>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include "platform/PosixMessageQueue.h"

PosixMessageQueue::PosixMessageQueue (CHAR* _name, SINT32 _flags, SINT32 _nonBlockingOrZero, UINT32 _mode, UINT32 _maxMsg, UINT32 _msgSize):
  flags (_flags),
  mode (_mode),
  created_by_me (false)
{
  strcpy (name, _name);
  mQAttr.mq_flags = _nonBlockingOrZero;
  mQAttr.mq_maxmsg = _maxMsg;
  mQAttr.mq_msgsize = _msgSize;
  mQAttr.mq_curmsgs = 0;

  pBuffer = (new char [mQAttr.mq_msgsize]);

  if ((msgFD = mq_open (name, (int) (flags), (mode_t) (mode), &mQAttr)) != -1)
    {
      created_by_me = ((flags & O_EXCL) && (flags & O_CREAT)) ? true : false;
    }
  else
    {
      if ((flags & O_EXCL) && errno == EEXIST)
        {
          flags &= ~O_EXCL;
          if ((msgFD = mq_open (name, (int) (flags), (mode_t) (mode), &mQAttr)) != -1)
            {
            }
          else
            {
              std::cout << "[" << getpid() << "] MQ " << name << " open failed: errno=" << errno << " (" << strerror(errno) << ")" << std::endl;
              assert (0);
            }
        }
      else
        {
          std::cout << "[" << getpid() << "] MQ " << name << " create failed: errno=" << errno << " (" << strerror(errno) << ")" << std::endl;
          assert (0);
        }
    }
}

PosixMessageQueue::~PosixMessageQueue ()
{
  delete [] pBuffer;

  if (mq_close (msgFD) == -1)
    {
      std::cout << "POSIX message queue (" << name << ") could not be closed properly: errno=" << errno << " (" << strerror(errno) << ")" << std::endl;
    }

  if (created_by_me)
    {
      if (mq_unlink (name) == -1)
        {
          std::cout << "POSIX message queue (" << name << ") could not be unlinked properly: errno=" << errno << " (" << strerror(errno) << ")" << std::endl;
        }
    }
}

void PosixMessageQueue::send (const CHAR* buf, const UINT32 msgSize)
{
  SINT32 retVal = 0;

  if ((retVal = mq_send (msgFD, buf, msgSize, 0)) == -1)
    {
      std::cout << "mq_send failed for " << name << ": errno=" << errno << " (" << strerror(errno) << ")" << std::endl;
    }
}

SINT32 PosixMessageQueue::receive ()
{
  SINT32  received = 0;

  memset (pBuffer, 0, mQAttr.mq_msgsize);
  if ((received = mq_receive (msgFD, pBuffer, mQAttr.mq_msgsize, NULL)) == -1)
    {
      if (errno != EAGAIN)
        std::cout << "mq_receive failed for " << name << ": errno=" << errno << " (" << strerror(errno) << ")" << std::endl;
    }

  return received;
}

UINT32 PosixMessageQueue::getNumberOfMessagesInQueue ()
{
  return (UINT32) mQAttr.mq_curmsgs;
}

void PosixMessageQueue::setNonBlockingMode ()
{
  mq_attr newAttr = mQAttr;

  newAttr.mq_flags |= O_NONBLOCK;
  if (mq_setattr (msgFD, &newAttr, &mQAttr) == -1)
    {
      if (errno == EBADF)
	{
	}
      else if (errno == EINVAL)
	{
	}
    }
}
