#ifndef DPFSM_CLOSING_STATE_H
#define DPFSM_CLOSING_STATE_H

#include "DPFSMBaseState.h"
#include "DiameterPeer.h"

class DPFSMClosingState : public DPFSMBaseState
{
 public:
  static DPFSMBaseState* getInstance ();
  virtual const CHAR* getStateName () const;

  Result processIRcvDPA (DiameterPeer* const pPeer);
  Result processRRcvDPA (DiameterPeer* const pPeer);
  Result processTimeout (DiameterPeer* const pPeer);
  Result processIPeerDisc (DiameterPeer* const pPeer);
  Result processRPeerDisc (DiameterPeer* const pPeer);

 private:
  static DPFSMBaseState* pInstance;
  DPFSMClosingState ();
};
#endif
