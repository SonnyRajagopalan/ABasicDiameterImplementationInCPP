#ifndef DIAMETER_UNSIGNED_64_AVP_H
#define DIAMETER_UNSIGNED_64_AVP_H

//#include <iostream>
#include <string>
#include <sstream>
#include <list>

#include "DiameterAvp.h"

class DiameterUnsigned64Avp : public DiameterAvp
{
 public:

  // Constructors and destructors
  DiameterUnsigned64Avp (); // Default constructor
  DiameterUnsigned64Avp (UINT32 _avpCode, UINT8 _avpFLags,
			 UINT32 _vendorId, UINT64 _value);
  DiameterUnsigned64Avp (BasicString& _name, UINT32& _code,
			 UINT8& _must, UINT8& _may, UINT8& _shouldNot, UINT8& _mustNot, bool& _encr);
  ~DiameterUnsigned64Avp ();

  // Getters and setters
  inline  UINT64 getValue () { return value; };
  inline  void   setValue (UINT64 _value) { value = _value; };

  // Serialize and deserializers
  virtual void   serialize   (OutputStream& os);
  virtual void   deserialize (InputStream& in);

 private:
  UINT64 value;
};

#endif
