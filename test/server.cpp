#include <iostream>
#include "net/DiameterServer.h"

int main()
{
  DiameterServer* dserv = new DiameterServer();
  dserv->initialize();
  dserv->run();
}
