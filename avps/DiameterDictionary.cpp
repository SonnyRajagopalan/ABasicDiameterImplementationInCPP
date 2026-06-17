#include <map>
#include <string>
#include "avps/DiameterDictionary.h"
#include "avps/DiameterAvp.h"
#include "avps/DiameterInteger32Avp.h"
#include "avps/DiameterInteger64Avp.h"
#include "avps/DiameterUnsigned32Avp.h"
#include "avps/DiameterUnsigned64Avp.h"
#include "avps/DiameterFloat32Avp.h"
#include "avps/DiameterFloat64Avp.h"
#include "avps/DiameterEnumeratedAvp.h"
#include "avps/DiameterGroupedAvp.h"
#include "avps/DiameterOctetStringAvp.h"
#include "avps/DiameterUTF8StringAvp.h"
#include "avps/DiameterAddressAvp.h"
#include "avps/DiameterDiamIdentAvp.h"
#include "avps/DiameterDiamURIAvp.h"
#include "avps/DiameterIPFiltrRuleAvp.h"
#include "avps/DiameterQoSFilterRuleAvp.h"
#include "avps/DiameterTimeAvp.h"
#include "common/Debug.h"

DiameterDictionary* DiameterDictionary::instance = 0;
//map<UINT32, DiameterAvp*> DiameterDictionary::entries;

DiameterDictionary* DiameterDictionary::getInstance()
{
  if (!instance)
    {
      instance = new DiameterDictionary();
      //cout << "DiameterDictionary getInstance () called " << endl;
    }

  return instance;
}

DiameterDictionary::DiameterDictionary()
{
  init (); // Load all AVPs into dictionary
}

DiameterDictionary::~DiameterDictionary()
{
}

