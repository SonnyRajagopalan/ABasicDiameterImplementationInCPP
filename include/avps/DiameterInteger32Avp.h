#ifndef DIAMETER_INTEGER_32_AVP_H
#define DIAMETER_INTEGER_32_AVP_H

//#include <iostream>
#include <string>
#include <sstream>
#include <list>
#include "DiameterAvp.h"

class DiameterInteger32Avp : public DiameterAvp
{
 public:

  // Constructors and destructors
  DiameterInteger32Avp (); // Default constructor
  DiameterInteger32Avp (UINT32 _code, UINT8 _flags,
			UINT32 _vendorId, SINT32 _value);
  DiameterInteger32Avp (BasicString& _name, UINT32& _code,
			UINT8& _must, UINT8& _may, UINT8& _shouldNot, UINT8& _mustNot, bool& _encr);
  ~DiameterInteger32Avp ();

  // Getters and setters
  inline  SINT32 getValue () { return value; };
  inline  void   setValue (SINT32 _value) { value = _value; };

  // Serialize and deserializers
  virtual void   serialize   (OutputStream& os);
  virtual void   deserialize (InputStream& in);

 private:
  SINT32 value;
};

#endif
