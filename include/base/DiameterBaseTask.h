#ifndef DIAMETER_BASE_TASK_H
#define DIAMETER_BASE_TASK_H

#include <assert.h>
#include <sys/epoll.h>
#include "platform/PosixMessageQueue.h"
#include "platform/Runnable.h"
#include "platform/TaskID.h"
#include "logging/DiameterBaseTaskDiagnosticLog.h"
#include "net/DiameterServer.h"
#include "base/DiameterPeerDb.h"

#define DIAMETERBASETASKLOG (DiameterBaseTaskDiagnosticLog::getInstance ())
#define MAX_MESSAGE_QUEUE_NAME_LENGTH 30
#define MAX_DIAMETER_CONNECTIONS 1024

class DiameterBaseTask : public Runnable
{
 public:
  DiameterBaseTask (CHAR* _name);

  virtual void initialize (void* pArg) {};
  virtual void run (void* pArg);
  virtual void exit (void* pArg) {};

 private:
  // Methods
  void processMQ ();
  void processKnownClient (UINT32 _fdIndex);
  void processNewClient ();

  void serialize (OutputStream& os);
  void deserialize (InputStream& in);

  ~DiameterBaseTask (); // Don't want any other object deleting this object
  DiameterBaseTask& operator= (const DiameterBaseTask& _that)
    {
      DIAMETERBASETASKLOG << LOG_ERR << "Cannot copy DiameterBaseTask task!" << endl;
      assert (1);
    }  
  // Attributes
  TaskID* pId;
  CHAR name [MAX_MESSAGE_QUEUE_NAME_LENGTH];
  PosixMessageQueue* pDiameterBaseTaskMsgQ;
  SINT32 epollFD;
  SINT32 diameterServerFD;
  sockaddr_in diameterServerSocketAddress;
  SINT32 baseProcessMessageQFD;
  struct epoll_event processableEvents [MAX_DIAMETER_CONNECTIONS];
  //DiameterServer* pServer;  
  DiameterPeerDb* pDiameterPeerDb;
};
#endif
