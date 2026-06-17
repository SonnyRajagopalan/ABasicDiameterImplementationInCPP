#ifndef DIAMETER_SERVER_H
#define DIAMETER_SERVER_H

#include "common/Types.h"
#include "base/DiameterPeer.h"
#include "ServerSocket.h"
#include "base/DiameterPeerDb.h"

class DiameterServer : public ServerSocket
{
 public:
  DiameterServer ();
  ~DiameterServer ();
  DiameterServer (std::string& _description);

  Result initialize ();
  Result run ();

  UINT32 getNumberOfPeers () { return numberOfPeersConnected; }
  BasicString getDescription () { return description; }

  void   incNumberOfPeers () { numberOfPeersConnected ++; }
  void   setDescription (BasicString& _description) { description = _description; }
  void   serialize (OutputStream& os);
  void   deserialize (InputStream& in);
 private:
  BasicString description;
  UINT32 numberOfPeersConnected;
  DiameterPeerDb* pDiameterPeerDb;
  /**
     Functions needed for looking up a peer
  */
  //DiameterPeer* getDiameterPeerFromIPv4Address (BasicString& ipv4Addr);
};
#endif
