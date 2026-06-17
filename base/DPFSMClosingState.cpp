#include "base/DPFSMClosingState.h"
#include "base/DPFSMBaseState.h"

DPFSMBaseState* DPFSMClosingState::pInstance = 0;

DPFSMBaseState* DPFSMClosingState::getInstance ()
{
  if (pInstance == NULL)
    {
      pInstance = new DPFSMClosingState ();
    }
  return pInstance;
}

DPFSMClosingState::DPFSMClosingState ()
{
  DiameterPeerState _state (DiameterPeerState::Closing);
  setState (_state);
}

const CHAR* DPFSMClosingState::getStateName () const
{
  return getState().getString();
}

Result DPFSMClosingState::processIRcvDPA (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMClosingState::processIRcvDPA called" << endl;
  return OK;
}

Result DPFSMClosingState::processRRcvDPA (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMClosingState::processRRcvDPA called" << endl;
  return OK;
}

Result DPFSMClosingState::processTimeout (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMClosingState::processTimeout called" << endl;
  return OK;
}

Result DPFSMClosingState::processIPeerDisc (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMClosingState::processIPeerDisc called" << endl;
  return OK;
}

Result DPFSMClosingState::processRPeerDisc (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMClosingState::processRPeerDisc called" << endl;
  return OK;
}
