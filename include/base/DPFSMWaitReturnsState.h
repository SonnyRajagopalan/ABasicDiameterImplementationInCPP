#ifndef DPFSM_WAIT_RETURNS_STATE_H
#define DPFSM_WAIT_RETURNS_STATE_H

#include "DPFSMBaseState.h"
#include "DiameterPeer.h"

class DPFSMWaitReturnsState : public DPFSMBaseState
{
 public:
  static DPFSMBaseState* getInstance ();
  virtual const CHAR* getStateName () const;

  Result processRPeerDisc (DiameterPeer* const pPeer);
  Result processTimeout (DiameterPeer* const pPeer);
  Result processIRcvCEA (DiameterPeer* const pPeer);
  Result processWinElection (DiameterPeer* const pPeer);
  Result processIPeerDisc (DiameterPeer* const pPeer);
  Result processRConnCER (DiameterPeer* const pPeer);

 private:
  static DPFSMBaseState* pInstance;
  DPFSMWaitReturnsState ();
};
#endif
