#include <map>
#include "base/DiameterPeerDb.h"
#include "base/DiameterPeer.h"
#include "common/Debug.h"

DiameterPeerDb* DiameterPeerDb::instance = 0;
std::map<BasicString, DiameterPeer*> entriesByUUID;
std::map<BasicString, DiameterPeer*> entriesByIP;
std::map<SINT32, DiameterPeer*> entriesBySocketFD;

DiameterPeerDb::DiameterPeerDb()
{
}

DiameterPeerDb::~DiameterPeerDb()
{
}

DiameterPeerDb* DiameterPeerDb::getInstance ()
{
  if (instance == 0)
    {
      instance = new DiameterPeerDb();
      cout << "Allocated a new PeerDb instance at " << hex << instance << dec << endl;
      instance->init();
    }

  return instance;
}

Result DiameterPeerDb::init() const
{
  return OK;
}

Result DiameterPeerDb::insertDiameterPeerIntoDb (DiameterPeer* pPeer)
{
  cout << "Inserting a new Diameter peer " << hex << pPeer << " into Db..." << dec << endl;
  entriesByUUID.insert (std::pair<BasicString, DiameterPeer*> (pPeer->getUUIDString(), pPeer));
  entriesByIP.insert (std::pair<BasicString, DiameterPeer*> (pPeer->getIPv4Address(), pPeer));
  entriesBySocketFD.insert (std::pair<SINT32, DiameterPeer*> (pPeer->getSocketFD(), pPeer));

  return OK;
}

Result DiameterPeerDb::removeDiameterPeerFromDb (DiameterPeer* pPeer)
{
  cout << "Removing a Diameter peer " << hex << pPeer << " from Db..." << dec << endl;
  entriesByUUID.erase (pPeer->getUUIDString());
  entriesByIP.erase (pPeer->getIPv4Address());
  entriesBySocketFD.erase (pPeer->getSocketFD());
  
  return OK;
}

DiameterPeer* DiameterPeerDb::getPeerFromUUID (BasicString& _uuid)
{
  std::map<BasicString, DiameterPeer*>::iterator item;

  item = entriesByUUID.find (_uuid);

  if (item != entriesByUUID.end())
    return (*item).second;
  else
    return NULL;
}

DiameterPeer* DiameterPeerDb::getPeerFromIPv4 (BasicString& _ipv4Address)
{
  std::map<BasicString, DiameterPeer*>::iterator item;
  
  item = entriesByIP.find (_ipv4Address);

  if (item != entriesByIP.end ())
    return (*item).second;
  else
    return NULL;
}

DiameterPeer* DiameterPeerDb::getPeerFromSocketFD (SINT32& _socketFD)
{
  std::map<SINT32, DiameterPeer*>::iterator item;
  
  item = entriesBySocketFD.find (_socketFD);

  if (item != entriesBySocketFD.end ())
    return (*item).second;
  else
    return NULL;
}
