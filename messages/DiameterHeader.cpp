#include "messages/DiameterHeader.h"
#include "common/Debug.h"

DiameterHeader::DiameterHeader()
{
}

DiameterHeader::DiameterHeader(UINT8 _version,
			       UINT8 _messageLength[],
			       UINT8 _flags,
			       UINT8 _commandCode[],
			       UINT32 _applicationId,
			       UINT32 _hopByHopId,
			       UINT32 _endToEndId) :
  version (_version),
  flags (_flags),
  applicationId (_applicationId),
  hopByHopId (_hopByHopId),
  endToEndId (_endToEndId)
{
  UINT32 mLen, cCode;

  mLen = getUINT32From3ByteUINT8Array (_messageLength);
  splitUINT32To3ByteUINT8Array (mLen, messageLength);

  cCode = getUINT32From3ByteUINT8Array (_commandCode);
  splitUINT32To3ByteUINT8Array (cCode, commandCode);
}

DiameterHeader::~DiameterHeader()
{
}

void DiameterHeader::serialize (OutputStream& os)
{
  serializeUINT8 (version, os);
  serialize3ByteUnsignedChar (messageLength, os);
  serializeUINT8 (flags, os);
  serialize3ByteUnsignedChar (commandCode, os);
  serializeUINT32 (applicationId, os);
  serializeUINT32 (hopByHopId, os);
  serializeUINT32 (endToEndId, os);
}

void DiameterHeader::deserialize (InputStream& in)
{
  deserializeUINT8 (version, in);
  deserializeTo3ByteUnsignedChar (messageLength, in);
  deserializeUINT8 (flags, in);
  deserializeTo3ByteUnsignedChar (commandCode, in);
  deserializeUINT32 (applicationId, in);
  deserializeUINT32 (hopByHopId, in);
  deserializeUINT32 (endToEndId, in); 
  
  cout << "DiameterHeader::deserialize:: version = " << version;
  cout << ", messageLength = " << getUINT32From3ByteUINT8Array (messageLength);
  cout << ", flags = " << flags;
  cout << ", commandCode = " << getUINT32From3ByteUINT8Array (commandCode);
  cout << ", applicationId = " << applicationId << ", hopByHopId = " << hopByHopId << ", and endToEndId = " << endToEndId << endl;
}
