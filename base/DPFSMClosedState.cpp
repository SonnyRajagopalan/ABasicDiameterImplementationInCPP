#include "base/DPFSMClosedState.h"
#include "base/DPFSMBaseState.h"

DPFSMBaseState* DPFSMClosedState::pInstance = 0;

DPFSMBaseState* DPFSMClosedState::getInstance ()
{
  if (pInstance == NULL)
    {
      pInstance = new DPFSMClosedState ();
    }
  return pInstance;
}

DPFSMClosedState::DPFSMClosedState ()
{
  DiameterPeerState _state (DiameterPeerState::Closed);
  setState (_state);
}

const CHAR* DPFSMClosedState::getStateName () const
{
  return getState().getString();
}

Result DPFSMClosedState::processStart (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMClosedState::processStart called" << endl;
  return OK;
}

Result DPFSMClosedState::processRConnCER (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMClosedState::processRConnCER called" << endl;
  return OK;
}
