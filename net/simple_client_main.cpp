#include "../include/ClientSocket.h"
#include "../include/SocketException.h"
#include <iostream>
#include <string>
#include <sstream>
#include "../include/SimpleObject.h"

int main ( int argc, char* argv[] )
{
  try
    {

      ClientSocket client_socket ( argv[1], 30000 );
      ostringstream outputstream;
      istringstream inputstream;
      string str("Doofus");
      string os;

      SimpleObject a;
      a.someNumber = 2;
      a.someCharacter = 'a';
      str.copy(a.someString, 100, 0);
      a.someOtherCharacter = 'b';
      a.someShortNumber = 256;
      a.someFloat = 3.14;

      std::string reply;

      try
	{
	  outputstream << a;
	  client_socket << outputstream.str();
	  client_socket >> reply;
	}
      catch ( SocketException& ) {}

      std::cout << "We received this response from the server:\n\"" << reply << "\"\n";;

    }
  catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\n";
    }

  return 0;
}
