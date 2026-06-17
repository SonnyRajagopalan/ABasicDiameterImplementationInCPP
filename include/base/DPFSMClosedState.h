#ifndef DPFSM_CLOSED_STATE_H
#define DPFSM_CLOSED_STATE_H

#include "DPFSMBaseState.h"
#include "DiameterPeer.h"

class DPFSMClosedState : public DPFSMBaseState
{
 public:
  static DPFSMBaseState* getInstance ();
  virtual const CHAR* getStateName () const;

  Result processStart (DiameterPeer* const pPeer);
  Result processRConnCER (DiameterPeer* const pPeer);

 private:
  static DPFSMBaseState* pInstance;
  DPFSMClosedState ();
  DPFSMClosedState (const DPFSMClosedState& _dummy); // Prevents creation of a copy of the singleton
};
#endif
