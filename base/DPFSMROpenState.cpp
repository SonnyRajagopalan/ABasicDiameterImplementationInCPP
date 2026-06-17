#include "base/DPFSMROpenState.h"
#include "base/DPFSMBaseState.h"

DPFSMBaseState* DPFSMROpenState::pInstance = 0;

DPFSMBaseState* DPFSMROpenState::getInstance ()
{
  if (pInstance == NULL)
    {
      pInstance = new DPFSMROpenState ();
    }
  return pInstance;
}

DPFSMROpenState::DPFSMROpenState ()
{
  DiameterPeerState _state (DiameterPeerState::ROpen);
  setState (_state);
}

const CHAR* DPFSMROpenState::getStateName () const
{
  return getState().getString();
}

Result DPFSMROpenState::processSendMessage (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMROpenState::processSendMessage called" << endl;
  return OK;
}

Result DPFSMROpenState::processRRcvMessage (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMROpenState::processRRcvMessage called" << endl;
  return OK;
}

Result DPFSMROpenState::processRRcvDWR (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMROpenState::processRRcvDWR called" << endl;
  return OK;
}

Result DPFSMROpenState::processRRcvDWA (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMROpenState::processRRcvDWA called" << endl;
  return OK;
}

Result DPFSMROpenState::processRConnCER (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMROpenState::processRConnCER called" << endl;
  return OK;
}

Result DPFSMROpenState::processStop (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMROpenState::processStop called" << endl;
  return OK;
}

Result DPFSMROpenState::processRRcvDPR (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMROpenState::processRRcvDPR called" << endl;
  return OK;
}

Result DPFSMROpenState::processRPeerDisc (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMROpenState::processRPeerDisc called" << endl;
  return OK;
}

Result DPFSMROpenState::processRRcvCER (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMROpenState::processRRcvCER called" << endl;
  return OK;
}

Result DPFSMROpenState::processRRcvCEA (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMROpenState::processRRcvCEA called" << endl;
  return OK;
}
