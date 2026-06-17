#ifndef DIAMETER_PEER_DB_H
#define DIAMETER_PEER_DB_H

#include <map>
#include "common/Types.h"
//#include "DiameterPeerDb.h" Hmmmm???

class DiameterPeer;

class DiameterPeerDb
{
 public:
  ~DiameterPeerDb();
  Result init () const;
  Result insertDiameterPeerIntoDb (DiameterPeer* pPeer);
  Result removeDiameterPeerFromDb (DiameterPeer* pPeer);
  DiameterPeer* getPeerFromUUID (BasicString& _uuid);
  DiameterPeer* getPeerFromIPv4 (BasicString& _ipv4Address);
  DiameterPeer* getPeerFromSocketFD (SINT32& _socketFD);
  static DiameterPeerDb* getInstance ();
  UINT32 size () const { return entriesByUUID.size(); }
 private:
  DiameterPeerDb ();
  static DiameterPeerDb* instance;
  std::map<BasicString, DiameterPeer*> entriesByUUID;
  std::map<BasicString, DiameterPeer*> entriesByIP;
  std::map<SINT32, DiameterPeer*> entriesBySocketFD;
};
#endif
