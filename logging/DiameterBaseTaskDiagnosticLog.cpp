#include "logging/DiameterBaseTaskDiagnosticLog.h"

DiameterBaseTaskDiagnosticLog* DiameterBaseTaskDiagnosticLog::pInstance = 0;

DiameterBaseTaskDiagnosticLog& DiameterBaseTaskDiagnosticLog::getInstance ()
{
  if (pInstance == NULL)
    {
      pInstance = new DiameterBaseTaskDiagnosticLog ();
    }
  return *pInstance;
}

DiameterBaseTaskDiagnosticLog::DiameterBaseTaskDiagnosticLog () :
  DiagnosticLog ("DiameterBaseTask", 0, LOG_LOCAL1, LOG_DEBUG)
{
  
}
