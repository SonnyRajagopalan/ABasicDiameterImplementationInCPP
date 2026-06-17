#ifndef DIAMETER_BASE_TASK_DIAGNOSTIC_LOG_H
#define DIAMETER_BASE_TASK_DIAGNOSTIC_LOG_H

#include "logging/DiagnosticLog.h"

class DiameterBaseTaskDiagnosticLog : public DiagnosticLog
{
 public:
  static DiameterBaseTaskDiagnosticLog& getInstance ();
  
 private:
  DiameterBaseTaskDiagnosticLog (); // Prevent clients from creating a new singleton
  DiameterBaseTaskDiagnosticLog (const DiameterBaseTaskDiagnosticLog& _dummy); // Prevent clients from creating a copy of the singleton
  static DiameterBaseTaskDiagnosticLog* pInstance;
};
#endif
