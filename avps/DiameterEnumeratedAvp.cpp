#include "avps/DiameterEnumeratedAvp.h"
#include "avps/DiameterInteger32Avp.h"

DiameterEnumeratedAvp::DiameterEnumeratedAvp () : DiameterInteger32Avp ()
{
}

DiameterEnumeratedAvp::DiameterEnumeratedAvp (UINT32 _code, 
					      UINT8 _flags, 
					      UINT32 _vendorId,
					      SINT32 _value) : 
  DiameterInteger32Avp (_code, _flags, _vendorId, _value)
{
  setType (DiameterAvp::Enumerated);

  //std::cout << "DiameterEnumeratedAvp constructor with args (code, flags, vendorId, value) called." << std::endl;
}

DiameterEnumeratedAvp::DiameterEnumeratedAvp (BasicString& _name, UINT32& _code,
					      UINT8& _must, UINT8& _may, UINT8& _shouldNot, 
					      UINT8& _mustNot, bool& _encr) : 
  DiameterInteger32Avp (_name, _code, _must, _may, _shouldNot, _mustNot, _encr)
{
  setType (DiameterAvp::Enumerated);

  //std::cout << "DiameterEnumeratedAvp dictionary constructor with args (name, code, <ALL FLAGS>) called." << std::endl;
}

DiameterEnumeratedAvp::~DiameterEnumeratedAvp ()
{
  //std::cout << "DiameterEnumeratedAvp destructor called." << std::endl;
}
