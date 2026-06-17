#ifndef DIAMETER_FLOAT_32_AVP_H
#define DIAMETER_FLOAT_32_AVP_H

//#include <iostream>
#include <string>
#include <sstream>

#include "DiameterAvp.h"
#include <list>

class DiameterFloat32Avp : public DiameterAvp
{
 public:

  // Constructors and destructors
  DiameterFloat32Avp (); // Default constructor
  DiameterFloat32Avp (UINT32 _avpCode, UINT8 _avpFLags,
		      UINT32 _vendorId, FLT32 _value);
  DiameterFloat32Avp (BasicString& _name, UINT32& _code,
		      UINT8& _must, UINT8& _may, UINT8& _shouldNot, UINT8& _mustNot, bool& _encr);
  ~DiameterFloat32Avp ();

  // Getters and setters
  inline  FLT32 getValue () { return value; };
  inline  void   setValue (FLT32 _value) { value = _value; };

  // Serialize and deserializers
  virtual void   serialize   (OutputStream& os);
  virtual void   deserialize (InputStream& in);

 private:
  FLT32 value;
};

#endif
