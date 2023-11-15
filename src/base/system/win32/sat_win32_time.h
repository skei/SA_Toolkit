#ifndef sat_win32_time_included
#define sat_win32_time_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "base/sat_const.h"
#include "base/utils/sat_math.h"

  //SYSTEMTIME st;
  //GetSystemTime(&st);
  //printf("The system time is: %02d:%02d\n", st.wHour, st.wMinute);
    
  //SYSTEMTIME lt;
  //GetLocalTime(&lt);
  //printf(" The local time is: %02d:%02d\n", lt.wHour, lt.wMinute);

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

double SAT_GetTimeMS(void) {
  return 0.0;
}

//----------

double SAT_GetTime(void) {
  SYSTEMTIME st;
  GetSystemTime(&st);
  double time = ((double)st.wHour         * 60 * 60)
              + ((double)st.wMinute       * 60)
              +  (double)st.wSecond
              + ((double)st.wMilliseconds * 0.001);
  //printf("time %f\n",time);
  return time;
}

//----------

struct SAT_CurrentTime {
  int32_t year;
  int32_t month;
  int32_t day;
  int32_t hour;
  int32_t minutes;
  int32_t seconds;
};

//----------

void SAT_GetLocalTime(SAT_CurrentTime* ATime) {
  time_t t       = time(nullptr);
  struct tm tm   = *localtime(&t); // cppcheck recommends localtime_r
  ATime->year    = tm.tm_year + 1900;
  ATime->month   = tm.tm_mon + 1;
  ATime->day     = tm.tm_mday;
  ATime->hour    = tm.tm_hour;
  ATime->minutes = tm.tm_min;
  ATime->seconds = tm.tm_sec;

//
//
//  const time_t* sourceTime = nullptr;
//  struct tm* time = nullptr;
//  time = localtime(const time_t* sourceTime);
//
//  ATime->year    = 0;
//  ATime->month   = 0;
//  ATime->day     = 0;
//  ATime->hour    = 0;
//  ATime->minutes = 0;
//  ATime->seconds = 0;
}

//----------

// usleep is deprecated

//void SAT_Sleep(SAT_ui32 ms) {
//  usleep(ms*1000); // ms*1000;
//}

int SAT_Sleep(long ms) {
  return 0;
}

//----------

//----------------------------------------------------------------------
// timer
//----------------------------------------------------------------------

//class SAT_TimerListener {
//  public:
//    virtual void on_timerCallback(void) {}
//};

//----------

//void sat_timer_callback(sigval val) {
//  SAT_TimerListener* listener = (SAT_TimerListener*)val.sival_ptr;
//  if (listener) listener->on_timerCallback();
//}

//----------

//class SAT_Timer {
//
//private:
//
//
//public:
//
//  SAT_Timer() {
//  }
//
//  //----------
//
//  ~SAT_Timer() {
//  }
//
//  //----------
//
//  void start(float ms, bool oneshot=false) {
//  }
//
//  //----------
//
//  void stop(void) {
//  }
//
//};

//----------------------------------------------------------------------
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
