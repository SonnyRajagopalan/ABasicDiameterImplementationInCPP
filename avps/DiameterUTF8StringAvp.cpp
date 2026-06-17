#include <string>
#include "avps/DiameterUTF8StringAvp.h"

DiameterUTF8StringAvp::DiameterUTF8StringAvp(UINT32 _code, 
					     UINT8 _flags, 
					     UINT32 _vendorId,
					     std::string _value) : 
  DiameterOctetStringAvp (_code, _flags, _vendorId, _value)
{
  setType (DiameterAvp::UTF8String);

  //std::cout << "DiameterUTF8StringAvp constructor with args (code, flags, vendorId, value) called." << std::endl;
}

DiameterUTF8StringAvp::DiameterUTF8StringAvp (BasicString& _name, UINT32& _code,
					      UINT8& _must, UINT8& _may, UINT8& _shouldNot, 
					      UINT8& _mustNot,  bool& _encr) : 
  DiameterOctetStringAvp (_name, _code, _must, _may, _shouldNot, _mustNot, _encr)
{
  setType (DiameterAvp::UTF8String);

  //std::cout << "DiameterUTF8StringAvp dictionary constructor with args (name, code, <ALL FLAGS>) called." << std::endl;
}

DiameterUTF8StringAvp::~DiameterUTF8StringAvp ()
{
  //std::cout << "DiameterUTF8StringAvp destructor called." << std::endl;
}
