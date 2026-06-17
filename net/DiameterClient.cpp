#include <string>
#include <iostream>
#include <sstream>
#include "net/ClientSocket.h"
#include "net/SocketException.h"
#include "common/Types.h"
#include "base/DiameterPeer.h"
#include "base/DiameterPeerDb.h"
#include "net/DiameterClient.h"

DiameterClient::DiameterClient (BasicString& _serverIpAddress) : ClientSocket (_serverIpAddress, DIAMETER_PORT)
{
  pDiameterPeerDb = DiameterPeerDb::getInstance ();
}

DiameterClient::~DiameterClient ()
{
}

DiameterClient::DiameterClient (BasicString& _serverIpAddress, BasicString& _description) : 
  ClientSocket (_serverIpAddress, DIAMETER_PORT),
  description (_description)
{
  pDiameterPeerDb = DiameterPeerDb::getInstance ();
}
