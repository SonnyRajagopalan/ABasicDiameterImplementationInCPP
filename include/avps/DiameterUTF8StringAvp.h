#ifndef DIAMETER_UTF8STRING_AVP_H
#define DIAMETER_UTF8STRING_AVP_H

//#include <iostream>
#include <string>
#include <sstream>

#include "DiameterAvp.h"
#include "DiameterOctetStringAvp.h"

class DiameterUTF8StringAvp : public DiameterOctetStringAvp
{
 public:

  // Constructors and destructors
  DiameterUTF8StringAvp() {}; // Default constructor
  DiameterUTF8StringAvp(UINT32 _avpCode, UINT8 _avpFLags,
			UINT32 _vendorId, std::string _value);
  DiameterUTF8StringAvp (BasicString& _name, UINT32& _code,
			 UINT8& _must, UINT8& _may, UINT8& _shouldNot, UINT8& _mustNot, bool& _encr);
  ~DiameterUTF8StringAvp();

 private:
  std::string value;
};

#endif
