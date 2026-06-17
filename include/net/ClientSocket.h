#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H

#include "Socket.h"

// Keeping this private means a class accessing the
// ClientSocket object will not be able to write to 
// Socket or read from socket using the operator>> and
// operator <<.
//class ClientSocket : private Socket
class ClientSocket : public Socket
{
 public:

  ClientSocket (BasicString host, int port);
  virtual ~ClientSocket(){};

/*   const ClientSocket& operator << (const std::vector<char>&) const; */
/*   const ClientSocket& operator >> (std::vector<char>&); */
};

#endif
