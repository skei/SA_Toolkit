#ifndef sat_linux_timer_included
#define sat_linux_timer_included
//----------------------------------------------------------------------

// -lrt

#include <time.h>   // timer
#include <signal.h> // sigval

//#include <sys/time.h>
//#include <errno.h>      // errno


#include "base/utils/sat_math.h"

//----------------------------------------------------------------------

class SAT_Timer;
class SAT_TimerListener {
  public:
    virtual void do_timerListener_callback(SAT_Timer* ATimer) {}
};

//----------------------------------------------------------------------

//static void sat_timer_callback(sigval val) {
//  //SAT_TimerListener* listener = (SAT_TimerListener*)val.sival_ptr;
//  //if (listener) listener->on_timerCallback(val.sival_int);
//  SAT_Timer* timer = (SAT_Timer*)val.sival_ptr;
//  if (timer) {
//    timer->on_timer();
//    //if (listener) listener->on_timerCallback(val.sival_int);
//  }
//}

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Timer {

//------------------------------
private:
//------------------------------

  //friend
  //static void sat_timer_callback(sigval val);

  static void sat_timer_callback(sigval val) {
    //SAT_TimerListener* listener = (SAT_TimerListener*)val.sival_ptr;
    //if (listener) listener->on_timerCallback(val.sival_int);
    SAT_Timer* timer = (SAT_Timer*)val.sival_ptr;
    if (timer) {
      timer->on_timer();
      //if (listener) listener->on_timerCallback(val.sival_int);
    }
  }

//------------------------------
private:
//------------------------------

  sigevent            MSigEvent       = {};
  timer_t             MTimer          = nullptr;
  itimerspec          MTimerSpec      = {};
  std::atomic<bool>   MIsRunning      {false};
  SAT_TimerListener*  MTimerListener  = nullptr;

//------------------------------
public:
//------------------------------

  SAT_Timer(SAT_TimerListener* AListener/*, void* AUserPtr*/) {

    MTimerListener = AListener;
    MIsRunning = false;

    MSigEvent.sigev_notify            = SIGEV_THREAD;
    MSigEvent.sigev_notify_function   = sat_timer_callback;
    MSigEvent.sigev_notify_attributes = nullptr;
    MSigEvent.sigev_value.sival_ptr   = this;//AListener;
    //int res =
    timer_create(CLOCK_REALTIME, &MSigEvent, &MTimer);
    //SAT_Print("MTimer %i\n",MTimer);
    /*if (res == -1) {
      switch(errno) {
        case EAGAIN:
          SAT_Print("timer_create error: Temporary error during kernel allocation of timer structures.\n");
          break;
        case EINVAL:
          SAT_Print("timer_create error: Clock ID, sigev_notify, sigev_signo, or sigev_notify_thread_id is invalid.\n");
          break;
        case ENOMEM:
          SAT_Print("timer_create error: Could not allocate memory.\n");
          break;
      }
    }*/
  }

  //----------

  // timer_delete() deletes the timer whose ID is given in timerid.  If
  // the timer was armed at the time of this call, it is disarmed before
  // being deleted.  The treatment of any pending signal generated by the
  // deleted timer is unspecified.

  ~SAT_Timer() {
    timer_delete(MTimer);
  }

//------------------------------
public:
//------------------------------

  bool isRunning() {
    return MIsRunning;
  }

  //----------

  void start(float ms, bool oneshot=false) {
    if (!MIsRunning) {
      float s = ms * 0.001f;
      float sec = SAT_Trunc(s);
      float nsec = (s-sec) * 1000000000; // 1000000.0f * (sec - SAT_Fract(s));
      time_t isec = sec;
      long insec = nsec;
      //SAT_Print("s %.4f sec %.4f nsec %.4f isec %i insec %i\n",s,sec,nsec,isec,insec);
      if (oneshot) {
        MTimerSpec.it_interval.tv_sec   = 0;
        MTimerSpec.it_interval.tv_nsec  = 0;
      }
      else {
        MTimerSpec.it_interval.tv_sec   = isec;
        MTimerSpec.it_interval.tv_nsec  = insec;
      }
      MTimerSpec.it_value.tv_sec      = isec;
      MTimerSpec.it_value.tv_nsec     = insec;
      //int res =
      timer_settime(MTimer, 0, &MTimerSpec, 0);
      /*if (res == -1) {
        switch(errno) {
          case EFAULT:
            SAT_Print("timer_settime error: new_value, old_value, or curr_value is not a valid pointer.\n");
            break;
          //case EINVAL:
          //  SAT_Print("timer_settime error: timerid is invalid.\n");
          //  break;
          case EINVAL:
            SAT_Print("timer_settime: new_value.it_value is negative; or new_value.it_value.tv_nsec is negative or greater than 999,999,999.\n");
            break;
        }
      }*/
      MIsRunning = true;
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
    if (MIsRunning) {
      MIsRunning = false;
      MTimerSpec.it_interval.tv_sec   = 0;
      MTimerSpec.it_interval.tv_nsec  = 0;
      MTimerSpec.it_value.tv_sec      = 0;
      MTimerSpec.it_value.tv_nsec     = 0;
      /*int res =*/ timer_settime(MTimer, 0, &MTimerSpec, 0);
      //if (res != 0) { SAT_Print("error stopping timer\n"); }
    //  MIsRunning = false;
    }
  }

  //----------

  void on_timer() {
    if (MTimerListener && MIsRunning) MTimerListener->do_timerListener_callback(this);
  }

};

//----------------------------------------------------------------------
#endif

