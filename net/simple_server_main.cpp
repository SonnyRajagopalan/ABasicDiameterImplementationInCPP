#include "../include/ServerSocket.h"
#include "../include/SocketException.h"
#include <string>
#include <iostream>
#include <sstream>
#include "../include/SimpleObject.h"
#include "../include/Types.h"

int main ( int argc, char* argv[] )
{
  std::cout << "running....\n";
  istringstream input;
  SimpleObject a;

  try
    {
      // Create the socket
      ServerSocket server ( 30000 );

      while ( true )
	{

	  ServerSocket new_sock;
	  server.accept ( new_sock );

	  try
	    {
	      while ( true )
		{
		  string data;
		  new_sock >> data; // 1
		  input.str(data);  // 2
		  input >> a;
		  cout << "Received from the client (SO): [" << a << "]" << endl;
		  std::cout << "received message from IP = " << new_sock.getPeerIPv4Address() << std::endl;
		  new_sock << data; // Reply back to client (echo service)		  
		}
	    }
	  catch ( SocketException& ) {}

	}
    }
  catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
    }

  return 0;
}
