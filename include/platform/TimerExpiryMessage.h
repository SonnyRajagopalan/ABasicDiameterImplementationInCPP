#ifndef TIMER_EXPIRY_MESSAGE_H
#define TIMER_EXPIRY_MESSAGE_H

#include "AsynchronousMessage.h"

class Timer;

class TimerExpiryMessage : public AsynchronousMessage
{
 public:

  TimerExpiryMessage () {}
  TimerExpiryMessage (Timer* _pTimer);
  ~TimerExpiryMessage ();

  Timer* getTimer () { return pTimer; }
  void setTimer (Timer* _pTimer) { pTimer = _pTimer; }
  void process ();

 private:
  Timer* pTimer;
};

#endif
