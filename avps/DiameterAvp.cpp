/**
   @file This is the implementation file for the DiameterAvp class.
   @author Sonny Rajagopalan. Sonny.Rajagopalan#Tekelec.com
   @details Implementaion file for the DiameterAvp class
   @date 09/14/2011
   @copyright Tekelec, 2010--2011.
*/

#include "avps/DiameterAvp.h"
#include <sstream>
#include <string>
#include <string.h>
#include <list>
#include <iostream>
#include <iomanip>
#include <arpa/inet.h>
#include <sstream>
#include <list>

/**
   Default constructor
*/
DiameterAvp::DiameterAvp()
{
  //std::cout << "DiameterAvp default constructor called." << std::endl;
}

/**
   Default destructor
*/
DiameterAvp::~DiameterAvp()
{
  //std::cout << "DiameterAvp destructor called." << std::endl;
}


/**
   The usual constructor: Called by all the derived classes to set basic AVP attributes
*/
DiameterAvp::DiameterAvp (UINT32& _code,
			  UINT8& _flags,
			  UINT32& _vendorId) : code (_code), 
					       flags (_flags),
					       vendorId (_vendorId)
{
  //std::cout << "DiameterAvp constructor with args (code, flags, vendorId) called." << std::endl;
}

/**
   The constructor called by the dictionary init: Called by all the derived classes to set basic AVP attributes
*/
DiameterAvp::DiameterAvp (BasicString& _name,
			  UINT32& _code,
			  UINT8& _must,
			  UINT8& _may,
			  UINT8& _shouldNot,
			  UINT8& _mustNot,
			  bool& _encr) : code (_code),
					 name (_name),
					 must (_must),
					 may (_may),
					 shouldNot (_shouldNot),
					 mustNot (_mustNot),
					 encr (_encr)
{
  //std::cout << "DiameterAvp dictionary constructor with args (name, code, <ALL FLAGS>) called." << std::endl;
}

BasicString getAvpTypeString (UINT32 type)
{
  switch (type)
    {
    case DiameterAvp::OctetString:
      return "OctetString";
    case DiameterAvp::Integer32:
      return "Integer32";
    case DiameterAvp::Integer64:
      return "Integer64";
    case DiameterAvp::Unsigned32:
      return "Unsigned32";
    case DiameterAvp::Unsigned64:
      return "Unsigned64";
    case DiameterAvp::Float32:
      return "Float32";
    case DiameterAvp::Float64:
      return "Float64";
    case DiameterAvp::Grouped:
      return "Grouped";
    case DiameterAvp::Address:
      return "Address";
    case DiameterAvp::Time:
      return "Time";
    case DiameterAvp::UTF8String:
      return "UTF8String";
    case DiameterAvp::DiamIdent:
      return "DiamIdent";
    case DiameterAvp::DiamURI:
      return "DiamURI";
    case DiameterAvp::Enumerated:
      return "Enumerated";
    case DiameterAvp::IPFiltrRule:
      return "IPFiltrRule";
    case DiameterAvp::QoSFilterRule:
      return "QoSFilterRule";
    default:
      return "ERROR";
    }
}
