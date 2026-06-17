#include <iostream>
#include <sstream>
#include "net/ClientSocket.h"
#include "net/SocketException.h"
#include "messages/DiameterCER.h"
#include "messages/DiameterHeader.h"
#include "avps/DiameterAvpList.h"
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
      cout << "[" << getpid() << "] Connecting to " << serverIp
	   << ":" << port << endl;

      ClientSocket client (serverIp, port);

      ostringstream os (ios::binary);
      DiameterCER cer;
      cer.serialize (os);

      cout << "[" << getpid() << "] Sending CER ("
	   << cer.getHeader ().getMessageLength () << " bytes)..." << endl;

      client << os;

      vector<char> response;
      SINT32 received = client.recv (response);

      if (received <= 0)
	{
	  cerr << "[" << getpid() << "] FAIL: No CEA received" << endl;
	  client.close ();
	  return 1;
	}

      cout << "[" << getpid() << "] Received " << received << " bytes" << endl;

      stringstream in (string (response.begin (), response.end ()));

      DiameterHeader ceaHdr;
      ceaHdr.deserialize (in);

      UINT32 cc = ceaHdr.getCommandCode ();
      UINT8 flags = ceaHdr.getFlags ();

      cout << "[" << getpid() << "] CEA: CC=" << cc
	   << " Flags=0x" << hex << (int)flags << dec << endl;

      if (cc != 257)
	{
	  cerr << "[" << getpid() << "] FAIL: Expected CC=257, got " << cc << endl;
	  client.close ();
	  return 1;
	}

      if (flags & DIAMETER_REQUEST_FLAG)
	{
	  cerr << "[" << getpid() << "] FAIL: CEA has REQ flag set" << endl;
	  client.close ();
	  return 1;
	}

      DiameterAvpList ceaAvps;
      ceaAvps.deserialize (in);

      cout << "[" << getpid() << "] CEA has " << ceaAvps.getNumberOfAvps ()
	   << " AVPs" << endl;
      cout << "[" << getpid() << "] PASS: CER/CEA handshake successful" << endl;

      client.close ();
      return 0;
    }
  catch (SocketException& e)
    {
      cerr << "[" << getpid() << "] FAIL: " << e.description () << endl;
      return 1;
    }
}
