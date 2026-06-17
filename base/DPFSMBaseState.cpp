#include "base/DPFSMBaseState.h"

Result DPFSMBaseState::processEvent (DiameterPeer* const pPeer, DiameterPeerEvent const& event)
{
  switch (event.getType ())
    {
    case DiameterPeerEvent::Start:
       return processStart (pPeer);
     case DiameterPeerEvent::RConnCER:
       return processRConnCER (pPeer);
     case DiameterPeerEvent::IRcvConnNack:
       return processIRcvConnNack (pPeer);
     case DiameterPeerEvent::Timeout:
       return processTimeout (pPeer);
     case DiameterPeerEvent::IRcvConnAck:
       return processIRcvConnAck (pPeer);
     case DiameterPeerEvent::IRcvNonCEA:
       return processIRcvNonCEA (pPeer);
     case DiameterPeerEvent::IPeerDisc:
       return processIPeerDisc (pPeer);
     case DiameterPeerEvent::IRcvCEA:
       return processIRcvCEA (pPeer);
     case DiameterPeerEvent::RPeerDisc:
       return processRPeerDisc (pPeer);
     case DiameterPeerEvent::WinElection:
       return processWinElection (pPeer);
     case DiameterPeerEvent::SendMessage:
       return processSendMessage (pPeer);
     case DiameterPeerEvent::RRcvMessage:
       return processRRcvMessage (pPeer);
     case DiameterPeerEvent::RRcvDWR:
       return processRRcvDWR (pPeer);
     case DiameterPeerEvent::RRcvDWA:
       return processRRcvDWA (pPeer);
     case DiameterPeerEvent::Stop:
       return processStop (pPeer);
     case DiameterPeerEvent::RRcvDPR:
       return processRRcvDPR (pPeer);
     case DiameterPeerEvent::RRcvCER:
       return processRRcvCER (pPeer);
     case DiameterPeerEvent::RRcvCEA:
       return processRRcvCEA (pPeer);
     case DiameterPeerEvent::IRcvMessage:
       return processIRcvMessage (pPeer);
     case DiameterPeerEvent::IRcvDWR:
       return processIRcvDWR (pPeer);
     case DiameterPeerEvent::IRcvDWA:
       return processIRcvDWA (pPeer);
     case DiameterPeerEvent::IRcvDPR:
       return processIRcvDPR (pPeer);
     case DiameterPeerEvent::IRcvCER:
       return processIRcvCER (pPeer);
     case DiameterPeerEvent::IRcvDPA:
       return processIRcvDPA (pPeer);
     case DiameterPeerEvent::RRcvDPA:
       return processRRcvDPA (pPeer);
    default:
      return ERROR;
    }
}

Result DPFSMBaseState::changeState (DiameterPeer* const pPeer, DPFSMBaseState* pNextState)
{
  pPeer->changeState (pNextState);

  return OK;
}

Result DPFSMBaseState::processStart (DiameterPeer* const pPeer) 
{
  DPFSMLOG << LOGPRIORITY_ERROR << "processStart called in base class" << endl;
  return ERROR; 
}

Result DPFSMBaseState::processRConnCER (DiameterPeer* const pPeer) 
{
  DPFSMLOG << LOGPRIORITY_ERROR << "processRConnCER called in base class" << endl;
  return ERROR; 
}

Result DPFSMBaseState::processIRcvConnNack (DiameterPeer* const pPeer) 
{
  DPFSMLOG << LOGPRIORITY_ERROR << "processIRcvConnNack called in base class" << endl;
  return ERROR; 
}

Result DPFSMBaseState::processTimeout (DiameterPeer* const pPeer) 
{
  DPFSMLOG << LOGPRIORITY_ERROR << "processTimeout called in base class" << endl;
  return ERROR; 
}

Result DPFSMBaseState::processIRcvConnAck (DiameterPeer* const pPeer) 
{
  DPFSMLOG << LOGPRIORITY_ERROR << "processIRcvConnAck called in base class" << endl;
  return ERROR; 
}

Result DPFSMBaseState::processIRcvNonCEA (DiameterPeer* const pPeer) 
{
  DPFSMLOG << LOGPRIORITY_ERROR << "processIRcvNonCEA called in base class" << endl;
  return ERROR; 
}

