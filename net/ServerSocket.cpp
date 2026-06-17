#include <iostream>
#include "net/ServerSocket.h"
#include "net/SocketException.h"
#include "common/Debug.h"

ServerSocket::ServerSocket ()
{
}

ServerSocket::ServerSocket (SINT32 port)
{
  cout << "[" << getpid() << "] ServerSocket port=" << port << endl;

  if (Socket::create() != OK)
    throw SocketException ("Could not create server socket.");

  if (Socket::bind (port) != OK)
    throw SocketException ("Could not bind to port.");

  if (Socket::listen() != OK)
    throw SocketException ("Could not listen to socket.");
}

ServerSocket::~ServerSocket()
{
  cout << "[" << getpid() << "] ~ServerSocket" << endl;
}
