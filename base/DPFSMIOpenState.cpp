#include "base/DPFSMIOpenState.h"
#include "base/DPFSMBaseState.h"

DPFSMBaseState* DPFSMIOpenState::pInstance = 0;

DPFSMBaseState* DPFSMIOpenState::getInstance ()
{
  if (pInstance == NULL)
    {
      pInstance = new DPFSMIOpenState ();
    }
  return pInstance;
}

DPFSMIOpenState::DPFSMIOpenState ()
{
  DiameterPeerState _state (DiameterPeerState::IOpen);
  setState (_state);
}

const CHAR* DPFSMIOpenState::getStateName () const
{
  return getState().getString();
}

Result DPFSMIOpenState::processSendMessage (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMIOpenState::processSendMessage called" << endl;
  return OK;
}

Result DPFSMIOpenState::processIRcvMessage (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMIOpenState::processIRcvMessage called" << endl;
  return OK;
}

Result DPFSMIOpenState::processIRcvDWR (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMIOpenState::processIRcvDWR called" << endl;
  return OK;
}

Result DPFSMIOpenState::processIRcvDWA (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMIOpenState::processIRcvDWA called" << endl;
  return OK;
}

Result DPFSMIOpenState::processRConnCER (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMIOpenState::processRConnCER called" << endl;
  return OK;
}

Result DPFSMIOpenState::processStop (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMIOpenState::processStop called" << endl;
  return OK;
}

Result DPFSMIOpenState::processIRcvDPR (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMIOpenState::processIRcvDPR called" << endl;
  return OK;
}

Result DPFSMIOpenState::processIPeerDisc (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMIOpenState::processIPeerDisc called" << endl;
  return OK;
}

Result DPFSMIOpenState::processIRcvCER (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMIOpenState::processIRcvCER called" << endl;
  return OK;
}

Result DPFSMIOpenState::processIRcvCEA (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMIOpenState::processIRcvCEA called" << endl;
  return OK;
}
