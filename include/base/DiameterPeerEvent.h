#ifndef DIAMETER_PEER_EVENT_H
#define DIAMETER_PEER_EVENT_H

#include "common/Types.h"

class DiameterPeerEvent
{
 public:
  
  enum DiameterPeerEventType
    {
      Start,
      RConnCER,
      IRcvConnNack,
      Timeout,
      IRcvConnAck,
      IRcvNonCEA,
      IPeerDisc,
      IRcvCEA,
      RPeerDisc,
      WinElection,
      SendMessage,
      RRcvMessage,
      RRcvDWR,
      RRcvDWA,
      Stop,
      RRcvDPR,
      RRcvCER,
      RRcvCEA,
      IRcvMessage,
      IRcvDWR,
      IRcvDWA,
      IRcvDPR,
      IRcvCER,
      IRcvDPA,
      RRcvDPA,
      Invalid
    };

  DiameterPeerEvent (DiameterPeerEventType _type) : 
    type (_type)
    {
    }
  DiameterPeerEvent();
  void setType (DiameterPeerEventType _type) { type = _type; };
  DiameterPeerEventType getType () const { return type; }
  inline const CHAR* getString ();

 private:  

    DiameterPeerEventType type;
};

const CHAR* DiameterPeerEvent::getString()
{
  switch (type)
    {
    case Start:
      return "Start";
    case RConnCER:
      return "RConnCER";
    case IRcvConnNack:
      return "IRcvConnNack";
    case Timeout:
      return "Timeout";
    case IRcvConnAck:
      return "IRcvConnAck";
    case IRcvNonCEA:
      return "IRcvNonCEA";
    case IPeerDisc:
      return "IPeerDisc";
    case IRcvCEA:
      return "IRcvCEA";
    case RPeerDisc:
      return "RPeerDisc";
    case WinElection:
      return "WinElection";
    case SendMessage:
      return "SendMessage";
    case RRcvMessage:
      return "RRcvMessage";
    case RRcvDWR:
      return "RRcvDWR";
    case RRcvDWA:
      return "RRcvDWA";
    case Stop:
      return "Stop";
    case RRcvDPR:
      return "RRcvDPR";
    case RRcvCER:
      return "RRcvCER";
    case RRcvCEA:
      return "RRcvCEA";
    case IRcvMessage:
      return "IRcvMessage";
    case IRcvDWR:
      return "IRcvDWR";
    case IRcvDWA:
      return "IRcvDWA";
    case IRcvDPR:
      return "IRcvDPR";
    case IRcvCER:
      return "IRcvCER";
    case IRcvDPA:
      return "IRcvDPA";
    case RRcvDPA:
      return "RRcvDPA";
    default:
      return "INVALID_PEER_EVENT";
    }
}
#endif
