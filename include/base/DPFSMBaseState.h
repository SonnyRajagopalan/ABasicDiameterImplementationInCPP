#ifndef DPFSM_BASE_STATE_H
#define DPFSM_BASE_STATE_H

#include "DiameterPeer.h"
#include "DiameterPeerEvent.h"
#include "DiameterPeerState.h"
#include "logging/DiameterBaseTaskDiagnosticLog.h"
#include <string>

#define DPFSMLOG (DiameterBaseTaskDiagnosticLog::getInstance ())

class DPFSMBaseState
{
  /**
     See http://www.parashift.com/c++-faq-lite/const-correctness.html
     for my const notation
  */
 public:
  /**
     Some of the following functions declared pure virtual because 
     we want these functions implemented in the derived classes
     Also, the pure virtual functions are declared const because
     we know the functions will not change the value of (this).
  */
  virtual DiameterPeerState getState () const { return state; }
  virtual void setState (DiameterPeerState _state) { state = _state; }
  virtual const CHAR* getStateName () const = 0;
  virtual Result processEvent (DiameterPeer* const pPeer, DiameterPeerEvent const& event);

  // Protected access allows the derived classes to access these methods
 protected:
  Result changeState (DiameterPeer* const pPeer, DPFSMBaseState* pNextState);
  /**
     Methods for processing events in the state machine.
     Not declared pure virtual because some of these base methods
     will be used as "catchalls" when derived class methods won't
     be called. Note that these functions will most likely change the value
     of this (one way is by changing the state) so the methods are not 
     declared const.
  */
  virtual Result processIPeerDisc (DiameterPeer* const pPeer);
  virtual Result processIRcvCEA (DiameterPeer* const pPeer);
  virtual Result processIRcvCER (DiameterPeer* const pPeer);
  virtual Result processIRcvConnAck (DiameterPeer* const pPeer);
  virtual Result processIRcvConnNack (DiameterPeer* const pPeer);
  virtual Result processIRcvDPA (DiameterPeer* const pPeer);
  virtual Result processIRcvDPR (DiameterPeer* const pPeer);
  virtual Result processIRcvDWA (DiameterPeer* const pPeer);
  virtual Result processIRcvDWR (DiameterPeer* const pPeer);
  virtual Result processIRcvMessage (DiameterPeer* const pPeer);
  virtual Result processIRcvNonCEA (DiameterPeer* const pPeer);
  virtual Result processRConnCER (DiameterPeer* const pPeer);
  virtual Result processRPeerDisc (DiameterPeer* const pPeer);
  virtual Result processRRcvCEA (DiameterPeer* const pPeer);
  virtual Result processRRcvCER (DiameterPeer* const pPeer);
  virtual Result processRRcvDPA (DiameterPeer* const pPeer);
  virtual Result processRRcvDPR (DiameterPeer* const pPeer);
  virtual Result processRRcvDWA (DiameterPeer* const pPeer);
  virtual Result processRRcvDWR (DiameterPeer* const pPeer);
  virtual Result processRRcvMessage (DiameterPeer* const pPeer);
  virtual Result processSendMessage (DiameterPeer* const pPeer);
  virtual Result processStart (DiameterPeer* const pPeer);
  virtual Result processStop (DiameterPeer* const pPeer);
  virtual Result processTimeout (DiameterPeer* const pPeer);
  virtual Result processWinElection (DiameterPeer* const pPeer);
  
  // Private access only for this class and friend classeses
 private:
  DiameterPeerState state;
};

#endif