Result DPFSMBaseState::processIPeerDisc (DiameterPeer* const pPeer) 
{
  DPFSMLOG << LOGPRIORITY_ERROR << "processIPeerDic called in base class" << endl;
  return ERROR; 
}

Result DPFSMBaseState::processIRcvCEA (DiameterPeer* const pPeer) 
{
  DPFSMLOG << LOGPRIORITY_ERROR << "processIRcvCEA called in base class" << endl;
  return ERROR; 
}

Result DPFSMBaseState::processRPeerDisc (DiameterPeer* const pPeer) 
{
  DPFSMLOG << LOGPRIORITY_ERROR << "processRPeerDisc called in base class" << endl;
  return ERROR; 
}

Result DPFSMBaseState::processWinElection (DiameterPeer* const pPeer) 
{
  DPFSMLOG << LOGPRIORITY_ERROR << "processWinElection called in base class" << endl;
  return ERROR; 
}

Result DPFSMBaseState::processSendMessage (DiameterPeer* const pPeer) 
{
  DPFSMLOG << LOGPRIORITY_ERROR << "processSendMessage called in base class" << endl;
  return ERROR; 
}

Result DPFSMBaseState::processRRcvMessage (DiameterPeer* const pPeer) 
{
  DPFSMLOG << LOGPRIORITY_ERROR << "processRRcvMessage called in base class" << endl;
  return ERROR; 
}

Result DPFSMBaseState::processRRcvDWR (DiameterPeer* const pPeer) 
{
  DPFSMLOG << LOGPRIORITY_ERROR << "processRRcvDWR called in base class" << endl;
  return ERROR; 
}

Result DPFSMBaseState::processRRcvDWA (DiameterPeer* const pPeer) 
{
  DPFSMLOG << LOGPRIORITY_ERROR << "processRRcvDWA called in base class" << endl;
  return ERROR; 
}

Result DPFSMBaseState::processStop (DiameterPeer* const pPeer) 
{
  DPFSMLOG << LOGPRIORITY_ERROR << "processStop called in base class" << endl;
  return ERROR; 
}

Result DPFSMBaseState::processRRcvDPR (DiameterPeer* const pPeer) 
{
  DPFSMLOG << LOGPRIORITY_ERROR << "processRRcvDPR called in base class" << endl;
  return ERROR; 
}

Result DPFSMBaseState::processRRcvCER (DiameterPeer* const pPeer) 
{
  DPFSMLOG << LOGPRIORITY_ERROR << "processRRcvCER called in base class" << endl;
  return ERROR; 
}

Result DPFSMBaseState::processRRcvCEA (DiameterPeer* const pPeer) 
{
  DPFSMLOG << LOGPRIORITY_ERROR << "processRRcvCEA called in base class" << endl;
  return ERROR; 
}

Result DPFSMBaseState::processIRcvMessage (DiameterPeer* const pPeer) 
{
  DPFSMLOG << LOGPRIORITY_ERROR << "processIRcvMessage called in base class" << endl;
  return ERROR; 
}

Result DPFSMBaseState::processIRcvDWR (DiameterPeer* const pPeer) 
{
  DPFSMLOG << LOGPRIORITY_ERROR << "processIRcvDWR called in base class" << endl;
  return ERROR; 
}

Result DPFSMBaseState::processIRcvDWA (DiameterPeer* const pPeer) 
{
  DPFSMLOG << LOGPRIORITY_ERROR << "processIRcvDWA called in base class" << endl;
  return ERROR; 
}

Result DPFSMBaseState::processIRcvDPR (DiameterPeer* const pPeer) 
{
  DPFSMLOG << LOGPRIORITY_ERROR << "processIRcvDPR called in base class" << endl;
  return ERROR; 
}

Result DPFSMBaseState::processIRcvCER (DiameterPeer* const pPeer) 
{
  DPFSMLOG << LOGPRIORITY_ERROR << "processIRcvCER called in base class" << endl;
  return ERROR; 
}

Result DPFSMBaseState::processIRcvDPA (DiameterPeer* const pPeer) 
{
  DPFSMLOG << LOGPRIORITY_ERROR << "processIRcvDPA called in base class" << endl;
  return ERROR; 
}

Result DPFSMBaseState::processRRcvDPA (DiameterPeer* const pPeer) 
{
  DPFSMLOG << LOGPRIORITY_ERROR << "processRRcvDPA called in base class" << endl;
  return ERROR; 
}
