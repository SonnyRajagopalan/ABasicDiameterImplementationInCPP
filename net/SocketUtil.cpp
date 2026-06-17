#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h> // For inet_ntoa
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <string.h>
#include <sstream>
#include <vector>
#include "net/SocketUtil.h"
#include "common/Debug.h"

SINT32 createAndBind (SINT32 _port)
{
  SINT32 socketFD = -1;
  sockaddr_in socketAddress;
  SINT32 retVal = -1;

  memset (&socketAddress, 0, sizeof (socketAddress));

  if ((socketFD = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      return socketFD;
    }

  socketAddress.sin_family      = AF_INET;
  socketAddress.sin_addr.s_addr = INADDR_ANY;
  socketAddress.sin_port        = htons (_port);
  
  if ((retVal = bind (socketFD, (struct sockaddr *) & socketAddress, sizeof (socketAddress))) == -1)
    {
      return retVal;
      close (socketFD);
    }

  return socketFD;
}

SINT32 makeSocketNonBlocking (SINT32 _socketFD)
{
  SINT32 socketFlags = fcntl (_socketFD, F_GETFL, 0);
  if (socketFlags == -1)
    {
      return -1;
    }

  socketFlags |= O_NONBLOCK;
  if (fcntl (_socketFD, F_SETFL, socketFlags) == -1)
    {
      return -1;
    }
  return 0;
}

SINT32 makePortForSocketReusable (SINT32 _socketFD)
{
  SINT32 reuseAddr = 1;
  if (setsockopt (_socketFD, SOL_SOCKET, SO_REUSEADDR, (const char*) &reuseAddr, sizeof (reuseAddr)) == -1)
    {
      return -1;
    }
  return 0;
}

BasicString getPeerIPv4Address (SINT32 _socketFD)
{
  BasicString address;
  char cStyleAddr[INET_ADDRSTRLEN];
  sockaddr_in addr;
  unsigned int addrLength = sizeof(addr);

  if (getpeername(_socketFD, (sockaddr *) &addr,(socklen_t *) &addrLength) < 0) 
    {
      cout << "Error: cannot get foreign address" << endl;
    }

  strcpy(cStyleAddr, inet_ntoa(addr.sin_addr));
  address.assign (cStyleAddr);

  return address;
}

//TBD:
SINT32 readIntoInputStream (SINT32 _socketFD, std::stringstream& _in)
{
  vector<char> buffer(MAXRECV);
  SINT32 readStatus = -1;
  readStatus = recv (_socketFD, &buffer[0], buffer.size(), 0);
  if (readStatus < 0)
    {
      cout << "Could not receive from socketFD" << endl;
      return -1;
    }
  else if (readStatus == 0)
    {
      return 0;
    }
  else
    {
      buffer.resize (readStatus);
      string receivedMessage (buffer.begin (), buffer.end ());
      _in << receivedMessage;

      return readStatus;
    }
  // Can never get here
  return 0;
}

SINT32 sendFromOutputStream (SINT32 _socketFD, std::stringstream& _os)
{
  SINT32 sendStatus = -1;
  std::stringbuf* pSBuf = _os.rdbuf();
  std::vector<char> buffer (pSBuf->str().size());
  memcpy (&buffer[0], pSBuf->str().c_str(), pSBuf->str().size());

  sendStatus = send (_socketFD, &buffer[0], buffer.size(), 0);
  if (sendStatus == -1)
    {
      // cout << "Could not send" << endl;
      return -1;
    }
  else
    {
      return 0;
    }
  // Can never get here
  return 0;
}
