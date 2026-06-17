#ifndef LOG_H
#define LOG_H
#include <iostream>
#include <syslog.h>
#include "common/Types.h"
/**
   Design notes, 04/07/2012:
   
   Design adapted for use (modified lightly) from 
   http://stackoverflow.com/questions/2638654/redirect-c-stdclog-to-syslog-on-unix

   Some elements of this design were changed. the design specified above called
   for the use of std::clog, we will use the Log object as a private object to a
   ostringstream object, invalidating the need to associate the buffer (rdbuf(), 
   see http://www.cplusplus.com/reference/iostream/ostringstream/rdbuf/) with
   the ostringstream object being used, which was quite frankly, very clunky
   semantics.

   Future plans:

   The plan is to extend this class to do binary logs and 
   binary streamling logs as well. For both of the above, the idea is 
   to have specific call record structures that will be populated and written
   to disk in binary format, and/or transmitted to a remote North-bound server.

   For syslog details, syslog methods, please see 
   http://www.linuxselfhelp.com/gnu/glibc/html_chapter/libc_18.html
*/
enum LogPriority
  {
    LOGPRIORITY_EMERGENCY  = LOG_EMERG,   // System is unusable
    LOGPRIORITY_ALERT      = LOG_ALERT,   // Action must be taken immediately
    LOGPRIORITY_CRITICAL   = LOG_CRIT,    // Critical conditions
    LOGPRIORITY_ERROR      = LOG_ERR,     // Error conditions
    LOGPRIORITY_WARNING    = LOG_WARNING, // Warning conditions
    LOGPRIORITY_NOTICE     = LOG_NOTICE,  // Normal, but significant, condition
    LOGPRIORITY_INFO       = LOG_INFO,    // Informational message
    LOGPRIORITY_DEBUG      = LOG_DEBUG    // Debug-level message
  };

std::ostream& operator << (std::ostream& _os, const LogPriority& _logPriority);

class Log : public std::streambuf
//ass Log : public std::basic_streambuf<char, std::char_traits<char> >
{
 public:
  //Log (BasicString& _ident, UINT32 _option, UINT32 _facility, UINT32 _maxLevel = LOG_DEBUG);
  Log (const CHAR* _ident, UINT32 _option, UINT32 _facility, UINT32 _maxLevel = LOG_DEBUG);
  ~Log ();
  
  // Override some streambuf methods
  SINT32 sputc (CHAR c);
  std::streamsize sputn (const CHAR* s, std::streamsize n);

 protected:

  SINT32 sync ();
  SINT32 overflow (SINT32 c);
 private:
  friend std::ostream& operator << (std::ostream& _os, const LogPriority& _priority);
  void sendToSyslog (BasicString& _log);
  void setLogUpto (UINT32 _maxLevel);

  CHAR ident[50];
  UINT32 option;
  UINT32 facility;
  UINT32 priority;
  UINT32 maxLevel;
  BasicString buffer;
};

#endif
