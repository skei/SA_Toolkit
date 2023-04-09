#ifndef sat_win32_timer_included
#define sat_win32_timer_included
//----------------------------------------------------------------------

#include "base/utils/sat_math.h"

//----------------------------------------------------------------------

class SAT_Timer;
class SAT_TimerListener {
  public:
    virtual void do_timerListener_callback(SAT_Timer* ATimer) {}
};

//----------------------------------------------------------------------

//static void sat_timer_callback(sigval val) {
//  if (listener) listener->on_timerCallback(val.sival_int);
//}

//static
//void sat_win32_timerproc(HWND unnamedParam1, UINT unnamedParam2, UINT_PTR unnamedParam3, DWORD unnamedParam4) {
//}


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Timer {

private:

  //friend
  //static void sat_timer_callback(sigval val);

//  static void sat_timer_callback(sigval val) {
//    //SAT_TimerListener* listener = (SAT_TimerListener*)val.sival_ptr;
//    //if (listener) listener->on_timerCallback(val.sival_int);
//    SAT_Timer* timer = (SAT_Timer*)val.sival_ptr;
//    if (timer) {
//      timer->on_timer();
//      //if (listener) listener->on_timerCallback(val.sival_int);
//    }
//  }

  static void sat_win32_timerproc(HWND hwnd, UINT message, UINT_PTR idTimer, DWORD dwTime) {
    //SAT_Print("HWND %p WM_TIMER %i idTimer %i dwTime %i\n",hwnd,message,idTimer,dwTime);
    SAT_Timer* timer = (SAT_Timer*)idTimer;
    if (timer) {
      timer->on_timer();
    }
  }

private:

  bool                MRunning        = false;
  SAT_TimerListener*  MTimerListener  = nullptr;
  HWND                MWinHandle      = NULL;

public:

  SAT_Timer(SAT_TimerListener* AListener) {
    MTimerListener = AListener;
  }

  //----------

  // timer_delete() deletes the timer whose ID is given in timerid.  If
  // the timer was armed at the time of this call, it is disarmed before
  // being deleted.  The treatment of any pending signal generated by the
  // deleted timer is unspecified.

  ~SAT_Timer() {
  }

public:

  bool isRunning() { return MRunning; }

  //----------

  void start(float ms, HWND hwnd, bool oneshot=false) {
    MWinHandle = hwnd;
    if (!MRunning) {
      //SetTimer(MWinHandle,MTimerId,ms,(TIMERPROC)NULL);
      SetTimer(MWinHandle,(UINT_PTR)this,ms,sat_win32_timerproc);
      MRunning = true;
    }
  }

  //----------

  // If new_value->it_value specifies a nonzero value (i.e., either sub‐
  // field is nonzero), then timer_settime() arms (starts) the timer, set‐
  // ting it to initially expire at the given time.  (If the timer was
  // already armed, then the previous settings are overwritten.)  If
  // new_value->it_value specifies a zero value (i.e., both subfields are
  // zero), then the timer is disarmed.

  void stop() {
    if (MRunning) {
      KillTimer(MWinHandle,(UINT_PTR)this);
      MRunning = false;
    }
  }

  //----------

  void on_timer() {
    if (MTimerListener) MTimerListener->do_timerListener_callback(this);
  }

};

//----------------------------------------------------------------------
#endif


