#ifndef DPFSM_WAIT_I_CEA_STATE_H
#define DPFSM_WAIT_I_CEA_STATE_H

#include "DPFSMBaseState.h"
#include "DiameterPeer.h"

class DPFSMWaitICEAState : public DPFSMBaseState
{
 public:
  static DPFSMBaseState* getInstance ();
  virtual const CHAR* getStateName () const;

  Result processTimeout (DiameterPeer* const pPeer);
  Result processIRcvNonCEA (DiameterPeer* const pPeer);
  Result processIPeerDisc (DiameterPeer* const pPeer);
  Result processIRcvCEA (DiameterPeer* const pPeer);
  Result processRConnCER (DiameterPeer* const pPeer);

 private:
  static DPFSMBaseState* pInstance;
  DPFSMWaitICEAState ();
};
#endif
