#include "base/DPFSMWaitReturnsState.h"
#include "base/DPFSMBaseState.h"

DPFSMBaseState* DPFSMWaitReturnsState::pInstance = 0;

DPFSMBaseState* DPFSMWaitReturnsState::getInstance ()
{
  if (pInstance == NULL)
    {
      pInstance = new DPFSMWaitReturnsState ();
    }
  return pInstance;
}

DPFSMWaitReturnsState::DPFSMWaitReturnsState ()
{
  DiameterPeerState _state (DiameterPeerState::WaitReturns);
  setState (_state);
}

const CHAR* DPFSMWaitReturnsState::getStateName () const
{
  return getState().getString();
}

Result DPFSMWaitReturnsState::processRPeerDisc (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMWaitReturnsState::processRPeerDisc called" << endl;
  return OK;
}

Result DPFSMWaitReturnsState::processTimeout (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMWaitReturnsState::processTimeout called" << endl;
  return OK;
}

Result DPFSMWaitReturnsState::processIRcvCEA (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMWaitReturnsState::processIRcvCEA called" << endl;
  return OK;
}

Result DPFSMWaitReturnsState::processWinElection (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMWaitReturnsState::processWinElection called" << endl;
  return OK;
}

Result DPFSMWaitReturnsState::processIPeerDisc (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMWaitReturnsState::processIPeerDisc called" << endl;
  return OK;
}

Result DPFSMWaitReturnsState::processRConnCER (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMWaitReturnsState::processRConnCER called" << endl;
  return OK;
}
