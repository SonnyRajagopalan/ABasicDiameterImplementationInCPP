  /**
     REFERENCES:
     ===========
     1. http://kovyrin.net/2006/04/13/epoll-asynchronous-network-programming/
     2. http://lse.sourceforge.net/epoll/index.html (Epoll scalability page)
     3. http://www.kernel.org/doc/man-pages/online/pages/man4/epoll.4.html (epoll man page)
     4. https://banu.com/blog/2/how-to-use-epoll-a-complete-example-in-c/


     A NOTE ON DESIGN CHOICES:
     =========================
     1. The choice for the right multiplexing solution (select vs. poll vs. epoll vs. libev)
        really just depends on what our expectation for the maximum number of FDs that will be
	serviced is. Epoll is not really efficient for sub-100s of FDs, but it is being used
	here, even though select is really the correct choice.
     2. I could have used Libev, but I am not sure about its interface. I find its interface 
        a little unintuitive, but definitely far better designed than Libevent, or Libevent2
	both of which are very cumbersome, and unintuitive. And yes, Libev certainly has
	much better documentation.
  */
#include "base/DiameterBaseTask.h"
#include "messages/DiameterHeader.h"
#include "net/SocketUtil.h"
#include <errno.h>

DiameterBaseTask::DiameterBaseTask(CHAR* _name)
{
  /**
     Base task (process) is the "owner" of the network sockets.
     1. Create PosixMessageQueue into which other processes will
        send messages intended to be processed by base process.
     2. Create and set up an epoll event for the Diameter server
        and the Posix message queue.
  */
  struct epoll_event newDiameterServerEvent, newMQEvent;

  pId = new TaskID (TaskID::DIAMETER_BASE_TASK);
  if ((epollFD = epoll_create (MAX_DIAMETER_CONNECTIONS)) == -1)
    {
      DIAMETERBASETASKLOG << LOGPRIORITY_CRITICAL << "Could not create the epoll FD list. Aborting!" << endl;
      assert (1);
    }

  pDiameterBaseTaskMsgQ = new PosixMessageQueue ((char *) "/DiameterBaseTask",
						 POSIX_MQ_CREATOR,
						 0,
						 POSIX_PERM_RW0RW0RW0,
						 MAX_MQ_SIZE,
						 MAX_MQ_MSG_SIZE);

  pDiameterPeerDb = DiameterPeerDb::getInstance ();
  
  if ((diameterServerFD = createAndBind (DIAMETER_PORT)) == -1)
    {
      DIAMETERBASETASKLOG << LOGPRIORITY_CRITICAL << "Cannot create socket for Diameter server! Aborting!" << endl;
      assert (1);
    }

  if (makePortForSocketReusable (diameterServerFD) == -1)
    {
      DIAMETERBASETASKLOG << LOGPRIORITY_CRITICAL << "Cannot set socket option to reusable. Aborting!" << endl;
      assert (1);
    }

  if (makeSocketNonBlocking (diameterServerFD) == -1)
    {
      DIAMETERBASETASKLOG << LOGPRIORITY_CRITICAL << "Could not set DIAMETER_PORT to be non-blocking. Aborting!" << endl;
      assert (1);
    }

  if (listen (diameterServerFD, MAX_DIAMETER_CONNECTIONS) == -1)
    {
      DIAMETERBASETASKLOG << LOGPRIORITY_CRITICAL << "Listen failed on DIAMETER_PORT. Aborting!" << endl;
      assert (1);
    }

//   pServer = new DiameterServer ();
//   pServer->setNonBlocking (true);
//   pServer->initialize ();

//   diameterServerFD = pServer->getSocketDescriptor ();

  newDiameterServerEvent.data.fd = diameterServerFD;
  newDiameterServerEvent.events = EPOLLIN | EPOLLRDHUP;

  if (epoll_ctl (epollFD, EPOLL_CTL_ADD, diameterServerFD, &newDiameterServerEvent) == -1)
    {
      DIAMETERBASETASKLOG << LOGPRIORITY_CRITICAL << "Could not add the diameter server FD to the epoll FD list. Aborting!" << endl;
      assert (1);
    }

  baseProcessMessageQFD = pDiameterBaseTaskMsgQ->getMessageQDescriptor ();
  newMQEvent.data.fd = baseProcessMessageQFD;
  newMQEvent.events = EPOLLIN | EPOLLRDHUP;

  if (epoll_ctl (epollFD, EPOLL_CTL_ADD, baseProcessMessageQFD, &newMQEvent) == -1)
    {
      DIAMETERBASETASKLOG << LOGPRIORITY_CRITICAL << "Could not add the base process message queue FD to the epoll FD list. Aborting!" << endl;
      assert (1);
    }
}

