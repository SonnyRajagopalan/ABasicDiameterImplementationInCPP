#ifndef DIAMETER_PEER_H
#define DIAMETER_PEER_H

#include "apps/DiameterApp.h"
#include "DiameterPeerState.h"
#include "DiameterPeerEvent.h"
#include "common/DiameterConstants.h"
#include "net/Socket.h"

class DPFSMBaseState;

class DiameterPeer
{
 public:

  DiameterPeer (BasicString& _ipv4Address);
  DiameterPeer (BasicString& _ipv4Address, const Socket* _newClientSocket);
  DiameterPeer (BasicString& _ipv4Address, const SINT32 _newClientSocketFD);
  ~DiameterPeer ();
  DiameterPeer (BasicString& _ipv4Address, BasicString& _description);
  DPFSMBaseState* getCurrentState(); 
  Result changeState (DPFSMBaseState* pNextState);
  Result processEvent (DiameterPeerEvent _event);
  Result setApplication (DiameterApp& _application) { application = _application; return OK; }

  DiameterApp getApplication () const { return application; }
  BasicString getUUIDString () const { return uuid; }
  BasicString getIPv4Address () const { return ipv4Address; }
  //Socket getSocket () const { return socket; }
  SINT32 getSocketFD () const { return socketFD; }
  /**
     A number of statistics management getters/setters (TBD)
  */
 private:
  
  DPFSMBaseState* pCurrentState; // Pointer to singleton for current state
  //Socket socket;
  SINT32 socketFD;
  DiameterApp application;
  BasicString uuid;
  BasicString description;
  BasicString ipv4Address; // Some identification--IP address will do for now?
  /**
     A number of statistics management OMs (TBD)
  */
};

#endif
