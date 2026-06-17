#include "base/DPFSMWaitConnAckElectState.h"
#include "base/DPFSMBaseState.h"

DPFSMBaseState* DPFSMWaitConnAckElectState::pInstance = 0;

DPFSMBaseState* DPFSMWaitConnAckElectState::getInstance ()
{
  if (pInstance == NULL)
    {
      pInstance = new DPFSMWaitConnAckElectState ();
    }
  return pInstance;
}

DPFSMWaitConnAckElectState::DPFSMWaitConnAckElectState ()
{
  DiameterPeerState _state (DiameterPeerState::WaitConnAckElect);
  setState (_state);
}

const CHAR* DPFSMWaitConnAckElectState::getStateName () const
{
  return getState().getString();
}

Result DPFSMWaitConnAckElectState::processRConnCER (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMWaitConnAckElectState::processRConnCER called" << endl;
  return OK;
}

Result DPFSMWaitConnAckElectState::processIRcvConnNack (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMWaitConnAckElectState::processIRcvConnNack called" << endl;
  return OK;
}

Result DPFSMWaitConnAckElectState::processIRcvConnAck (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMWaitConnAckElectState::processIRcvConnAck called" << endl;
  return OK;
}

Result DPFSMWaitConnAckElectState::processTimeout (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMWaitConnAckElectState::processTimeout called" << endl;
  return OK;
}

Result DPFSMWaitConnAckElectState::processRPeerDisc (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMWaitConnAckElectState::processRPeerDisc called" << endl;
  return OK;
}
