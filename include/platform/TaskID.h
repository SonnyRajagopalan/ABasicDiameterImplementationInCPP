#ifndef TASK_ID_H
#define TASK_ID_H

#include "common/Types.h"

class TaskID
{
 public:

  enum ID
    {
      DIAMETER_BASE_TASK   = 1,
      TIMER_TASK           = 2,
      // Register new applications below
      // E.g.:
      // GX_APPLICATION    = 3,
    };
  TaskID () {};
  TaskID (ID _id) : id (_id) {}
  inline const ID getID () const { return id; }
  inline void setID (const ID _id) { id = _id; }
  inline const CHAR* getPathName () const; // Return the POSIX message queue pathname

 private:
  ID id;
};

const CHAR* TaskID::getPathName() const
{
  switch (id)
    {
    case DIAMETER_BASE_TASK:
      return "/DiameterBaseTask";
    case TIMER_TASK:
      return "/TimerTask";
      // Add POSIX message queue path name for new applications below
      // E.g.:
      // case GX_APPLICATION:
      //   return "/GxApplication"
    default:
      return "Invalid";
    }
}
#endif
