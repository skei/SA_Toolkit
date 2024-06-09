#ifndef sat_mac_time_included
#define sat_mac_time_included
//----------------------------------------------------------------------

// link to librt:
// -lrt

#include <time.h>     // timer
#include <sys/time.h>
#include <signal.h>     // sigval
#include <errno.h>      // errno

#include "sat.h"
//#include "base/sat_const.h"
//#include "base/utils/sat_math.h"

//----------------------------------------------------------------------

struct SAT_CurrentTime {
  int32_t year;
  int32_t month;
  int32_t day;
  int32_t hour;
  int32_t minutes;
  int32_t seconds;
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

void SAT_GetLocalTime(SAT_CurrentTime* ATime) {
  time_t t       = time(nullptr);
  struct tm tm   = *localtime(&t); // cppcheck recommends localtime_r
  ATime->year    = tm.tm_year + 1900;
  ATime->month   = tm.tm_mon + 1;
  ATime->day     = tm.tm_mday;
  ATime->hour    = tm.tm_hour;
  ATime->minutes = tm.tm_min;
  ATime->seconds = tm.tm_sec;
  //SAT_Print("now: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
  //return &CODE_CURRENT_TIME;
}

//----------

/*
  The C library function clock_t clock(void) returns the number of clock ticks
  elapsed since the program was launched. To get the number of seconds used by
  the CPU, you will need to divide by CLOCKS_PER_SEC.
  On a 32 bit system where CLOCKS_PER_SEC equals 1000000 this function will
  return the same value approximately every 72 minutes.
*/

//

// http://man7.org/linux/man-pages/man2/timer_create.2.html

double SAT_GetTimeMS(void) {
  //clock_t curtime = clock();
  //return (double)curtime / (double)CLOCKS_PER_SEC * 1000.0;
  struct timeval time;
  gettimeofday(&time,NULL);
  double t = (double)time.tv_sec + (double)time.tv_usec * .000001;
  return t * 1000.0;
}

double SAT_GetTime(void) {
  struct timeval time;
  gettimeofday(&time,NULL);
  double t = (double)time.tv_sec + (double)time.tv_usec * .000001;
  return t;
}

// usleep is deprecated

//void SAT_Sleep(SAT_ui32 ms) {
//  usleep(ms*1000); // ms*1000;
//}

int SAT_Sleep(long ms) {
  struct timespec req, rem;
  if (ms > 999) {
    req.tv_sec = (int)(ms / 1000);                            // Must be Non-Negative
    req.tv_nsec = (ms - ((long)req.tv_sec * 1000)) * 1000000; // Must be in range of 0 to 999999999
  }
  else {
    req.tv_sec = 0;               // Must be Non-Negative
    req.tv_nsec = ms * 1000000;   // Must be in range of 0 to 999999999
  }
  return nanosleep(&req , &rem);
}

//----------

//----------------------------------------------------------------------
//
// timer
//
//----------------------------------------------------------------------

//class SAT_TimerListener {
//  public:
//    virtual void on_timerCallback(void) {}
//};
//
////----------
//
//void sat_timer_callback(sigval val) {
//  SAT_TimerListener* listener = (SAT_TimerListener*)val.sival_ptr;
//  if (listener) listener->on_timerCallback();
//}
//
////----------
//
//class SAT_Timer {
//
//private:
//
//  sigevent    MSigEvent;
//  timer_t     MTimer;
//  itimerspec  MTimerSpec;
//
//public:
//
//  SAT_Timer(SAT_TimerListener* AListener) {
//    MSigEvent.sigev_notify            = SIGEV_THREAD;
//    MSigEvent.sigev_notify_function   = sat_timer_callback;
//    MSigEvent.sigev_notify_attributes = nullptr;
//    MSigEvent.sigev_value.sival_int   = 0; // timer index?
//    MSigEvent.sigev_value.sival_ptr   = AListener;
//    //int res =
//    timer_create(CLOCK_REALTIME, &MSigEvent, &MTimer);
//    //SAT_Trace("MTimer %i\n",MTimer);
//    /*if (res == -1) {
//      switch(errno) {
//        case EAGAIN:
//          SAT_Trace("timer_create error: Temporary error during kernel allocation of timer structures.\n");
//          break;
//        case EINVAL:
//          SAT_Trace("timer_create error: Clock ID, sigev_notify, sigev_signo, or sigev_notify_thread_id is invalid.\n");
//          break;
//        case ENOMEM:
//          SAT_Trace("timer_create error: Could not allocate memory.\n");
//          break;
//      }
//    }*/
//  }
//
//  //----------
//
//  // timer_delete() deletes the timer whose ID is given in timerid.  If
//  // the timer was armed at the time of this call, it is disarmed before
//  // being deleted.  The treatment of any pending signal generated by the
//  // deleted timer is unspecified.
//
//  ~SAT_Timer() {
//    timer_delete(MTimer);
//  }
//
//  //----------
//
//  void start(float ms, bool oneshot=false) {
//    float s = ms * 0.001f;
//    float sec = SAT_Trunc(s);
//    float nsec = (s-sec) * 1000000000; // 1000000.0f * (sec - SAT_Fract(s));
//    time_t isec = sec;
//    long insec = nsec;
//    //SAT_Trace("s %.4f sec %.4f nsec %.4f isec %i insec %i\n",s,sec,nsec,isec,insec);
//    if (oneshot) {
//      MTimerSpec.it_interval.tv_sec   = 0;
//      MTimerSpec.it_interval.tv_nsec  = 0;
//    }
//    else {
//      MTimerSpec.it_interval.tv_sec   = isec;
//      MTimerSpec.it_interval.tv_nsec  = insec;
//    }
//    MTimerSpec.it_value.tv_sec      = isec;
//    MTimerSpec.it_value.tv_nsec     = insec;
//    //int res =
//    timer_settime(MTimer, 0, &MTimerSpec, 0);
//    /*if (res == -1) {
//      switch(errno) {
//        case EFAULT:
//          SAT_Trace("timer_settime error: new_value, old_value, or curr_value is not a valid pointer.\n");
//          break;
//        //case EINVAL:
//        //  SAT_Trace("timer_settime error: timerid is invalid.\n");
//        //  break;
//        case EINVAL:
//          SAT_Trace("timer_settime: new_value.it_value is negative; or new_value.it_value.tv_nsec is negative or greater than 999,999,999.\n");
//          break;
//      }
//    }*/
//  }
//
//  //----------
//
//  // If new_value->it_value specifies a nonzero value (i.e., either sub‐
//  // field is nonzero), then timer_settime() arms (starts) the timer, set‐
//  // ting it to initially expire at the given time.  (If the timer was
//  // already armed, then the previous settings are overwritten.)  If
//  // new_value->it_value specifies a zero value (i.e., both subfields are
//  // zero), then the timer is disarmed.
//
//  void stop(void) {
//    MTimerSpec.it_interval.tv_sec   = 0;
//    MTimerSpec.it_interval.tv_nsec  = 0;
//    MTimerSpec.it_value.tv_sec      = 0;
//    MTimerSpec.it_value.tv_nsec     = 0;
//    timer_settime(&MTimer, 0, &MTimerSpec, 0);
//  }
//
//};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//class SAT_Time {
//
//  public:
//
//    SAT_Time(bool AStart=true) {
//      if (AStart) start();
//    }
//
//    ~SAT_Time() {}
//
//  public:
//
//    double start(void) {
//      MStartTime = clock();
//      return MStartTime;
//    }
//
//    /*
//      wraparound? negative?
//      if clock has wrapped around since start() was called, we would
//      clock_t -> long int
//    */
//
//    double sec(void) {
//      clock_t currenttime = clock();
//      double elapsed = currenttime - MStartTime;
//      return elapsed / CLOCKS_PER_SEC; // 1000000;
//    }
//
//    double ms(void) {
//      return sec() * 1000.0;
//    }
//
//    double us(void) {
//      return sec() * 1000000.0;
//    }
//
//  private:
//      clock_t MStartTime = 0; // cppcheck
//};

//----------------------------------------------------------------------
#endif
