#ifndef DIAMETER_OCTET_STRING_AVP_H
#define DIAMETER_OCTET_STRING_AVP_H

//#include <iostream>
#include <string>
#include <sstream>
#include <list>

#include "DiameterAvp.h"
#include "common/Debug.h"

class DiameterOctetStringAvp : public DiameterAvp
{
 public:

  // Constructors and destructors
  DiameterOctetStringAvp(); // Default constructor
  DiameterOctetStringAvp(UINT32 _avpCode, UINT8 _avpFLags,
			 UINT32 _vendorId, std::string _value);
  DiameterOctetStringAvp (BasicString& _name, UINT32& _code,
			  UINT8& _must, UINT8& _may, UINT8& _shouldNot, UINT8& _mustNot, bool& _encr);
  ~DiameterOctetStringAvp();

  // Getters and setters, used by derived classes as well
  inline  std::string getValue () { return value; };
  inline  void        setValue (std::string _value) 
  { 
    UINT8 bytes[3];
    UINT32 lenUINT32 = getLength ();
    //cout << "-------Size of AVP = " << lenUINT32 << endl;
    splitUINT32To3ByteUINT8Array ((lenUINT32 + _value.size ()), bytes); 
    value = _value; 
    setLength (bytes);
    //cout << "-------Size of AVP after adding [" << value << "] = " << getLength () << endl;
  }

  // Serialize and deserializers
  virtual void   serialize   (OutputStream& os);
  virtual void   deserialize (InputStream& in);

 private:
  std::string value;
};

#endif
