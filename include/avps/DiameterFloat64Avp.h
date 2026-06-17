#ifndef DIAMETER_FLOAT_64_AVP_H
#define DIAMETER_FLOAT_64_AVP_H

//#include <iostream>
#include <string>
#include <sstream>

#include "DiameterAvp.h"
#include <list>

class DiameterFloat64Avp : public DiameterAvp
{
 public:

  // Constructors and destructors
  DiameterFloat64Avp (); // Default constructor
  DiameterFloat64Avp (UINT32 _avpCode, UINT8 _avpFLags,
		      UINT32 _vendorId, FLT64 _value);
  DiameterFloat64Avp (BasicString& _name, UINT32& _code,
		      UINT8& _must, UINT8& _may, UINT8& _shouldNot, UINT8& _mustNot, bool& _encr);
  ~DiameterFloat64Avp ();

  // Getters and setters
  inline  FLT64 getValue () { return value; };
  inline  void   setValue (FLT64 _value) { value = _value; };

  // Serialize and deserializers
  virtual void   serialize   (OutputStream& os);
  virtual void   deserialize (InputStream& in);

 private:
  FLT64 value;
};

#endif
