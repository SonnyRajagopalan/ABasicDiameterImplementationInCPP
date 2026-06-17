#ifndef DIAMETER_GROUPED_AVP_H
#define DIAMETER_GROUPED_AVP_H

//#include <iostream>
#include <string>
#include <sstream>
#include <list>

#include "DiameterAvp.h"

class DiameterGroupedAvp : public DiameterAvp
{
 public:

  // Constructors and destructors
  DiameterGroupedAvp (); // Default constructor
  DiameterGroupedAvp (UINT32 _code, UINT8 _fLags,
		      UINT32 _vendorId, std::list<DiameterAvp*> _value);
  DiameterGroupedAvp (BasicString& _name, UINT32& _code,
		      UINT8& _must, UINT8& _may, UINT8& _shouldNot, UINT8& _mustNot, bool& _encr);
  ~DiameterGroupedAvp ();

  // Getters and setters
  inline  std::list<DiameterAvp*> getValue () { return value; };
  inline  void   setValue (std::list<DiameterAvp*> _value) { value = _value; };

  // AVP management
  void addAnAvp(DiameterAvp* _anAvp);
  // Serialize and deserializers
  virtual void   serialize   (OutputStream& os);
  virtual void   deserialize (InputStream& in);

 private:
  std::list<DiameterAvp*> value;
};

#endif
