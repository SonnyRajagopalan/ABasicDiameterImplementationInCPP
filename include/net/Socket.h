#ifndef SOCKET_H
#define SOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>
#include <vector>
#include <iostream>
#include "common/Types.h"

const SINT32 MAXHOSTNAME = 200;
const SINT32 MAXCONNECTIONS = 16;
const SINT32 MAXRECV = 8192; // 8K
//const SINT32 MSG_NOSIGNAL = 0; // defined by dgame

class Socket
{
 public:
  Socket ();
  Socket (SINT32 _socketFD);
  virtual ~Socket(); // You want the derived classes to call their destructors too
  Socket (const Socket& anotherSocket) // Copy constructor
    {
      socketFD = anotherSocket.socketFD;
      socketAddress = anotherSocket.socketAddress;
    }
  // Server initialization
  Result create ();
  Result bind (const SINT32 port);
  Result listen () const;
  Result accept (Socket* peerSocket) const;
  // Client initialization
  Result connect (const BasicString host, const SINT32 port);
  // Data Transimission
  Result send (const std::vector<char>& message) const;
  SINT32 recv (std::vector<char>& message) const;
  void setNonBlocking (const bool);
  //Validators
  bool isValid () const { return socketFD != -1; }
  // Getters
  BasicString getPeerIPv4Address ();
  SINT32 getSocketDescriptor () const { return socketFD; }
  // Close the socket
  void close ();
  // Overloaded stream extraction/insertion operators
  const Socket& operator << (const std::ostringstream&) const;
  const Socket& operator << (const std::vector<char>& s) const;
  const Socket& operator >> (std::vector<char>&) const;

 private:
  SINT32 socketFD;
  sockaddr_in socketAddress;
};


#endif
