#ifndef DPFSM_WAIT_CONN_ACK_STATE_H
#define DPFSM_WAIT_CONN_ACK_STATE_H

#include "DPFSMBaseState.h"
#include "DiameterPeer.h"

class DPFSMWaitConnAckState : public DPFSMBaseState
{
 public:
  static DPFSMBaseState* getInstance ();
  virtual const CHAR* getStateName () const;

  Result processIRcvConnNack (DiameterPeer* const pPeer);
  Result processTimeout (DiameterPeer* const pPeer);
  Result processIRcvConnAck (DiameterPeer* const pPeer);
  Result processRConnCER (DiameterPeer* const pPeer);

 private:
  static DPFSMBaseState* pInstance;
  DPFSMWaitConnAckState ();
};
#endif
