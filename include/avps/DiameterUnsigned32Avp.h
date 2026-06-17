#ifndef DIAMETER_UNSIGNED_32_AVP_H
#define DIAMETER_UNSIGNED_32_AVP_H

//#include <iostream>
#include <string>
#include <sstream>
#include <list>

#include "DiameterAvp.h"

class DiameterUnsigned32Avp : public DiameterAvp
{
 public:

  // Constructors and destructors
  DiameterUnsigned32Avp (); // Default constructor
  DiameterUnsigned32Avp (UINT32 _avpCode, UINT8 _avpFLags,
			 UINT32 _vendorId, UINT32 _value);
  DiameterUnsigned32Avp (BasicString& _name, UINT32& _code,
			 UINT8& _must, UINT8& _may, UINT8& _shouldNot, UINT8& _mustNot, bool& _encr);
  ~DiameterUnsigned32Avp ();

  // Getters and setters
  inline  UINT32 getValue () { return value; };
  inline  void   setValue (UINT32 _value) { value = _value; };

  // Serialize and deserializers
  virtual void   serialize   (OutputStream& os);
  virtual void   deserialize (InputStream& in);

 private:
  UINT32 value;
};

#endif
