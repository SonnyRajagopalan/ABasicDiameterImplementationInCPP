#include "string.h"
#include <string>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "net/Socket.h"
#include "net/SocketException.h"
#include "common/Types.h"
#include "common/Debug.h"

Socket::Socket() : socketFD (-1)
{
  memset (&socketAddress, 0, sizeof (socketAddress));
}

Socket::Socket (SINT32 _socketFD) : socketFD (_socketFD)
{
}

Socket::~Socket ()
{
  cout << "[" << getpid() << "] ~Socket fd=" << socketFD << endl;
}

void Socket::close ()
{
  if (isValid())
    {
      cout << "[" << getpid() << "] close fd=" << socketFD << endl;
      ::close (socketFD);
    }
}

Result Socket::create()
{
  socketFD = socket (AF_INET, SOCK_STREAM, 0);
  if (socketFD == -1)
    {
      cout << "[" << getpid() << "] socket() failed" << endl;
      return ERROR;
    }

  SINT32 reuse = 1;
  if (setsockopt (socketFD, SOL_SOCKET, SO_REUSEADDR, (const char*) &reuse, sizeof (reuse)) == -1)
    {
      cout << "[" << getpid() << "] setsockopt(SO_REUSEADDR) failed" << endl;
      return ERROR;
    }
  return OK;
}

Result Socket::bind (const SINT32 port)
{
  if (! isValid())
    return ERROR;

  socketAddress.sin_family = AF_INET;
  socketAddress.sin_addr.s_addr = INADDR_ANY;
  socketAddress.sin_port = htons (port);

  SINT32 retVal = ::bind (socketFD, (struct sockaddr *) &socketAddress, sizeof (socketAddress));
  if (retVal == -1)
    {
      cout << "[" << getpid() << "] bind port=" << port << " failed" << endl;
      return ERROR;
    }

  return OK;
}

Result Socket::listen () const
{
  if (! isValid())
    return ERROR;

  SINT32 retVal = ::listen (socketFD, MAXCONNECTIONS);
  if (retVal == -1)
    {
      cout << "[" << getpid() << "] listen failed" << endl;
      return ERROR;
    }

  return OK;
}

Result Socket::accept (Socket* newSocket) const
{
  UINT32 addrLength = sizeof (socketAddress);

  newSocket->socketFD = ::accept (socketFD, (sockaddr *) &socketAddress, (socklen_t *) &addrLength);
  if (newSocket->socketFD <= 0)
    {
      cout << "[" << getpid() << "] accept failed" << endl;
      return ERROR;
    }

  return OK;
}

Result Socket::send (const vector<char>& message) const
{
  SINT32 status = ::send (socketFD, &message[0], message.size(), 0);
  if (status == -1)
    {
      cout << "[" << getpid() << "] send fd=" << socketFD << " failed" << endl;
      return ERROR;
    }

  return OK;
}

SINT32 Socket::recv (vector<char>& message) const
{    
  vector<char> buffer(MAXRECV);

  SINT32 status = ::recv (socketFD, &buffer[0], buffer.size(), 0);

  if (status < 0) 
    {
      cout << "[" << getpid() << "] recv fd=" << socketFD << " failed" << endl;
      return -1;      
    }
  else if (status == 0)
    {
      return 0;
    }
  else
    {
      buffer.resize(status);
      message.swap(buffer);
      return status;      
    }
}


Result Socket::connect (const BasicString host, const SINT32 port)
{
  cout << "Socket::connect () called" << endl;
  if (! isValid()) return ERROR;

  socketAddress.sin_family = AF_INET;
  socketAddress.sin_port = htons (port);

  SINT32 status = inet_pton (AF_INET, host.c_str(), &socketAddress.sin_addr);

  if (errno == EAFNOSUPPORT) return ERROR;

  status = ::connect (socketFD, (sockaddr *) &socketAddress, sizeof (socketAddress));

  if (status == 0)
    return OK;
  else
    {
      cout << "Could not Socket::connect ()" << endl;
      return ERROR;
    }
}

void Socket::setNonBlocking (const bool b)
{
  SINT32 opts;

  opts = fcntl (socketFD, F_GETFL);

  if (opts < 0)
    {
      return;
    }

  if (b)
    opts = (opts | O_NONBLOCK);
  else
    opts = (opts & ~O_NONBLOCK);

  fcntl (socketFD, F_SETFL, opts);
}

BasicString Socket::getPeerIPv4Address ()
{
  BasicString address;
  char cStyleAddr[INET_ADDRSTRLEN];
  sockaddr_in addr;
  unsigned int addrLength = sizeof(addr);

  if (getpeername(socketFD, (sockaddr *) &addr,(socklen_t *) &addrLength) < 0) 
    {
      cout << "Error: cannot get foreign address" << endl;
    }

  strcpy(cStyleAddr, inet_ntoa(addr.sin_addr));
  address.assign (cStyleAddr);

  return address;
}

const Socket& Socket::operator << (const std::ostringstream& os) const
{
  std::stringbuf* pSBuf = os.rdbuf();
  std::vector<char> buffer (pSBuf->str().size());
  memcpy (&buffer[0], pSBuf->str().c_str(), pSBuf->str().size());

  if (Socket::send (buffer) != OK)
    {
      throw SocketException ("Could not write to socket.");
    }

  return *this;

}

const Socket& Socket::operator << (const std::vector<char>& s) const
{
  if (Socket::send (s) != OK)
    {
      throw SocketException ("Could not write to socket.");
    }

  return *this;
}

const Socket& Socket::operator >> (std::vector<char>& s) const
{
  if (! Socket::recv (s))
    {
      throw SocketException ("Could not read from socket.");
    }

  return *this;
}
