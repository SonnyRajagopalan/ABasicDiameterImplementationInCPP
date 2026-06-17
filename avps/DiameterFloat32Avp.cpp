#include "avps/DiameterFloat32Avp.h"

DiameterFloat32Avp::DiameterFloat32Avp () : DiameterAvp ()
{
}

DiameterFloat32Avp::DiameterFloat32Avp (UINT32 _code, 
					UINT8 _flags, 
					UINT32 _vendorId,
					FLT32 _value) : 
  DiameterAvp (_code, _flags, _vendorId)
{
  UINT32 len = 0;
  UINT8 bytes[3];

  if (flags & DIAMETER_AVP_VENDOR_SPECIFIC_FLAG)
    len = 16;
  else
    len = 12;

  splitUINT32To3ByteUINT8Array (len, bytes);
  setLength (bytes);
  setValue (_value);
  setType (DiameterAvp::Float32);

  //std::cout << "DiameterFloat32Avp constructor with args (code, flags, vendorId, value) called." << std::endl;
}

DiameterFloat32Avp::DiameterFloat32Avp (BasicString& _name, UINT32& _code,
					UINT8& _must, UINT8& _may, UINT8& _shouldNot, 
					UINT8& _mustNot, bool& _encr) : 
  DiameterAvp (_name, _code, _must, _may, _shouldNot, _mustNot, _encr)
{
  setType (DiameterAvp::Float32);

  //std::cout << "DiameterFloat32Avp dictionary constructor with args (name, code, <ALL FLAGS>) called." << std::endl;
}

DiameterFloat32Avp::~DiameterFloat32Avp ()
{
  //std::cout << "DiameterFloat32Avp destructor called." << std::endl;
}

void DiameterFloat32Avp::serialize (OutputStream& os)
{
  serializeUINT32 (code, os);
  serializeUINT8  (flags, os);
  serialize3ByteUnsignedChar (length, os);

  if (flags & DIAMETER_AVP_VENDOR_SPECIFIC_FLAG)
    serializeUINT32 (vendorId, os);

  serializeFLT32 (value, os);
}

void DiameterFloat32Avp::deserialize (InputStream& in)
{
  deserializeUINT32 (code, in);
  deserializeUINT8  (flags, in);
  deserializeTo3ByteUnsignedChar (length, in);
  
  if (flags & DIAMETER_AVP_VENDOR_SPECIFIC_FLAG)
    deserializeUINT32 (vendorId, in);
  
  deserializeFLT32 (value, in);
}
