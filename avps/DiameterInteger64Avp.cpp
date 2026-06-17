#include "avps/DiameterInteger64Avp.h"

DiameterInteger64Avp::DiameterInteger64Avp () : DiameterAvp ()
{
}

DiameterInteger64Avp::DiameterInteger64Avp (UINT32 _code, 
					    UINT8 _flags, 
					    UINT32 _vendorId,
					    SINT64 _value) : 
  DiameterAvp (_code, _flags, _vendorId)
{
  UINT32 len = 0;
  UINT8 bytes[3];
 
  if (flags & DIAMETER_AVP_VENDOR_SPECIFIC_FLAG)
    len = 20;
  else
    len = 16;

  splitUINT32To3ByteUINT8Array (len, bytes);
  setLength (bytes);
  setValue (_value);
  setType (DiameterAvp::Integer64);

  //std::cout << "DiameterInteger64Avp constructor with args (code, flags, vendorId, value) called." << std::endl;
}

DiameterInteger64Avp::DiameterInteger64Avp (BasicString& _name, UINT32& _code,
					    UINT8& _must, UINT8& _may, UINT8& _shouldNot, 
					    UINT8& _mustNot, bool& _encr) : 
  DiameterAvp (_name, _code, _must, _may, _shouldNot, _mustNot, _encr)
{
  setType (DiameterAvp::Integer64);

  //std::cout << "DiameterInteger64Avp dictionary constructor with args (name, code, <ALL FLAGS>) called." << std::endl;
}

DiameterInteger64Avp::~DiameterInteger64Avp ()
{
  //std::cout << "DiameterInteger64Avp destructor called." << std::endl;
}

void DiameterInteger64Avp::serialize (OutputStream& os)
{
  serializeUINT32 (code, os);
  serializeUINT8  (flags, os);
  serialize3ByteUnsignedChar (length, os);

  if (flags & DIAMETER_AVP_VENDOR_SPECIFIC_FLAG)
    serializeUINT32 (vendorId, os);

  serializeSINT32 (value.int64s.msb, os);
  serializeUINT32 (value.int64s.lsb, os);
}

void DiameterInteger64Avp::deserialize (InputStream& in)
{
  deserializeUINT32 (code, in);
  deserializeUINT8  (flags, in);
  deserializeTo3ByteUnsignedChar (length, in);
  
  if (flags & DIAMETER_AVP_VENDOR_SPECIFIC_FLAG)
    deserializeUINT32 (vendorId, in);
  
  deserializeSINT32 (value.int64s.msb, in);
  deserializeUINT32 (value.int64s.lsb, in);
}
