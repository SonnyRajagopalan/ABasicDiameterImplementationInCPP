#include "logging/Log.h"
#include <string.h>
#include <iostream>
#include <fstream> // For EOF

//using namespace std;

//Log::Log (BasicString& _ident, UINT32 _option, UINT32 _facility, UINT32 _maxLevel) :
Log::Log (const CHAR* _ident, UINT32 _option, UINT32 _facility, UINT32 _maxLevel) :
  option (_option),
  facility (_facility),
  maxLevel (_maxLevel)
{
  priority = LOG_DEBUG;
  //strncpy (ident, _ident.c_str (), _ident.size());
  strcpy (ident, _ident);
  //ident[_ident.size()] = '\0';

  setlogmask (LOG_UPTO (maxLevel));
  openlog (ident, LOG_CONS | LOG_PID | LOG_NDELAY, facility);
}

Log::~Log ()
{
}

std::ostream& operator<< (std::ostream& _os, const LogPriority& _priority) 
{
  static_cast<Log *>(_os.rdbuf())->priority = (int)_priority;
  return _os;
}

void Log::sendToSyslog (BasicString& _log)
{
  char* log = new char [_log.size () + 1];
  strcpy (log, _log.c_str());
  syslog (priority, "%s", log);
}

void Log::setLogUpto (UINT32 _maxLevel)
{ 
  // LOG_MASK has the potential for funkily set 
  // syslog parameters. We will avoid it.
  maxLevel = _maxLevel; 
  setlogmask (LOG_UPTO (maxLevel)); 
}

SINT32 Log::sputc (CHAR c)
{
  char* log = new char [2];
  log[0] = c;
  log[1] = '\0';
  syslog (priority, "%s", log);

  return 0;
}

std::streamsize Log::sputn (const CHAR* s, std::streamsize n)
{
  syslog (priority, "%s", s);

  return n;
}

SINT32 Log::sync ()
{
  if (buffer.length ())
    {
      syslog (priority, "%s", buffer.c_str ());
      buffer.erase ();
      priority = LOG_DEBUG;
    }
  return 0;
}

SINT32 Log::overflow (SINT32 c)
{
  if (c != EOF)    
    {
      buffer += static_cast<char> (c);
    }
  else
    sync ();

  return c;
}
