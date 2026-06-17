#include "net/ClientSocket.h"
#include "net/SocketException.h"
#include "common/Types.h"

ClientSocket::ClientSocket (BasicString host, int port)
{
  if (Socket::create() != OK)
    {
      throw SocketException ("Could not create client socket.");
    }

  if (Socket::connect (host, port) != OK)
    {
      throw SocketException ("Could not bind to port.");
    }

}


// const ClientSocket& ClientSocket::operator << (const std::vector<char>& s) const
// {
//   if (Socket::send (s) != OK)
//     {
//       throw SocketException ("Could not write to socket.");
//     }

//   return *this;

// }


// const ClientSocket& ClientSocket::operator >> (std::vector<char>& s)
// {
//   if (! Socket::recv (s))
//     {
//       throw SocketException ("Could not read from socket.");
//     }

//   return *this;
// }
