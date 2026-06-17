#ifndef DPFSM_I_OPEN_STATE_H
#define DPFSM_I_OPEN_STATE_H

#include "DPFSMBaseState.h"
#include "DiameterPeer.h"

class DPFSMIOpenState : public DPFSMBaseState
{
 public:
  static DPFSMBaseState* getInstance ();
  virtual const CHAR* getStateName () const;

  Result processSendMessage (DiameterPeer* const pPeer);
  Result processIRcvMessage (DiameterPeer* const pPeer);
  Result processIRcvDWR (DiameterPeer* const pPeer);
  Result processIRcvDWA (DiameterPeer* const pPeer);
  Result processRConnCER (DiameterPeer* const pPeer);
  Result processStop (DiameterPeer* const pPeer);
  Result processIRcvDPR (DiameterPeer* const pPeer);
  Result processIPeerDisc (DiameterPeer* const pPeer);
  Result processIRcvCER (DiameterPeer* const pPeer);
  Result processIRcvCEA (DiameterPeer* const pPeer);

 private:
  static DPFSMBaseState* pInstance;
  DPFSMIOpenState ();
};
#endif