void DiameterBaseTask::run (void* pArg)
{
  UINT32 fdIndex = 0;
  struct epoll_event newEvent;

  while (1)
    {
      UINT32 numEvents = epoll_wait (epollFD, processableEvents, MAX_DIAMETER_CONNECTIONS, -1);
      cout << "numEvents = " << numEvents << endl;
      if (numEvents < 0)
	{
	  DIAMETERBASETASKLOG << LOGPRIORITY_CRITICAL << "Serious error in epoll setup: epoll_wait () returned < 0 status!" << endl;
	  assert (1);
	}

      for (fdIndex = 0; fdIndex < numEvents; fdIndex ++)
	{
	  // Catch the common error conditions
	  if ((processableEvents[fdIndex].events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) &&
	      (processableEvents[fdIndex].data.fd != diameterServerFD))
	    {
	      if (processableEvents[fdIndex].events & EPOLLRDHUP)
		{
		  //DIAMETERBASETASKLOG << LOGPRIORITY_INFO << "Client closed connection unexpectedly!" << endl;
		  close (processableEvents[fdIndex].data.fd);

		  cout << "Client closed connection unexpectedly" << endl;
		  continue;
		}
	    }

	  if (processableEvents[fdIndex].data.fd == diameterServerFD)
	    {
	      // Received a message from a new client; add this FD 
	      // to the epoll list being monitored for events
	      cout << "Received on DIAMETER_PORT" << endl;

	      while (1)
		{
		  SINT32 newClientSocketFD = -1;
		  UINT32 addressLength = sizeof (diameterServerSocketAddress);
		  BasicString ipv4Addr;
		  DiameterPeer* pPeer;

		  if ((newClientSocketFD = accept (diameterServerFD, (sockaddr *) &diameterServerSocketAddress, (socklen_t *) &addressLength)) == -1)
		    {		      
		      cout << "Return value from accept = " << newClientSocketFD << endl;
		      
		      if ((errno == EAGAIN) ||
			  (errno == EWOULDBLOCK))
			{
			  cout << "Finished...." << endl;
			  break;
			}
		      else
			{
			  cout << "Some other problem..." << endl;
			  break;
			}
		    }

		  if (makeSocketNonBlocking (newClientSocketFD) == -1)
		    {
		      DIAMETERBASETASKLOG << LOGPRIORITY_CRITICAL << "Could not make new client socket non blocking. Aborting!" << endl;
		      assert (1);
		    }

		  newEvent.events =  EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLRDHUP;
		  newEvent.data.fd = newClientSocketFD;
		  
		  if (epoll_ctl(epollFD, EPOLL_CTL_ADD, newClientSocketFD, &newEvent) == -1)
		    {
		      DIAMETERBASETASKLOG << LOGPRIORITY_CRITICAL << "Cannot add new client socket FD to epoll control! Aborting!" << endl;
		      assert (1);
		    }
		  
		  ipv4Addr = ::getPeerIPv4Address (newClientSocketFD);
		  pPeer = pDiameterPeerDb->getPeerFromIPv4 (ipv4Addr);
		  
		  if (pPeer == NULL)
		    {
		      //cout << "pPeer = NULL, creating a new peer entry" << endl;
		      //cout << "3..." << endl;
		      pPeer = new DiameterPeer (ipv4Addr, newClientSocketFD); // Calls the destructor and closes this socket!
		      //cout << "4..." << endl;
		      pDiameterPeerDb->insertDiameterPeerIntoDb (pPeer);
		      //cout << "5..." << endl;
		    }
		  else
		    {
		      // The peer exists. Receive a message from the peer and
		      // HandleDiameterMessage (buffer); which passes it on to the correct app
		      // after a whole load of validations. E.g. is the peer registered for 
		      // the app it is trying to access?
		    }
		} // End of while
	      continue;
	    }// End of received on server port
	  else if (processableEvents[fdIndex].data.fd == baseProcessMessageQFD) // Message received in the mq
	    {
	      // Received this message on the POSIX message queue,
	      // Could be from a timer or any other Diameter application task in the system
	      cout << "Received on MQ" << endl;
	      processMQ ();
	    }
	  else // From a known client
	    {
	      // Received a message from a known Diameter client.
	      cout << "PROCESS KNOWN CLIENT" << endl;
	      processKnownClient (fdIndex);
	    }	  
	} // End of fdIndex lopp
    } // End of while
}

