#ifndef DPFSM_R_OPEN_STATE_H
#define DPFSM_R_OPEN_STATE_H

#include "DPFSMBaseState.h"
#include "DiameterPeer.h"

class DPFSMROpenState : public DPFSMBaseState
{
 public:
  static DPFSMBaseState* getInstance ();
  virtual const CHAR* getStateName () const;

  Result processSendMessage (DiameterPeer* const pPeer);
  Result processRRcvMessage (DiameterPeer* const pPeer);
  Result processRRcvDWR (DiameterPeer* const pPeer);
  Result processRRcvDWA (DiameterPeer* const pPeer);
  Result processRConnCER (DiameterPeer* const pPeer);
  Result processStop (DiameterPeer* const pPeer);
  Result processRRcvDPR (DiameterPeer* const pPeer);
  Result processRPeerDisc (DiameterPeer* const pPeer);
  Result processRRcvCER (DiameterPeer* const pPeer);
  Result processRRcvCEA (DiameterPeer* const pPeer);

 private:
  static DPFSMBaseState* pInstance;
  DPFSMROpenState ();
};
#endif
