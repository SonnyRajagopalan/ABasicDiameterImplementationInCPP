#ifndef SOCKET_UTIL_H
#define SOCKET_UTIL_H

#include "common/Types.h"

#define MAXRECV 8192 // Redefinition in Socket.h

SINT32 createAndBind (SINT32 _port);
SINT32 makeSocketNonBlocking (SINT32 _socketFD);
SINT32 makePortForSocketReusable (SINT32 _socketFD);
BasicString getPeerIPv4Address (SINT32 _socketFD);
//TBD:
SINT32 readIntoInputStream (SINT32 _socketFD, std::stringstream& _in);
SINT32 sendFromOutputStream (SINT32 _socketFD, std::stringstream& _os);
#endif
