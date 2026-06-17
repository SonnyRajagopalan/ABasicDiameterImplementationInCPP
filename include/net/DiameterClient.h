#ifndef DIAMETER_CLIENT_H
#define DIAMETER_CLIENT_H

//#include "common/Types.h"
#include "base/DiameterPeer.h"
#include "ClientSocket.h"
#include "base/DiameterPeerDb.h"

class DiameterClient : public ClientSocket
{
 public:
  DiameterClient ();
  ~DiameterClient ();
  DiameterClient (std::string& _serverIpAddress);
  DiameterClient (std::string& _serverIpAddress, std::string& _description);

  BasicString getDescription () { return description; }
  void   setDescription (BasicString& _description) {description = _description; }

 private:
  BasicString description;
  DiameterPeerDb* pDiameterPeerDb;
  /**
     Functions needed for looking up a peer
  */
  //DiameterPeer* getDiameterPeerFromIPv4Address (BasicString& ipv4Addr);
};
#endif
