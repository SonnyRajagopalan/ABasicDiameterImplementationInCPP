#ifndef DIAMETER_INTEGER_64_AVP_H
#define DIAMETER_INTEGER_64_AVP_H

//#include <iostream>
#include <string>
#include <sstream>
#include <list>

#include "DiameterAvp.h"

class DiameterInteger64Avp : public DiameterAvp
{
 public:

  // Constructors and destructors
  DiameterInteger64Avp (); // Default constructor
  DiameterInteger64Avp (UINT32 _code, UINT8 _flags,
			UINT32 _vendorId, SINT64 _value);
  DiameterInteger64Avp (BasicString& _name, UINT32& _code,
			UINT8& _must, UINT8& _may, UINT8& _shouldNot, UINT8& _mustNot, bool& _encr);
  ~DiameterInteger64Avp ();

  // Getters and setters
  inline  SINT64 getValue () { return value; };
  inline  void   setValue (SINT64 _value) { value = _value; };

  // Serialize and deserializers
  virtual void   serialize   (OutputStream& os);
  virtual void   deserialize (InputStream& in);

 private:
  SINT64 value;
};

#endif
