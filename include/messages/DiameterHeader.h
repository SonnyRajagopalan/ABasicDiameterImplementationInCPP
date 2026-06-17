#ifndef DIAMETER_HEADER_H
#define DIAMETER_HEADER_H
#include <sstream>
#include "common/Types.h"
#include "common/Util.h"

/**
   RFC3588 defined Diameter header
*/
class DiameterHeader
{
 public:

  DiameterHeader ();
  DiameterHeader (UINT8 _version,
		  UINT8 _messageLength[],
		  UINT8 _flags,
		  UINT8 _commandCode[],
		  UINT32 _applicationId,
		  UINT32 _hopByHopId,
		  UINT32 _endToEndId);
  ~DiameterHeader();

  inline UINT8   getVersion () { return version; }
  inline UINT32  getMessageLength () { return getUINT32From3ByteUINT8Array (messageLength); }
  inline UINT8   getFlags () { return flags; }
  inline UINT32  getCommandCode () { return getUINT32From3ByteUINT8Array (commandCode); }
  inline UINT32  getApplicationId () { return applicationId; }
  inline UINT32  getHopByHopId () { return hopByHopId; }
  inline UINT32  getEndToEndId () { return endToEndId; }

  inline void    setVersion (const UINT8& _version) { version = _version; }
  inline void    setMessageLength (const UINT32& _length) { splitUINT32To3ByteUINT8Array (_length, messageLength); }
  inline void    setFlags (const UINT8 _flags) { flags = _flags; }
  inline void    setCommandCode (const UINT32 _commandCode) { splitUINT32To3ByteUINT8Array (_commandCode, commandCode); }
  inline void    setApplicationId (const UINT32 _applicationId) { applicationId = _applicationId; }
  inline void    setHopByHopId (const UINT32 _hopByHopId) { hopByHopId = _hopByHopId; }
  inline void    setEndToEndId (const UINT32 _endToEndId) { endToEndId = _endToEndId; }

  void     serialize (OutputStream&);
  void     deserialize (InputStream&);

 private:
  /**
    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |    Version    |                 Message Length                |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   | command flags |                  Command-Code                 |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                         Application-ID                        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                      Hop-by-Hop Identifier                    |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                      End-to-End Identifier                    |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |  AVPs ...
   +-+-+-+-+-+-+-+-+-+-+-+-+-

   */
  UINT8 version;
  /**
     From RFC 3588:
     The Message Length field is three octets and indicates the length
     of the Diameter message including the header fields.
  */
  UINT8 messageLength[3];
  UINT8 flags;
  UINT8 commandCode[3];
  UINT32 applicationId;
  UINT32 hopByHopId;
  UINT32 endToEndId;
};

#endif
