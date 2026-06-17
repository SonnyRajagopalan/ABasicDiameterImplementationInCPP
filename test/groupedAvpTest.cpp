#include <iostream>
#include <sstream>
#include <list>
#include "net/ClientSocket.h"
#include "net/SocketException.h"
#include "messages/DiameterHeader.h"
#include "avps/DiameterAvpList.h"
#include "avps/DiameterUnsigned32Avp.h"
#include "avps/DiameterOctetStringAvp.h"
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
      cout << "[" << getpid() << "] Grouped AVP Test connecting to "
	   << serverIp << ":" << port << endl;

      ClientSocket client (serverIp, port);

      DiameterHeader hdr;
      hdr.setVersion (DIAMETER_VERSION);
      hdr.setMessageLength (DIAMETER_HEADER_SIZE);
      hdr.setFlags (DIAMETER_REQUEST_FLAG | DIAMETER_AVP_MANDATORY_FLAG);
      hdr.setCommandCode (301);
      hdr.setApplicationId (0);
      hdr.setHopByHopId (1);
      hdr.setEndToEndId (1);

      DiameterAvpList avps;

      {
	list<DiameterAvp*> emptyList;
	DiameterGroupedAvp* level3 = new DiameterGroupedAvp (
	    EXPERIMENTAL_RESULT, DIAMETER_AVP_MANDATORY_FLAG, 0, emptyList);
	level3->addAnAvp (new DiameterUnsigned32Avp (
	    RESULT_CODE, DIAMETER_AVP_MANDATORY_FLAG, 0, 2001));
	level3->addAnAvp (new DiameterOctetStringAvp (
	    PROXY_STATE, DIAMETER_AVP_MANDATORY_FLAG, 0, "level3"));

	DiameterGroupedAvp* level2 = new DiameterGroupedAvp (
	    VENDOR_SPECIFIC_APP_ID, DIAMETER_AVP_MANDATORY_FLAG, 0, emptyList);
	level2->addAnAvp (level3);
	level2->addAnAvp (new DiameterUnsigned32Avp (
	    VENDOR_ID, DIAMETER_AVP_MANDATORY_FLAG, 0, 10415));

	DiameterGroupedAvp* level1 = new DiameterGroupedAvp (
	    PROXY_INFO, DIAMETER_AVP_MANDATORY_FLAG, 0, emptyList);
	level1->addAnAvp (level2);
	level1->addAnAvp (new DiameterOctetStringAvp (
	    PROXY_STATE, DIAMETER_AVP_MANDATORY_FLAG, 0, "nested-grouped-test"));

	avps.addAvpToAvpList (level1);
      }

      avps.addAvpToAvpList (new DiameterUnsigned32Avp (
	  ORIGIN_STATE_ID, DIAMETER_AVP_MANDATORY_FLAG, 0, 42));

      hdr.setMessageLength (DIAMETER_HEADER_SIZE + avps.getAvpListLength ());

      ostringstream os (ios::binary);
      hdr.serialize (os);
      avps.serialize (os);

      cout << "[" << getpid() << "] Sending " << hdr.getMessageLength ()
	   << " bytes with " << avps.getNumberOfAvps ()
	   << " AVPs (3-level nested Grouped)" << endl;

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
