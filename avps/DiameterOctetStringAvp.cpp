#include <string>
#include "avps/DiameterOctetStringAvp.h"
#include "common/Debug.h"

DiameterOctetStringAvp::DiameterOctetStringAvp () : DiameterAvp ()
{
}

DiameterOctetStringAvp::DiameterOctetStringAvp (UINT32 _code, 
						UINT8 _flags, 
						UINT32 _vendorId,
						std::string _value) : 
  DiameterAvp (_code, _flags, _vendorId)
{
  UINT32 len = 0;
  UINT8 bytes[3];

  if (flags & DIAMETER_AVP_VENDOR_SPECIFIC_FLAG)
    len = 12 + _value.size ();
  else
    len = 8 + _value.size ();

  splitUINT32To3ByteUINT8Array (len, bytes);
  setLength (bytes);
  value = _value;
  setType (DiameterAvp::OctetString);

  //std::cout << "DiameterOctetStringAvp constructor with args (code, flags, vendorId, value) called." << std::endl;
}

DiameterOctetStringAvp::DiameterOctetStringAvp (BasicString& _name, UINT32& _code,
						UINT8& _must, UINT8& _may, UINT8& _shouldNot, 
						UINT8& _mustNot, bool& _encr) : 
  DiameterAvp (_name, _code, _must, _may, _shouldNot, _mustNot, _encr)
{
  setType (DiameterAvp::OctetString);

  //std::cout << "DiameterOctetStringAvp constructor with args (name, code, <ALL FLAGS>) called." << std::endl;
}

DiameterOctetStringAvp::~DiameterOctetStringAvp ()
{
  //std::cout << "DiameterOctetStringAvp destructor called." << std::endl;  
}

void DiameterOctetStringAvp::serialize (OutputStream& os)
{
  UINT32 paddingLen = 0;
  UINT32 lenUINT32 = getUINT32From3ByteUINT8Array (length);

  if ((lenUINT32 % 4) != 0)
    paddingLen = 4 - (lenUINT32 % 4);

  serializeUINT32 (code, os);
  serializeUINT8  (flags, os);
  serialize3ByteUnsignedChar (length, os);

  if (flags & DIAMETER_AVP_VENDOR_SPECIFIC_FLAG)
    serializeUINT32 (vendorId, os);

  //cout << "DiameterOctetStringAvp::serialize--serialized " << lenUINT32 << "-byte string [" << value << "] and padding = " << paddingLen << " bytes" << endl;
  serializeString (value, os);

  if (paddingLen == 0)
    {
      // No padding
    }
  else
    {
      //std::cout << "+++++++++++ AvpLen = " << getUINT32From3ByteUINT8Array (length) << ", will pad for " << paddingLen << " bytes" << std::endl;
      serializePadding (paddingLen, os);
    }
}

void DiameterOctetStringAvp::deserialize (InputStream& in)
{
  UINT32 datalen = 0;
  deserializeUINT32 (code, in);
  deserializeUINT8  (flags, in);
  deserializeTo3ByteUnsignedChar (length, in);
  
  if (flags & DIAMETER_AVP_VENDOR_SPECIFIC_FLAG)
    {
      deserializeUINT32 (vendorId, in);
      datalen = getUINT32From3ByteUINT8Array (length) - 12;
    }
  else
    {
      datalen = getUINT32From3ByteUINT8Array (length) - 8;
    }
  
  deserializeString (value, in, datalen);
}
