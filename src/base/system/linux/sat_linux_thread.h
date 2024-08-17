#ifndef sat_linux_thread_included
#define sat_linux_thread_included
//----------------------------------------------------------------------

#include <pthread.h>
//#include <unistd.h> // sleep

#include "sat.h"
#include "base/system/sat_time.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#include <sys/syscall.h>
#include <sys/unistd.h>
#define gettid() syscall(SYS_gettid)

//----------

// uint32_t thread_id = SAT_GetThreadId();

pid_t SAT_GetProcessId() {
  return getpid();
}

//----------

pid_t SAT_GetThreadId() {
  return gettid();
}


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Thread {

private:

  bool        MThreadRunning;
  int32_t     MThreadSleep;
  void*       MUsr;
  pthread_t   MThreadHandle;

//------------------------------
public:
//------------------------------

  SAT_Thread() {
    MUsr = nullptr;
    MThreadRunning = false;
    MThreadSleep = -1;
    MThreadHandle = 0;
  }

  //----------

  virtual ~SAT_Thread() {
    if (MThreadRunning) stop();
  }

//------------------------------
public:
//------------------------------

  virtual void on_threadFunc(void* usr) { /*SAT_Trace("on_threadFunc\n");^*/ };

//------------------------------
private:
//------------------------------

  static
  void* threadProc(void* data) {
    //SAT_PRINT("threadProc\n");
    SAT_Thread* thr = (SAT_Thread*)data;
    if (thr) {
      if (thr->MThreadSleep >= 0) {
        while (thr->MThreadRunning) {
          thr->on_threadFunc(thr->MUsr);
          //usleep(thr->MThreadSleep*1000); //ms*1000;
          SAT_Sleep(thr->MThreadSleep);
        }
      }
      else {
        thr->on_threadFunc(thr->MUsr);
      }
    }
    return nullptr;
  }

//------------------------------
public:
//------------------------------

  void start(void* usr, int32_t ms=-1) { // -1 = no timer
    SAT_PRINT("start\n");
    MUsr = usr;
    MThreadSleep = ms;
    MThreadRunning = true;
    pthread_create(&MThreadHandle,nullptr,&threadProc,this);
  }

  //----------

  void stop(void) {
    SAT_PRINT("stop\n");
    MThreadRunning = false;
    void* ret;
    pthread_join(MThreadHandle,&ret);
  }

//------------------------------

};

//----------------------------------------------------------------------
#endif