void DiameterDictionary::init()
{
  //cout << "Calling DiameterDictionary::init(), entries max size =  " << entries.max_size() << endl;
  /**
     RFC 3588 base protocol AVPs
  */
  addAvp ((char *)"Acct-Interim-Interval", 85, DiameterAvp::Unsigned32, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  //debugging (85);

  addAvp ((char *)"Accounting-Realtime-Required", 483, DiameterAvp::Enumerated, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Acct-Multi-Session-Id", 50, DiameterAvp::UTF8String, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Accounting-Record-Number", 485, DiameterAvp::Unsigned32, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Accounting-Record-Type", 480, DiameterAvp::Enumerated, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Accounting-Session-Id", 44, DiameterAvp::OctetString, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Accounting-Sub-Session-Id", 287, DiameterAvp::Unsigned64, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Acct-Application-Id", 259, DiameterAvp::Unsigned32, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  false /**< encryption */
	  );
  addAvp ((char *)"Auth-Application-Id", 258, DiameterAvp::Unsigned32, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  false /**< encryption */
	  );
  addAvp ((char *)"Auth-Request-Type", 274, DiameterAvp::Enumerated, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  false /**< encryption */
	  );
  addAvp ((char *)"Authorization-Lifetime", 291, DiameterAvp::Unsigned32, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  false /**< encryption */
	  );
  addAvp ((char *)"Auth-Grace-Period", 276, DiameterAvp::Unsigned32, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  false /**< encryption */
	  );
  addAvp ((char *)"Auth-Session-State", 277, DiameterAvp::Enumerated, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  false /**< encryption */
	  );
  addAvp ((char *)"Re-Auth-Request-Type", 285, DiameterAvp::Enumerated, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  false /**< encryption */
	  );
  addAvp ((char *)"Class", 25, DiameterAvp::OctetString, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Destination-Host", 293, DiameterAvp::DiamIdent, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  false /**< encryption */
	  );
  addAvp ((char *)"Destination-Realm", 283, DiameterAvp::DiamIdent, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  false /**< encryption */
	  );
  addAvp ((char *)"Disconnect-Cause", 273, DiameterAvp::Enumerated, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  false /**< encryption */
	  );
  addAvp ((char *)"E2E-Sequence", 300, DiameterAvp::Grouped,
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Error-Message", 281, DiameterAvp::UTF8String, 
	  DIAMETER_AVP_INVALID_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG | DIAMETER_AVP_MANDATORY_FLAG, /**< must not */
	  false /**< encryption */
	  );
  addAvp ((char *)"Error-Reporting-Host", 294, DiameterAvp::DiamIdent, 
	  DIAMETER_AVP_INVALID_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG | DIAMETER_AVP_MANDATORY_FLAG, /**< must not */
	  false /**< encryption */
	  );
  addAvp ((char *)"Event-Timestamp", 55, DiameterAvp::Time, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  false /**< encryption */
	  );
  addAvp ((char *)"Experimental-Result", 297, DiameterAvp::Grouped, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  false /**< encryption */
	  );
  addAvp ((char *)"Experimental-Result-Code", 298, DiameterAvp::Unsigned32, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  false /**< encryption */
	  );
  addAvp ((char *)"Failed-AVP", 279, DiameterAvp::Grouped, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  false /**< encryption */
	  );
  addAvp ((char *)"Firmware-Revision", 267, DiameterAvp::Unsigned32, 
	  DIAMETER_AVP_INVALID_FLAG, /**< must */
	  DIAMETER_AVP_INVALID_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_PBIT_FLAG | DIAMETER_AVP_VENDOR_SPECIFIC_FLAG | DIAMETER_AVP_MANDATORY_FLAG, /**< must not */
	  false /**< encryption */
	  );
  addAvp ((char *)"Host-IP-Address", 257, DiameterAvp::Address, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  false /**< encryption */
	  );
  addAvp ((char *)"Inband-Security-Id", 299, DiameterAvp::Unsigned32, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  false /**< encryption */
	  );
  addAvp ((char *)"Multi-Round-Time-Out", 272, DiameterAvp::Unsigned32, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Origin-Host", 264, DiameterAvp::DiamIdent, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  false /**< encryption */
	  );
  addAvp ((char *)"Origin-Realm", 296, DiameterAvp::DiamIdent, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  false /**< encryption */
	  );
  addAvp ((char *)"Origin-State-Id", 278, DiameterAvp::Unsigned32, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  false /**< encryption */
	  );
  addAvp ((char *)"Product-Name", 269, DiameterAvp::UTF8String, 
	  DIAMETER_AVP_INVALID_FLAG, /**< must */
	  DIAMETER_AVP_INVALID_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_PBIT_FLAG | DIAMETER_AVP_VENDOR_SPECIFIC_FLAG | DIAMETER_AVP_MANDATORY_FLAG, /**< must not */
	  false /**< encryption */
	  );
  addAvp ((char *)"Proxy-Host", 280, DiameterAvp::DiamIdent, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_INVALID_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_PBIT_FLAG | DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  false /**< encryption */
	  );
  addAvp ((char *)"Proxy-Info", 284, DiameterAvp::Grouped, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_INVALID_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_PBIT_FLAG | DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  false /**< encryption */
	  );
  addAvp ((char *)"Proxy-State", 33, DiameterAvp::OctetString, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_INVALID_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_PBIT_FLAG | DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  false /**< encryption */
	  );
  addAvp ((char *)"Redirect-Host", 292, DiameterAvp::DiamURI, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  false /**< encryption */
	  );
  addAvp ((char *)"Redirect-Host-Usage", 261, DiameterAvp::Enumerated, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  false /**< encryption */
	  );
  addAvp ((char *)"Redirect-Max-Cache-Time", 262, DiameterAvp::Unsigned32, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  false /**< encryption */
	  );
  addAvp ((char *)"Result-Code", 268, DiameterAvp::Unsigned32, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  false /**< encryption */
	  );
  addAvp ((char *)"Route-Record", 282, DiameterAvp::DiamIdent, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_INVALID_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_PBIT_FLAG | DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  false /**< encryption */
	  );
  addAvp ((char *)"Session-Id", 263, DiameterAvp::UTF8String, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Session-Timeout", 27, DiameterAvp::Unsigned32, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  false /**< encryption */
	  );
  addAvp ((char *)"Session-Binding", 270, DiameterAvp::Unsigned32, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Session-Server-Failover", 271, DiameterAvp::Enumerated, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Supported-Vendor-Id", 265, DiameterAvp::Unsigned32, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  false /**< encryption */
	  );
  addAvp ((char *)"Termination-Cause", 295, DiameterAvp::Enumerated, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  false /**< encryption */
	  );
  addAvp ((char *)"User-Name", 1, DiameterAvp::UTF8String, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Vendor-Id", 266, DiameterAvp::Unsigned32, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  false /**< encryption */
	  );
  addAvp ((char *)"Vendor-Specific-Application-Id", 260, DiameterAvp::Grouped, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  false /**< encryption */
	  );

  /**
     RFC 4006 AVPs
  */
  addAvp ((char *)"CC-Correlation-Id", 411, DiameterAvp::OctetString, 
	  DIAMETER_AVP_INVALID_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG | DIAMETER_AVP_MANDATORY_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"CC-Input-Octets", 412, DiameterAvp::Unsigned64, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"CC-Money", 413, DiameterAvp::Grouped, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"CC-Output-Octets", 414, DiameterAvp::Unsigned64, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"CC-Request-Number", 415, DiameterAvp::Unsigned32, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"CC-Request-Type", 416, DiameterAvp::Enumerated, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"CC-Service-Specific-Units", 417, DiameterAvp::Unsigned64, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"CC-Session-Failover", 418, DiameterAvp::Enumerated, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"CC-Sub-Session-Id", 419, DiameterAvp::Unsigned64, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"CC-Time", 420, DiameterAvp::Unsigned32, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"CC-Total-Octets", 421, DiameterAvp::Unsigned64, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"CC-Unit-Type", 454, DiameterAvp::Enumerated, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Check-Balance-Result", 422, DiameterAvp::Enumerated, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Cost-Information", 423, DiameterAvp::Grouped, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Cost-Unit", 424, DiameterAvp::UTF8String, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Credit-Control", 426, DiameterAvp::Enumerated, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Credit-Control-Failure-Handling", 427, DiameterAvp::Enumerated, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Currency-Code", 425, DiameterAvp::Unsigned32, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Direct-Debiting-Failure-Handling", 428, DiameterAvp::Enumerated, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Exponent", 429, DiameterAvp::Integer32, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Final-Unit-Action", 449, DiameterAvp::Enumerated, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Final-Unit-Indication", 430, DiameterAvp::Grouped, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Granted-Service-Unit", 431, DiameterAvp::Grouped, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"G-S-U-Pool-Identifier", 453, DiameterAvp::Unsigned32, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"G-S-U-Pool-Reference", 457, DiameterAvp::Grouped, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Multiple-Services-Credit-Control", 456, DiameterAvp::Grouped, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Multiple-Services-Indicator", 455, DiameterAvp::Enumerated, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Rating-Group", 432, DiameterAvp::Unsigned32, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Redirect-Address-Type", 433, DiameterAvp::Enumerated, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Redirect-Server", 434, DiameterAvp::Grouped, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Redirect-Server-Address", 435, DiameterAvp::UTF8String, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Requested-Action", 436, DiameterAvp::Enumerated, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Requested-Service-Unit", 437, DiameterAvp::Grouped, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Restriction-Filter-Rule", 438, DiameterAvp::IPFiltrRule, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Service-Context-Id", 461, DiameterAvp::UTF8String, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Service-Identifier", 439, DiameterAvp::Unsigned32, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Service-Parameter-Info", 440, DiameterAvp::Grouped, 
	  DIAMETER_AVP_INVALID_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG | DIAMETER_AVP_MANDATORY_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Service-Parameter-Type", 441, DiameterAvp::Unsigned32, 
	  DIAMETER_AVP_INVALID_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG | DIAMETER_AVP_MANDATORY_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Service-Parameter-Value", 442, DiameterAvp::OctetString, 
	  DIAMETER_AVP_INVALID_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG | DIAMETER_AVP_MANDATORY_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Subscription-Id", 443, DiameterAvp::Grouped, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Subscription-Id-Data", 444, DiameterAvp::UTF8String, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Subscription-Id-Type", 450, DiameterAvp::Enumerated, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Tariff-Change-Usage", 452, DiameterAvp::Enumerated, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Tariff-Time-Change", 451, DiameterAvp::Time, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Unit-Value", 445, DiameterAvp::Grouped, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Used-Service-Unit", 446, DiameterAvp::Grouped, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"User-Equipment-Info", 458, DiameterAvp::Grouped, 
	  DIAMETER_AVP_INVALID_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG | DIAMETER_AVP_MANDATORY_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"User-Equipment-Info-Type", 459, DiameterAvp::Enumerated, 
	  DIAMETER_AVP_INVALID_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG | DIAMETER_AVP_MANDATORY_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"User-Equipment-Info-Value", 460, DiameterAvp::OctetString, 
	  DIAMETER_AVP_INVALID_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG | DIAMETER_AVP_MANDATORY_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Value-Digits", 447, DiameterAvp::Integer64, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );
  addAvp ((char *)"Validity-Time", 448, DiameterAvp::Unsigned32, 
	  DIAMETER_AVP_MANDATORY_FLAG, /**< must */
	  DIAMETER_AVP_PBIT_FLAG, /**< may */
	  DIAMETER_AVP_INVALID_FLAG, /**< should not */
	  DIAMETER_AVP_VENDOR_SPECIFIC_FLAG, /**< must not */
	  true /**< encryption */
	  );

  //cout << "End DiameterDictionary::init(), entries max size =  " << entries.max_size() << endl;

  //debugging (85);
}

void DiameterDictionary::addAvp (char* name, UINT32 _code, DiameterAvp::AvpType type, 
				 UINT32 must, 
				 UINT32 may, 
				 UINT32 shouldNot, 
				 UINT32 mustNot, 
				 bool _encr)
{
  BasicString _name (name);
  UINT8  _must = (UINT8) must;
  UINT8  _may  = (UINT8) may;
  UINT8  _shouldNot = (UINT8) shouldNot;
  UINT8  _mustNot = (UINT8) mustNot;

  switch (type)
    {
    case DiameterAvp::OctetString:
      {	
	DiameterOctetStringAvp* avp = new DiameterOctetStringAvp (_name, _code, _must, _may, _shouldNot, _mustNot, _encr);
	entries.insert (pair<UINT32, DiameterAvp*>(_code, avp));

	break;
      }
    case DiameterAvp::Integer32:
      {
	DiameterInteger32Avp* avp = new DiameterInteger32Avp (_name, _code, _must, _may, _shouldNot, _mustNot, _encr);
	entries.insert (pair<UINT32, DiameterAvp*>(_code, avp));
	//cout << "Inserted [" << _name << "] AVP of code " << _code << endl;
	break;
      }
    case DiameterAvp::Integer64:
      {
	DiameterInteger64Avp* avp = new DiameterInteger64Avp (_name, _code, _must, _may, _shouldNot, _mustNot, _encr);
	entries.insert (pair<UINT32, DiameterAvp*>(_code, avp));

	break;
      }
    case DiameterAvp::Unsigned32:
      {
	DiameterUnsigned32Avp* avp = new DiameterUnsigned32Avp (_name, _code, _must, _may, _shouldNot, _mustNot, _encr);
	entries.insert (pair<UINT32, DiameterAvp*>(_code, avp));

	//cout << "Inserted [" << _name << "] AVP of code ----------" << avp->getCode() << endl; avp->debug();
	break;
      }
    case DiameterAvp::Unsigned64:
      {
	DiameterUnsigned64Avp* avp = new DiameterUnsigned64Avp (_name, _code, _must, _may, _shouldNot, _mustNot, _encr);
	entries.insert (pair<UINT32, DiameterAvp*>(_code, avp));

	break;
      }
    case DiameterAvp::Float32:
      {
	DiameterFloat32Avp* avp = new DiameterFloat32Avp (_name, _code, _must, _may, _shouldNot, _mustNot, _encr);
	entries.insert (pair<UINT32, DiameterAvp*>(_code, avp));

	break;
      }
    case DiameterAvp::Float64:
      {
	DiameterFloat64Avp* avp = new DiameterFloat64Avp (_name, _code, _must, _may, _shouldNot, _mustNot, _encr);
	entries.insert (pair<UINT32, DiameterAvp*>(_code, avp));

	break;
      }
    case DiameterAvp::Grouped:
      {
	DiameterGroupedAvp* avp = new DiameterGroupedAvp (_name, _code, _must, _may, _shouldNot, _mustNot, _encr);
	entries.insert (pair<UINT32, DiameterAvp*>(_code, avp));

	break;
      }
    case DiameterAvp::Address:
      {
	DiameterAddressAvp* avp = new DiameterAddressAvp (_name, _code, _must, _may, _shouldNot, _mustNot, _encr);
	entries.insert (pair<UINT32, DiameterAvp*>(_code, avp));

	break;
      }
    case DiameterAvp::Time:
      {
	DiameterTimeAvp* avp = new DiameterTimeAvp (_name, _code, _must, _may, _shouldNot, _mustNot, _encr);
	entries.insert (pair<UINT32, DiameterAvp*>(_code, avp));

	break;
      }
    case DiameterAvp::UTF8String:
      {
	DiameterUTF8StringAvp* avp = new DiameterUTF8StringAvp (_name, _code, _must, _may, _shouldNot, _mustNot, _encr);
	entries.insert (pair<UINT32, DiameterAvp*>(_code, avp));

	break;
      }
    case DiameterAvp::DiamIdent:
      {
	DiameterDiamIdentAvp* avp = new DiameterDiamIdentAvp (_name, _code, _must, _may, _shouldNot, _mustNot, _encr);
	entries.insert (pair<UINT32, DiameterAvp*>(_code, avp));

	break;
      }
    case DiameterAvp::DiamURI:
      {
	DiameterDiamURIAvp* avp = new DiameterDiamURIAvp (_name, _code, _must, _may, _shouldNot, _mustNot, _encr);
	entries.insert (pair<UINT32, DiameterAvp*>(_code, avp));

	break;
      }
    case DiameterAvp::Enumerated:
      {
	DiameterEnumeratedAvp* avp = new DiameterEnumeratedAvp (_name, _code, _must, _may, _shouldNot, _mustNot, _encr);
	entries.insert (pair<UINT32, DiameterAvp*>(_code, avp));

	break;
      }
    case DiameterAvp::IPFiltrRule:
      {
	DiameterIPFiltrRuleAvp* avp = new DiameterIPFiltrRuleAvp (_name, _code, _must, _may, _shouldNot, _mustNot, _encr);
	entries.insert (pair<UINT32, DiameterAvp*>(_code, avp));

	break;
      }
    case DiameterAvp::QoSFilterRule:
      {
	DiameterQoSFilterRuleAvp* avp = new DiameterQoSFilterRuleAvp (_name, _code, _must, _may, _shouldNot, _mustNot, _encr);
	entries.insert (pair<UINT32, DiameterAvp*>(_code, avp));

	break;
      }
    }
}

void DiameterDictionary::debugging (UINT32 code)
{
  map<UINT32, DiameterAvp*>::iterator it = entries.find (code);
  DiameterAvp* avp = it->second;
  cout << "DiameterDictionary::debugging (): first = " << it->first << ", second = " << avp << endl;
  avp->debug();
}