void DiameterBaseTask::processMQ()
{  
  char mQMsg [MAX_MQ_MSG_SIZE];

  if (pDiameterBaseTaskMsgQ->receive () < 0)
    {
      DIAMETERBASETASKLOG << LOGPRIORITY_CRITICAL << "Problem reading from message queue. Aborting!" << endl;
      assert (1);
    }
  memcpy (mQMsg, pDiameterBaseTaskMsgQ->getMsg (), MAX_MQ_MSG_SIZE);
  cout << "Received " << mQMsg << " on MQ" << endl;

  // Now parse the message
}

void DiameterBaseTask::processNewClient()
{
}

void DiameterBaseTask::processKnownClient (UINT32 _fdIndex)
{
  vector<char> receivedMessage;
  BasicString ipv4Addr;
  stringstream in;
  SINT32 socketFD = processableEvents [_fdIndex].data.fd;
  DiameterPeer* pPeer = pDiameterPeerDb->getPeerFromSocketFD (socketFD);
  SINT32 knownClientSocketFD = -1;

  if (pPeer == NULL)
    {
      DIAMETERBASETASKLOG << LOGPRIORITY_CRITICAL << "Message received on a socket that should map to a peer in the PeerDB, but doesn't! Aborting!" << endl;
      assert (1);
    }
  else
    {
      // The peer exists. Receive a message from the peer and
      // HandleDiameterMessage (buffer); which passes it on to the correct app
      // after a whole load of validations. E.g. is the peer registered for 
      // the app it is trying to access?
      knownClientSocketFD = pPeer->getSocketFD ();
    }
  /*
    05/05/2012.
    The following is the method os extracing from Socket object, which was more cumbersome
    than just using the socket FDs directly. This was so much easier, and removed that layer of
    abstraction between the programming interface and the system calls performed on the sockets.

    Socket knownClientSocket (knownClientSocketFD);
    
    //const Socket& Socket::operator >> (std::vector<char>& s) const
    
    knownClientSocket >> receivedMessage; // Extract from the knownClientSocket into (vector) receivedMessage.
    cout << "Received message from peer of size " << receivedMessage.size() << endl;
    string rcvdMsgString (receivedMessage.begin(), receivedMessage.end());
    
    in << rcvdMsgString; // Place the received message in the input stream
*/
  if (readIntoInputStream (knownClientSocketFD, in) == -1)
    {
      DIAMETERBASETASKLOG << LOGPRIORITY_CRITICAL << "Could not read from client socket. Aborting!" << endl;
      assert (1);
    }

  deserialize (in); // std::stringstream inherits from both istream and ostream
}

DiameterBaseTask::~DiameterBaseTask()
{
}

void DiameterBaseTask::serialize (OutputStream& os)
{
}


void DiameterBaseTask::deserialize (InputStream& in)
{
  DiameterHeader header;
  header.deserialize (in);

  // Now you know what the message type is. Either process locally or send to application.
}
