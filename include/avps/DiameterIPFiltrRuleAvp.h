#ifndef DIAMETER_IPFILTRRULE_AVP_H
#define DIAMETER_IPFILTRRULE_AVP_H

//#include <iostream>
#include <string>
#include <sstream>

#include "DiameterAvp.h"
#include "DiameterOctetStringAvp.h"

class DiameterIPFiltrRuleAvp : public DiameterOctetStringAvp
{
 public:

  // Constructors and destructors
  DiameterIPFiltrRuleAvp() {}; // Default constructor
  DiameterIPFiltrRuleAvp(UINT32 _avpCode, UINT8 _avpFLags,
			 UINT32 _vendorId, std::string _value);
  DiameterIPFiltrRuleAvp (BasicString& _name, UINT32& _code,
			  UINT8& _must, UINT8& _may, UINT8& _shouldNot, UINT8& _mustNot, bool& _encr);
  ~DiameterIPFiltrRuleAvp();

 private:
  std::string value;
};

#endif
