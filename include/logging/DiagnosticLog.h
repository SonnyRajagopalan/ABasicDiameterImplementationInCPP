#ifndef DIAGNOSTIC_LOG_H
#define DIAGNOSTIC_LOG_H

#include <ostream>
#include <sstream>
#include "logging/Log.h"

using namespace std;

class DiagnosticLog : public ostringstream
{
 public:

  DiagnosticLog (const CHAR* _ident, UINT32 _option, UINT32 _facility, UINT32 _maxLevel)
    {
      log = new Log (_ident, _option, _facility, _maxLevel);
      (static_cast<ostream*>(this))->rdbuf (log);
    };

 protected:

  friend std::ostream& operator << (std::ostream& _os, const LogPriority& _priority)
  {
    static_cast<Log *>(_os.rdbuf())->priority = (int)_priority;
    return _os;
  }
 private:

  Log* log;
};
#endif
