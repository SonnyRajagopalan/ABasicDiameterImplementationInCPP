#include <string>
#include "avps/DiameterQoSFilterRuleAvp.h"

DiameterQoSFilterRuleAvp::DiameterQoSFilterRuleAvp(UINT32 _code, 
						   UINT8 _flags, 
						   UINT32 _vendorId,
						   std::string _value) : 
  DiameterOctetStringAvp (_code, _flags, _vendorId, _value)
{
  setType (DiameterAvp::QoSFilterRule);

  //std::cout << "DiameterQoSFilterRuleAvp constructor with args (code, flags, vendorId, value) called." << std::endl;
}

DiameterQoSFilterRuleAvp::DiameterQoSFilterRuleAvp (BasicString& _name, UINT32& _code,
						    UINT8& _must, UINT8& _may, UINT8& _shouldNot, 
						    UINT8& _mustNot,  bool& _encr) : 
  DiameterOctetStringAvp (_name, _code, _must, _may, _shouldNot, _mustNot, _encr)
{
  setType (DiameterAvp::QoSFilterRule);

  //std::cout << "DiameterQoSFilterRuleAvp dictionary constructor with args (name, code, <ALL FLAGS>) called." << std::endl;
}

DiameterQoSFilterRuleAvp::~DiameterQoSFilterRuleAvp ()
{
  //std::cout << "DiameterQoSFilterRuleAvp destructor called." << std::endl;
}
