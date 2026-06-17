#ifndef DIAMETER_ADDRESS_AVP_H
#define DIAMETER_ADDRESS_AVP_H

//#include <iostream>
#include <string>
#include <sstream>

#include "DiameterAvp.h"
#include "DiameterOctetStringAvp.h"

class DiameterAddressAvp : public DiameterOctetStringAvp
{
 public:

  // Constructors and destructors
  DiameterAddressAvp () {}; // Default constructor
  DiameterAddressAvp (UINT32 _code, UINT8 _fLags,
		      UINT32 _vendorId, std::string _value);
  DiameterAddressAvp (BasicString& _name, UINT32& _code,
		      UINT8& _must, UINT8& _may, UINT8& _shouldNot, UINT8& _mustNot, bool& _encr);
  ~DiameterAddressAvp ();

 private:
  std::string value;
};

#endif
