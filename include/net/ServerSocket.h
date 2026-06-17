#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include "Socket.h"

/**
   The following class inherited privately from Socket before, but
   we need the methods in Socket in DiameterServer, so it
   was made a public interface
*/
class ServerSocket : public Socket
{
 public:

  ServerSocket (SINT32 port);
  ServerSocket ();
  virtual ~ServerSocket();

/*   const ServerSocket& operator << (const std::vector<char>&) const; */
/*   const ServerSocket& operator >> (std::vector<char>&); */

  //void accept (Socket&);
};

#endif
