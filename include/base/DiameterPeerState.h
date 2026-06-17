#ifndef DIAMETER_PEER_STATE_H
#define DIAMETER_PEER_STATE_H

#include "common/Types.h"

class DiameterPeerState
{
 public:
  enum DiameterPeerStateType
    {
      Closed,
      Closing,
      ROpen,
      WaitConnAck,
      WaitICEA,
      IOpen,
      WaitConnAckElect,
      WaitReturns,
      Invalid
    };

  DiameterPeerState (DiameterPeerStateType _type) : 
    type (_type) 
    {
    }
    DiameterPeerState () { type = Invalid; };
  const DiameterPeerStateType getState () const { return type; }
  void setType (DiameterPeerStateType _type) { type = _type; }
  inline const CHAR* getString () const;
 private:
  DiameterPeerStateType type;
};

inline const CHAR* DiameterPeerState::getString() const
{
  switch (type)
    {
      case Closed:
	return "Closed";
      case Closing:
	return "Closing";
      case ROpen:
	return "ROpen";
      case WaitConnAck:
	return "WaitConnAck";
      case WaitICEA:
	return "WaitICEA";
      case IOpen:
	return "IOpen";
      case WaitConnAckElect:
	return "WaitConnAckElect";
      case WaitReturns:
	return "WaitReturns";
    default:
      return "INVALID_PEER_STATE";
    }
}
#endif
