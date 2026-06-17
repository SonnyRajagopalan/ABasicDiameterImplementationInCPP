#ifndef DIAMETER_AVP_H
#define DIAMETER_AVP_H

#include <iostream>
#include <string>
#include <sstream>
#include <list>

#include "common/Types.h"
#include "common/Util.h"
#include "common/DiameterConstants.h"

BasicString getAvpTypeString (UINT32);

class DiameterAvp // Abstract class.
{
 public:

  enum AvpType
    {
      OctetString      = 1,
      Integer32        = 2,
      Integer64        = 3,
      Unsigned32       = 4,
      Unsigned64       = 5,
      Float32          = 6,
      Float64          = 7,
      Grouped          = 8,
      Address          = 9,
      Time             = 10,
      UTF8String       = 11,
      DiamIdent        = 12,
      DiamURI          = 13,
      Enumerated       = 14,
      IPFiltrRule      = 15,
      QoSFilterRule    = 16
  };

  DiameterAvp ();
  DiameterAvp (UINT32& _code, UINT8& _flags, UINT32& _vendorId);
  DiameterAvp (BasicString& _name,
	       UINT32& _code,
	       UINT8& _must,
	       UINT8& _may,
	       UINT8& _shouldNot,
	       UINT8& _mustNot,
	       bool& _encr);
  virtual ~DiameterAvp ();

  inline DiameterAvp::AvpType getType () const { return type; }
  inline BasicString          getTypeString () const { return ::getAvpTypeString(type); }
  inline UINT32               getCode () const { return code; }
  inline UINT8                getFlags () const { return flags; }
  inline UINT8                getMust () const { return must; }
  inline UINT8                getMay () const { return may; }
  inline UINT8                getShouldNot () const { return shouldNot; }
  inline UINT8                getMustNot () const { return mustNot; }
  inline bool                 getEncr () const {return encr; }
  inline UINT32               getLength () const;
  inline UINT32               getVendorId () const { return vendorId; }
  inline BasicString          getName () const { return name; }

  inline void                 setType (DiameterAvp::AvpType _type) { type = _type; }
  inline void                 setCode (UINT32 _code) { code = _code; }
  inline void                 setFlags (UINT8 _flags) { flags = _flags; }
  inline void                 setMust (UINT8& _must) { must = _must; }
  inline void                 setMay (UINT8& _may) { may = _may; }
  inline void                 setShouldNot (UINT8& _shouldNot) { shouldNot = _shouldNot; }
  inline void                 setMustNot (UINT8& _mustNot) { mustNot = _mustNot; }
  inline void                 setEncr (bool& _encr) { encr = _encr; }
  inline void                 setLength (UINT8 _length[]);
  inline void                 setVendorId (UINT32 _vendorId) { vendorId = _vendorId; }
  inline void                 setName (BasicString _name) { name = _name; }

  inline bool                 isBasicType () const;
  inline bool                 isDerivedType () const ;

  inline void                 debug () 
    { 
      std::cout << "type (" << type << ") code (" << code << ") name (" << name;
      std::cout << ") must (" << std::hex << static_cast<int>(must);
      std::cout << ") may (" << std::hex << static_cast<int>(may);
      std::cout << ") mustNot (" << std::hex << static_cast<int>(mustNot) << ")";
      std::cout << std::dec << std::endl;
    }

  virtual void       serialize   (OutputStream& os) = 0;
  virtual void       deserialize (InputStream& in) = 0;

 protected: /**< Allow access from derived classes */

  /**
     Items pertinent to marshalling:

    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                           AVP Code                            |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |V M P r r r r r|                  AVP Length                   |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                        Vendor-ID (opt)                        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |    Data ...
   +-+-+-+-+-+-+-+-+

  */
  UINT32 code;
  UINT8  flags;
  /**
     From RFC 3588:
     The AVP Length field is three octets, and indicates the number of
     octets in this AVP including the AVP Code, AVP Length, AVP Flags,
     Vendor-ID field (if present) and the AVP data.  If a message is
     received with an invalid attribute length, the message SHOULD be
     rejected.
  */
  UINT8  length[3];
  UINT32 vendorId;
  /**
     Items not-pertinent to marshalling
  */
  AvpType     type;
  BasicString name;
  UINT8       must;
  UINT8       may;
  UINT8       shouldNot;
  UINT8       mustNot;
  bool        encr;

 private:
  // Nothing private -- 
};

/**
   @returns length   The total length of the AVP, including padding (per RFC. Check RFC 3588.)
*/
UINT32 DiameterAvp::getLength () const
{
  UINT32 len = 0;
  
  unsplit3ByteUINT8ArrayToUINT32 (len, length);

  return len;
}

/**
   @param _length    The total length of the AVP, including padding (per RFC. Check RFC 3588.)
*/
void DiameterAvp::setLength (UINT8 _length[])
{
  length[0] = _length[0];
  length[1] = _length[1];
  length[2] = _length[2];
}

bool DiameterAvp::isBasicType () const
{
  if (type <= 8)
    return true;
  else
    return false;
}

bool DiameterAvp::isDerivedType () const
{
  if (type > 8)
    return true;
  else
    return false;
}

#endif
