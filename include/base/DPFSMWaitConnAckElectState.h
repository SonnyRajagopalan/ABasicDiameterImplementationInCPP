#ifndef DPFSM_WAIT_CONN_ACKELECT_STATE_H
#define DPFSM_WAIT_CONN_ACKELECT_STATE_H

#include "DPFSMBaseState.h"
#include "DiameterPeer.h"

class DPFSMWaitConnAckElectState : public DPFSMBaseState
{
 public:
  static DPFSMBaseState* getInstance ();
  virtual const CHAR* getStateName () const;

  Result processRConnCER (DiameterPeer* const pPeer);
  Result processIRcvConnNack (DiameterPeer* const pPeer);
  Result processIRcvConnAck (DiameterPeer* const pPeer);
  Result processTimeout (DiameterPeer* const pPeer);
  Result processRPeerDisc (DiameterPeer* const pPeer);

 private:
  static DPFSMBaseState* pInstance;
  DPFSMWaitConnAckElectState ();
};
#endif
