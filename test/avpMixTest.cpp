#include <iostream>
#include <sstream>
#include <list>
#include "net/ClientSocket.h"
#include "net/SocketException.h"
#include "messages/DiameterHeader.h"
#include "avps/DiameterAvpList.h"
#include "avps/DiameterUnsigned32Avp.h"
#include "avps/DiameterInteger32Avp.h"
#include "avps/DiameterInteger64Avp.h"
#include "avps/DiameterUnsigned64Avp.h"
#include "avps/DiameterFloat32Avp.h"
#include "avps/DiameterFloat64Avp.h"
#include "avps/DiameterOctetStringAvp.h"
#include "avps/DiameterAddressAvp.h"
#include "avps/DiameterTimeAvp.h"
#include "avps/DiameterUTF8StringAvp.h"
#include "avps/DiameterDiamIdentAvp.h"
#include "avps/DiameterDiamURIAvp.h"
#include "avps/DiameterEnumeratedAvp.h"
#include "avps/DiameterGroupedAvp.h"
#include "common/DiameterConstants.h"
#include "common/Debug.h"

using namespace std;

int main (int argc, char* argv[])
{
  BasicString serverIp = "127.0.0.1";
  SINT32 port = DIAMETER_PORT;

  if (argc > 1)
    serverIp = argv[1];
  if (argc > 2)
    port = atoi (argv[2]);

  try
    {
      cout << "[" << getpid() << "] AVP Mix Test connecting to "
	   << serverIp << ":" << port << endl;

      ClientSocket client (serverIp, port);

      DiameterHeader hdr;
      hdr.setVersion (DIAMETER_VERSION);
      hdr.setMessageLength (DIAMETER_HEADER_SIZE);
      hdr.setFlags (DIAMETER_REQUEST_FLAG | DIAMETER_AVP_MANDATORY_FLAG);
      hdr.setCommandCode (300);
      hdr.setApplicationId (0);
      hdr.setHopByHopId (100);
      hdr.setEndToEndId (200);

      DiameterAvpList avps;

      avps.addAvpToAvpList (new DiameterUnsigned32Avp (
	  VENDOR_ID, DIAMETER_AVP_MANDATORY_FLAG, 0, 42));

      avps.addAvpToAvpList (new DiameterInteger32Avp (
	  429, DIAMETER_AVP_MANDATORY_FLAG, 0, -1));

      {
	SINT64 s64;
	s64.int64 = (int64_t) 0x12345678LL;
	avps.addAvpToAvpList (new DiameterInteger64Avp (
	    447, DIAMETER_AVP_MANDATORY_FLAG, 0, s64));
      }

      {
	UINT64 u64;
	u64.uint64 = (u_int64_t) 0x87654321ULL;
	avps.addAvpToAvpList (new DiameterUnsigned64Avp (
	    ACCT_SUB_SESSION_ID, DIAMETER_AVP_MANDATORY_FLAG, 0, u64));
      }

      avps.addAvpToAvpList (new DiameterFloat32Avp (
	  9999, DIAMETER_AVP_MANDATORY_FLAG, 0, 3.14f));

      {
	FLT64 f64;
	f64.flt64 = (long double) 2.71828L;
	avps.addAvpToAvpList (new DiameterFloat64Avp (
	    9998, DIAMETER_AVP_MANDATORY_FLAG, 0, f64));
      }

      avps.addAvpToAvpList (new DiameterOctetStringAvp (
	  PROXY_STATE, DIAMETER_AVP_MANDATORY_FLAG, 0, "test-user"));

      avps.addAvpToAvpList (new DiameterAddressAvp (
	  HOST_IP_ADDRESS, DIAMETER_AVP_MANDATORY_FLAG, 0, "127.0.0.1"));

      avps.addAvpToAvpList (new DiameterTimeAvp (
	  EVENT_TIMESTAMP, DIAMETER_AVP_MANDATORY_FLAG, 0, "20260101T000000Z"));

      avps.addAvpToAvpList (new DiameterUTF8StringAvp (
	  PRODUCT_NAME, DIAMETER_AVP_MANDATORY_FLAG, 0, "AVP Mix Test"));

      avps.addAvpToAvpList (new DiameterDiamIdentAvp (
	  ORIGIN_HOST, DIAMETER_AVP_MANDATORY_FLAG, 0, "test-client.example.com"));

      avps.addAvpToAvpList (new DiameterDiamURIAvp (
	  DESTINATION_HOST, DIAMETER_AVP_MANDATORY_FLAG, 0,
	  "aaa://server.example.com"));

      avps.addAvpToAvpList (new DiameterEnumeratedAvp (
	  DISCONNECT_CAUSE, DIAMETER_AVP_MANDATORY_FLAG, 0, 0));

      {
	list<DiameterAvp*> subList;
	subList.push_back (new DiameterUnsigned32Avp (
	    RESULT_CODE, DIAMETER_AVP_MANDATORY_FLAG, 0, 2001));
	avps.addAvpToAvpList (new DiameterGroupedAvp (
	    EXPERIMENTAL_RESULT, DIAMETER_AVP_MANDATORY_FLAG, 0, subList));
      }

      hdr.setMessageLength (DIAMETER_HEADER_SIZE + avps.getAvpListLength ());

      ostringstream os (ios::binary);
      hdr.serialize (os);
      avps.serialize (os);

      cout << "[" << getpid() << "] Sending " << hdr.getMessageLength ()
	   << " bytes with " << avps.getNumberOfAvps ()
	   << " AVPs (all types)" << endl;

      client << os;
      cout << "[" << getpid() << "] Sent successfully" << endl;

      vector<char> response;
      SINT32 received = client.recv (response);
      if (received > 0)
	cout << "[" << getpid() << "] Received " << received
	     << " bytes response" << endl;

      client.close ();
      cout << "[" << getpid() << "] PASS" << endl;
      return 0;
    }
  catch (SocketException& e)
    {
      cerr << "[" << getpid() << "] FAIL: " << e.description () << endl;
      return 1;
    }
}
