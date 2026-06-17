#ifndef DIAMETER_MESSAGE_H
#define DIAMETER_MESSAGE_H

#include "DiameterMessageTypes.h"
#include "DiameterHeader.h"
#include "avps/DiameterAvpList.h"
#include "avps/DiameterAvp.h"

class DiameterMessage // Pure virtual class
{
 public:

  enum MessageFlags
    {
      RequestFlag                  = 0x80,
      ProxiableFlag                = 0x40,
      ErrorFlag                    = 0x20,
      PotentiallyRetransmittedFlag = 0x10,
    };

  DiameterMessage () {}
  //virtual ~DiameterMessage ();
  DiameterMessage (UINT8 _version,
		   UINT32 _messageLength,
		   UINT8 _flags,
		   UINT32 _commandCode,
		   UINT32 _applicationId,
		   UINT32 _hopByHopId,
		   UINT32 _endToEndId);
  // Getter functions specific to DiameterHeader object
  inline UINT8   getVersion () { return header.getVersion (); }
  inline UINT32  getMessageLength () { return header.getMessageLength (); }
  inline UINT8   getFlags () { return header.getFlags (); }
  inline UINT32  getCommandCode () { return header.getCommandCode (); }
  inline UINT32  getApplicationId () { return header.getApplicationId (); }
  inline UINT32  getHopByHopId () { return header.getHopByHopId (); }
  inline UINT32  getEndToEndId () { return header.getEndToEndId (); }
  inline DiameterHeader getHeader () { return header; }
  inline DiameterMessageType getMessageType () { return type; }

  // Getter functions specific to AVPs
  inline UINT32 getNumberOfAvps () { return avpList.getNumberOfAvps (); }
  inline DiameterAvpList getAvps () { return avpList; }

  // Setter functions specific to DiameterHeader object
  inline void    setVersion (UINT8 _version) { header.setVersion (_version); }
  inline void    updateMessageLength () { setMessageLength (avpList.getAvpListLength () + DIAMETER_HEADER_SIZE); }
  inline void    setMessageLength (UINT32 _length) { header.setMessageLength (_length); }
  inline void    setFlags (UINT8 _flags) { header.setFlags (_flags); }
  inline void    setCommandCode (UINT32 _commandCode) { header.setCommandCode (_commandCode); }
  inline void    setApplicationId (UINT32 _applicationId) { header.setApplicationId (_applicationId); }
  inline void    setHopByHopId (UINT32 _hopByHopId) { header.setHopByHopId (_hopByHopId); }
  inline void    setEndToEndId (UINT32 _endToEndId) { header.setEndToEndId (_endToEndId); }

  // Setter functions specific to AVPs
  inline void addAnAvp (DiameterAvp* avp) { avpList.addAvpToAvpList (avp); updateMessageLength(); }
  inline void setNumberOfAvps (UINT32 _numberOfAvps) {}

  virtual void serialize (OutputStream& os) = 0;
  virtual void deserialize (InputStream& in) = 0;
 protected: // Need derived classes to be able to access these
  DiameterMessageType type;
  DiameterHeader      header;
  DiameterAvpList     avpList;
};
#endif
