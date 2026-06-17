#include "base/DPFSMWaitICEAState.h"
#include "base/DPFSMBaseState.h"

DPFSMBaseState* DPFSMWaitICEAState::pInstance = 0;

DPFSMBaseState* DPFSMWaitICEAState::getInstance ()
{
  if (pInstance == NULL)
    {
      pInstance = new DPFSMWaitICEAState ();
    }
  return pInstance;
}

DPFSMWaitICEAState::DPFSMWaitICEAState ()
{
  DiameterPeerState _state (DiameterPeerState::WaitICEA);
  setState (_state);
}

const CHAR* DPFSMWaitICEAState::getStateName () const
{
  return getState().getString();
}

Result DPFSMWaitICEAState::processTimeout (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMWaitICEAState::processTimeout called" << endl;
  return OK;
}

Result DPFSMWaitICEAState::processIRcvNonCEA (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMWaitICEAState::processIRcvNonCEA called" << endl;
  return OK;
}

Result DPFSMWaitICEAState::processIPeerDisc (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMWaitICEAState::processIPeerDisc called" << endl;
  return OK;
}

Result DPFSMWaitICEAState::processIRcvCEA (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMWaitICEAState::processIRcvCEA called" << endl;
  return OK;
}

Result DPFSMWaitICEAState::processRConnCER (DiameterPeer* const pPeer)
{
  DPFSMLOG << LOGPRIORITY_INFO << "DPFSMWaitICEAState::processRConnCER called" << endl;
  return OK;
}
