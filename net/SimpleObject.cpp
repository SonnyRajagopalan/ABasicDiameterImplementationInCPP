#include "../include/SimpleObject.h"
#include <string.h>
#include <iostream>

using namespace std;
int main()
{
  SimpleObject a;
  a.someNumber = 2;
  a.someCharacter = 'a';
  strcpy (a.someString, "Sonny Rajagopalan");
  a.someOtherCharacter = 'b';
  a.someShortNumber = 256;
  a.someFloat = 3.14;

  cout << a << endl;
}
