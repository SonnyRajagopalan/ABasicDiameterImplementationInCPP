#include <string>
#include <sstream>
#include <fstream>
#include <cstring>
#include "net/ServerSocket.h"
#include "net/SocketException.h"
#include "common/Types.h"
#include "base/DiameterPeer.h"
#include "base/DiameterPeerDb.h"
#include "net/DiameterServer.h"
#include "avps/DiameterAvpList.h"
#include "messages/DiameterHeader.h"
#include "common/Debug.h"

DiameterServer::DiameterServer () : ServerSocket (DIAMETER_PORT)
{
}

DiameterServer::~DiameterServer ()
{
}

DiameterServer::DiameterServer (BasicString& _description) : 
  ServerSocket (DIAMETER_PORT),
  description (_description)
{
}

Result DiameterServer::initialize ()
{
  pDiameterPeerDb = DiameterPeerDb::getInstance ();
  return OK;
}

Result DiameterServer::run ()
{
  /**
     Will never really execute this bit of code. Exists for testing only.
  */
  try
    {
      while (true)
	{	  
	  Socket newSocket;
	  DiameterPeer* pPeer;
	  BasicString ipv4Addr;
	  vector<char> message;
	  ServerSocket::accept (&newSocket);

	  ipv4Addr = newSocket.getPeerIPv4Address ();
	  pPeer = pDiameterPeerDb->getPeerFromIPv4 (ipv4Addr);

	  cout << "received message from IP = " << newSocket.getPeerIPv4Address();
	  cout << ", DB size = " << pDiameterPeerDb->size () << endl;
	  if (pPeer == NULL)
	    {
	      cout << "pPeer = NULL, creating a new peer entry" << endl;
	      pPeer = new DiameterPeer (ipv4Addr);
	      pDiameterPeerDb->insertDiameterPeerIntoDb (pPeer);
	    }
	  else
	    {
	      // The peer exists. Receive a message from the peer and
	      // HandleDiameterMessage (buffer); which passes it on to the correct app
	      // after a whole load of validations. E.g. is the peer registered for 
	      // the app it is trying to access?
	    }
	  newSocket >> message; // Extract from the newSocket into (vector) message.
	  stringstream in;
	  cout << "Received message from peer of size " << message.size() << endl;
	  ofstream outfile ("/tmp/server.bin", ios::binary);
	  string rcvdMessage (message.begin(), message.end());

	  outfile << rcvdMessage;
	  in << rcvdMessage; // Place the received message in the input stream

	  outfile.close();
	  deserialize (in); // std::stringstream inherits from both istream and ostream
	}
    }
  catch (SocketException& e)
    {
      cout << "Exception was caught: " << e.description() << "\nExiting.\n";
      return ERROR;
    }

  return OK;
}

void DiameterServer::serialize (OutputStream& os)
{
}

void DiameterServer::deserialize (InputStream& in)
{
  DiameterHeader hdr;
  DiameterAvpList avpList;

  cout << "Calling DiameterHeader's deserialize" << endl;
  hdr.deserialize (in);
  cout << "Calling DiameterAvpList's deserialize..." << endl;
  avpList.deserialize (in);
}
