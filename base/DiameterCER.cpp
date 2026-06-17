#include "messages/DiameterCER.h"
#include "avps/DiameterOctetStringAvp.h"
#include "avps/DiameterDiamIdentAvp.h"
#include "avps/DiameterAddressAvp.h"
#include "avps/DiameterAddressAvp.h"
#include "avps/DiameterUnsigned32Avp.h"
#include "avps/DiameterUTF8StringAvp.h"
#include "avps/DiameterGroupedAvp.h"
#include "messages/DiameterCommandCodes.h"

DiameterCER::DiameterCER ()
{
  dict = DiameterDictionary::getInstance ();
  setVersion (DIAMETER_VERSION);
  setMessageLength (DIAMETER_HEADER_SIZE);
  setFlags (DIAMETER_AVP_MANDATORY_FLAG);
  setCommandCode (DIAMETER_CER);
  setApplicationId (12);
  setHopByHopId (34);
  setEndToEndId (56);
  initialize ();
}

DiameterCER::~DiameterCER ()
{
}

DiameterCER::DiameterCER (UINT8 _version,
			  UINT8 _flags,
			  UINT32 _applicationId,
			  UINT32 _hopByHopId,
			  UINT32 _endToEndId) :
  DiameterMessage (_version, DIAMETER_HEADER_SIZE, _flags, DIAMETER_CER, _applicationId, _hopByHopId, _endToEndId)
{
  //DiameterOctetStringAvp* avp = new DiameterOctetStringAvp (44, 0x40, 0, "Octet string avp");
  initialize ();
}

void DiameterCER::serialize (OutputStream& os)
{
  header.serialize (os);
  avpList.serialize (os);
}

void DiameterCER::deserialize (InputStream& in)
{
  //header.deserialize (in);
  avpList.deserialize (in);
}

void DiameterCER::initialize ()
{
  /**
     Initialize with the "base AVPs" as per RFC 3588, Section 5.3.1:

           <CER> ::= < Diameter Header: 257, REQ >
                { Origin-Host }
                { Origin-Realm }
             1* { Host-IP-Address }
                { Vendor-Id }
                { Product-Name }
                [ Origin-State-Id ]
              * [ Supported-Vendor-Id ]
              * [ Auth-Application-Id ]
              * [ Inband-Security-Id ]
              * [ Acct-Application-Id ]
              * [ Vendor-Specific-Application-Id ]
                [ Firmware-Revision ]
              * [ AVP ]
  */
  // Can appear anywhere in message, exactly one
  DiameterDiamIdentAvp* originHost = new DiameterDiamIdentAvp (ORIGIN_HOST, DIAMETER_AVP_MANDATORY_FLAG, 0, getMyHostname().c_str());
  addAnAvp (originHost); 
  
  // Can appear anywhere in message, exactly one
  DiameterDiamIdentAvp* originRealm = new DiameterDiamIdentAvp (ORIGIN_REALM, DIAMETER_AVP_MANDATORY_FLAG, 0, "sonnysrealm.info");
  addAnAvp (originRealm); 

  // Can appear anywhere in message, one minimum, unbounded maximum.
  DiameterAddressAvp* hostIpAddress = new DiameterAddressAvp (HOST_IP_ADDRESS, DIAMETER_AVP_MANDATORY_FLAG, 0, getMyIPAddress().c_str());
  addAnAvp (hostIpAddress); 

  // Can appear anywhere in message, exactly one
  DiameterUnsigned32Avp* vendorId = new DiameterUnsigned32Avp (VENDOR_ID, DIAMETER_AVP_MANDATORY_FLAG, 0, 42);
  addAnAvp (vendorId); 

  // Can appear anywhere in message, exactly one
  DiameterUTF8StringAvp* productName = new DiameterUTF8StringAvp (PRODUCT_NAME, DIAMETER_AVP_MANDATORY_FLAG, 0, PRODUCT_NAME_DEFAULT);
  addAnAvp (productName); 

  // Optional, can appear anywhere in the message, zero or one may be present
  DiameterUnsigned32Avp* originStateId = new DiameterUnsigned32Avp (ORIGIN_STATE_ID, DIAMETER_AVP_MANDATORY_FLAG, 0, 2);
  addAnAvp (originStateId); 

  // Optional, can appear anywhere in the message, zero or any number may be present
  DiameterUnsigned32Avp* supportedVendorId = new DiameterUnsigned32Avp (SUPPORTED_VENDOR_ID, DIAMETER_AVP_MANDATORY_FLAG, 0, 56);
  addAnAvp (supportedVendorId); 
  
  // Optional, can appear anywhere in the message, zero or any number may be present
  DiameterUnsigned32Avp* authApplicationId = new DiameterUnsigned32Avp (AUTH_APP_ID, DIAMETER_AVP_MANDATORY_FLAG, 0, 10);
  addAnAvp (authApplicationId); 

  // Optional, can appear anywhere in the message, zero or any number may be present
  DiameterUnsigned32Avp* inbandSecurityId = new DiameterUnsigned32Avp (INBAND_SECURITY_ID, DIAMETER_AVP_MANDATORY_FLAG, 0, 0);
  addAnAvp (inbandSecurityId); 

  // Optional, can appear anywhere in the message, zero or any number may be present
  DiameterUnsigned32Avp* acctApplicationId = new DiameterUnsigned32Avp (ACCT_APP_ID, DIAMETER_AVP_MANDATORY_FLAG, 0, 0);
  addAnAvp (acctApplicationId); 

  // Optional, can appear anywhere in the message, zero or any number may be present
  std::list<DiameterAvp*> nullList;
  DiameterGroupedAvp* vendorSpecificApplicationId = new DiameterGroupedAvp (VENDOR_SPECIFIC_APP_ID, DIAMETER_AVP_MANDATORY_FLAG, 0, nullList);
  addAnAvp (vendorSpecificApplicationId); 

  // Optional, can appear anywhere in the message, zero or one may be present
  DiameterUnsigned32Avp* firmwareRevision = new DiameterUnsigned32Avp (FIRMWARE_REVISION, DIAMETER_AVP_MANDATORY_FLAG, 0, 0);
  addAnAvp (firmwareRevision); 
}
