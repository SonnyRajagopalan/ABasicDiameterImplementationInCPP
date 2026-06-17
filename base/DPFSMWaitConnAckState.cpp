#include "base/DPFSMWaitConnAckState.h"
#include "base/DPFSMBaseState.h"

DPFSMBaseState* DPFSMWaitConnAckState::pInstance = 0;

DPFSMBaseState* DPFSMWaitConnAckState::getInstance ()
{
  if (pInstance == NULL)
    {
      pInstance = new DPFSMWaitConnAckState ();
    }
  return pInstance;
}

DPFSMWaitConnAckState::DPFSMWaitConnAckState ()
{
  DiameterPeerState _state (DiameterPeerState::WaitConnAck);
  setState (_state);
}

const CHAR* DPFSMWaitConnAckState::getStateName () const
{
  return getState().getString();
}

Result DPFSMWaitConnAckState::processIRcvConnNack (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMWaitConnAckState::processIRcvConnNack called" << endl;
  return OK;
}

Result DPFSMWaitConnAckState::processTimeout (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMWaitConnAckState::processTimeout called" << endl;
  return OK;
}

Result DPFSMWaitConnAckState::processIRcvConnAck (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMWaitConnAckState::processIRcvConnAck called" << endl;
  return OK;
}

Result DPFSMWaitConnAckState::processRConnCER (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMWaitConnAckState::processRConnCER called" << endl;
  return OK;
}
