#include "common/Util.h"
#include "base/DiameterPeer.h"
#include "base/DPFSMClosedState.h"
#include "common/Debug.h"

DiameterPeer::DiameterPeer(BasicString& _ipv4Address) : uuid (getNewUUIDString ()),
							ipv4Address (_ipv4Address),
							pCurrentState (NULL)

{
  cout << "UUID is " << uuid << " ipv4Address is " << ipv4Address << endl;
}

// DiameterPeer::DiameterPeer(BasicString& _ipv4Address, const Socket* _newClientSocket) : uuid (getNewUUIDString ()),
// 										  ipv4Address (_ipv4Address),
// 										  socket (*_newClientSocket)

// {
//   cout << "UUID is " << uuid << " ipv4Address is " << ipv4Address << endl;
// }

DiameterPeer::DiameterPeer (BasicString& _ipv4Address, const SINT32 _newClientSocketFD) : uuid (getNewUUIDString ()),
											ipv4Address (_ipv4Address),
											socketFD (_newClientSocketFD)
{
  cout << "UUID is " << uuid << " ipv4Address is " << ipv4Address << " socketFD = " << socketFD << endl;
}

DiameterPeer::~DiameterPeer()
{
  // Do nothing for now, but you should do the following:
  // Close the 
}

DiameterPeer::DiameterPeer (BasicString& _ipv4Address, 
			    BasicString& _description) : uuid (getNewUUIDString ()), 
							 description (_description),
							ipv4Address (_ipv4Address)
{
}

Result DiameterPeer::changeState (DPFSMBaseState* pNextState)
{
  pCurrentState = pNextState;  

  return OK;
}
