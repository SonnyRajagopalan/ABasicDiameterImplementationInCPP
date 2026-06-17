#include <iostream>
#include <sstream>
#include <list>
#include <csignal>
#include "net/ServerSocket.h"
#include "net/SocketException.h"
#include "messages/DiameterHeader.h"
#include "avps/DiameterAvpList.h"
#include "avps/DiameterUnsigned32Avp.h"
#include "avps/DiameterDiamIdentAvp.h"
#include "avps/DiameterAddressAvp.h"
#include "avps/DiameterUTF8StringAvp.h"
#include "common/DiameterConstants.h"
#include "common/Util.h"
#include "common/Debug.h"

using namespace std;

static volatile bool keepRunning = true;
static void handleSignal (int) { keepRunning = false; }

int main (int argc, char* argv[])
{
  SINT32 port = DIAMETER_PORT;
  if (argc > 1)
    port = atoi (argv[1]);

  signal (SIGINT, handleSignal);
  signal (SIGTERM, handleSignal);

  try
    {
      ServerSocket server (port);
      cout << "[" << getpid() << "] testServer listening on port "
	   << port << " (Ctrl-C to stop)" << endl;

      while (keepRunning)
	{
	  Socket peer;
	  if (server.accept (&peer) != OK)
	    {
	      cout << "[" << getpid() << "] accept failed" << endl;
	      continue;
	    }

	  string clientIp = peer.getPeerIPv4Address ();
	  cout << "[" << getpid() << "] Connection from " << clientIp << endl;

	  vector<char> raw;
	  SINT32 n = peer.recv (raw);
	  if (n <= 0)
	    {
	      cout << "[" << getpid() << "] recv failed or closed" << endl;
	      peer.close ();
	      continue;
	    }

	  cout << "[" << getpid() << "] Received " << n << " bytes" << endl;

	  stringstream in (string (raw.begin (), raw.end ()));

	  DiameterHeader hdr;
	  hdr.deserialize (in);

	  UINT32 cc = hdr.getCommandCode ();
	  UINT8 flags = hdr.getFlags ();
	  UINT32 hopByHop = hdr.getHopByHopId ();
	  UINT32 endToEnd = hdr.getEndToEndId ();

	  cout << "[" << getpid() << "] Header: CC=" << cc
	       << " Flags=0x" << hex << (int)flags << dec
	       << " H2H=" << hopByHop
	       << " E2E=" << endToEnd << endl;

	  DiameterAvpList avps;
	  avps.deserialize (in);

	  cout << "[" << getpid() << "] Received " << avps.getNumberOfAvps ()
	       << " AVPs" << endl;

	  if (cc == 257)
	    {
	      cout << "[" << getpid() << "] CER detected, sending CEA..." << endl;

	      DiameterHeader ceaHdr;
	      ceaHdr.setVersion (DIAMETER_VERSION);
	      ceaHdr.setMessageLength (DIAMETER_HEADER_SIZE);
	      ceaHdr.setFlags (DIAMETER_AVP_MANDATORY_FLAG);
	      ceaHdr.setCommandCode (257);
	      ceaHdr.setApplicationId (0);
	      ceaHdr.setHopByHopId (hopByHop);
	      ceaHdr.setEndToEndId (endToEnd);

	      DiameterAvpList ceaAvps;
	      ceaAvps.addAvpToAvpList (new DiameterUnsigned32Avp (
		  RESULT_CODE, DIAMETER_AVP_MANDATORY_FLAG, 0, 2001));
	      ceaAvps.addAvpToAvpList (new DiameterDiamIdentAvp (
		  ORIGIN_HOST, DIAMETER_AVP_MANDATORY_FLAG, 0, getMyHostname ()));
	      ceaAvps.addAvpToAvpList (new DiameterDiamIdentAvp (
		  ORIGIN_REALM, DIAMETER_AVP_MANDATORY_FLAG, 0, "test.realm"));
	      ceaAvps.addAvpToAvpList (new DiameterAddressAvp (
		  HOST_IP_ADDRESS, DIAMETER_AVP_MANDATORY_FLAG, 0, getMyIPAddress ()));
	      ceaAvps.addAvpToAvpList (new DiameterUnsigned32Avp (
		  VENDOR_ID, DIAMETER_AVP_MANDATORY_FLAG, 0, 42));
	      ceaAvps.addAvpToAvpList (new DiameterUTF8StringAvp (
		  PRODUCT_NAME, DIAMETER_AVP_MANDATORY_FLAG, 0, "testServer"));

	      ceaHdr.setMessageLength (DIAMETER_HEADER_SIZE + ceaAvps.getAvpListLength ());

	      ostringstream response (ios::binary);
	      ceaHdr.serialize (response);
	      ceaAvps.serialize (response);

	      string respStr = response.str ();
	      vector<char> respData (respStr.begin (), respStr.end ());

	      peer.send (respData);
	      cout << "[" << getpid() << "] Sent CEA (" << respData.size ()
		   << " bytes, " << ceaAvps.getNumberOfAvps () << " AVPs)" << endl;
	    }

	  peer.close ();
	  cout << "[" << getpid() << "] Connection closed" << endl;
	}

      cout << "[" << getpid() << "] Shutting down" << endl;
      return 0;
    }
  catch (SocketException& e)
    {
      cerr << "[" << getpid() << "] Server exception: " << e.description () << endl;
      return 1;
    }
}
