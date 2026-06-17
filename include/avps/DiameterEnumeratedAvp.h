#ifndef DIAMETER_ENUMERATED_AVP_H
#define DIAMETER_ENUMERATED_AVP_H

//#include <iostream>
#include <string>
#include <sstream>
#include <list>

#include "DiameterInteger32Avp.h"

class DiameterEnumeratedAvp : public DiameterInteger32Avp
{
 public:

  // Constructors and destructors
  DiameterEnumeratedAvp (); // Default constructor
  DiameterEnumeratedAvp (UINT32 _avpCode, UINT8 _avpFLags,
			 UINT32 _vendorId, SINT32 _value);
  DiameterEnumeratedAvp (BasicString& _name, UINT32& _code,
			 UINT8& _must, UINT8& _may, UINT8& _shouldNot, UINT8& _mustNot, bool& _encr);
  ~DiameterEnumeratedAvp ();

 private:
  // Should preferably have ways of validating this value, but 
  // we won't have these complications now.
  SINT32 value;
};

#endif
