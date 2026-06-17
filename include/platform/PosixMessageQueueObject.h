#ifndef POSIX_MESSAGE_QUEUE_OBJECT_H
#define POSIX_MESSAGE_QUEUE_OBJECT_H
#include "common/Types.h"
#include "platform/TaskID.h"
#include "common/Util.h"

class PosixMessageQueueObject
{
 public:

  PosixMessageQueueObject () {};
  PosixMessageQueueObject (TaskID _from, EventID _event, UINT64 _shmIndex) :
    from (_from),
    event (_event),
    shmIndex (_shmIndex)
    {
    }
  inline void serializeToCharBuf (CHAR* buf) const;
  inline void deserializeFromCharBuf (const CHAR* buf);
  void   setTaskID (UINT32 _id) { from.setID ((TaskID::ID)(_id)); }
  TaskID getTaskID () const { return from; }
  void   setShmIndex (const UINT64& _shmIndex) { shmIndex = _shmIndex; }
  UINT64 getShmIndex () const { return shmIndex; }

 private:

  TaskID from;
  EventID event; 
  UINT64 shmIndex;
};

void PosixMessageQueueObject::serializeToCharBuf (CHAR* buf) const
{
  //buf = new CHAR [sizeof (TaskID) + sizeof (EventID) + sizeof (UINT64)]; Should be initialied in the caller!

  ::serializeUINT32ToCharBuf (from.getID (), buf);
  ::serializeUINT32ToCharBuf (event, (buf + sizeof (UINT32)));
  ::serializeUINT64ToCharBuf (shmIndex, (buf + sizeof (UINT32) + sizeof (UINT32)));
}

void PosixMessageQueueObject::deserializeFromCharBuf (const CHAR* buf)
{
  UINT32 _id;
  UINT32 _event;
  UINT64 _shmIndex;

  ::deserializeUINT32FromCharBuf (_id, buf);
  ::deserializeUINT32FromCharBuf (_event, (buf + sizeof (UINT32)));
  ::deserializeUINT64FromCharBuf (_shmIndex, (buf + sizeof (UINT32) + sizeof (UINT32)));

  setTaskID (_id);
  setShmIndex (_shmIndex);
}
#endif
