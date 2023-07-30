#ifndef sat_debug_crash_handler_included
#define sat_debug_crash_handler_included
//----------------------------------------------------------------------

#include "base/sat.h"

#ifdef SAT_LINUX
  #include <signal.h>
#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//void sat_crash_handler_callback2(int sig);

void sat_crash_handler_callback2(int sig) {
  //SAT_GLOBAL.DEBUG.crashHandler(sig);
}

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_CrashHandler {

//------------------------------
private:
//------------------------------

  #ifdef SAT_DEBUG_CRASH_HANDLER
    const char* MSignalNames[32] = {
      "(kill(pid,0))",
      "SIGHUP (Hangup)",
      "SIGINT (Interactive attention signal)",
      "SIGQUIT (Quit)",
      "SIGILL (Illegal instruction)",
      "SIGTRAP (Trace/breakpoint trap)",
      "SIGABRT (Abnormal termination)",
      "(-)",
      "SIGFPE (Erroneous arithmetic operation)",
      "SIGKILL (Killed)",
      "SIGBUS (Bus error)",
      "SIGSEGV (Invalid access to storage)",
      "SIGSYS (Bad system call)",
      "SIGPIPE (Broken pipe)",
      "SIGALRM (Alarm clock)",
      "SIGTERM (Termination request)",
      "SIGURG (Urgent data is available at a socket)",
      "SIGSTOP (Stop, unblockable)",
      "SIGTSTP (Keyboard stop)",
      "SIGCONT (Continue)",
      "SIGCHLD (Child terminated or stopped)",
      "SIGTTIN (Background read from control terminal)",
      "SIGTTOU (Background write to control terminal)",
      "SIGPOLL (Pollable event occurred)",
      "SIGXCPU (CPU time limit exceeded)",
      "SIGXFSZ (File size limit exceeded)",
      "SIGVTALRM (Virtual timer expired)",
      "SIGPROF (Profiling timer expired)",
      "SIGWINCH (Window size change)",
      "(-)",
      "SIGUSR1 (User-defined signal 1)",
      "SIGUSR2 (User-defined signal 2)"
    };
  #endif
  
//------------------------------
public:
//------------------------------

  SAT_CrashHandler() {
  }
  
  //----------

  ~SAT_CrashHandler() {
  }

//------------------------------
public: // crash handler
//------------------------------

  #ifdef SAT_DEBUG_CRASH_HANDLER

    void initCrashHandler(int sig) {
      #ifdef SAT_LINUX
        //signal(SIGSEGV,sat_crash_handler_callback);
        signal(sig,sat_crash_handler_callback2);
      #endif
    }

    //----------

    bool initCrashHandlers() {
      #ifdef SAT_LINUX
        //for (int i=0; i<32; i++) SAT_InitSignalHandler(i);
        initCrashHandler(SIGILL);
        initCrashHandler(SIGABRT);
        initCrashHandler(SIGFPE);
        initCrashHandler(SIGSEGV);
      #endif
      return true;
    }
    //----------
    
    void crashHandler(int sig) {
      #ifdef SAT_LINUX
//        print("\nCRASH!\n");
//        print("  %i : %s\n",sig,MSignalNames[sig]);
        //SAT_GLOBAL_WATCHES.printWatches("watched:");
        //SAT_DumpCallStack;
        //SAT_DumpCallStackSkip(0); // 2
//        print_callstack();
//        #ifdef SAT_DEBUG_OBSERVER
//          print_observers();
//        #endif
//        print("\n");
        exit(1); //_exit(1);
      #endif
    }  

  #endif // crash handler
  
};

//----------------------------------------------------------------------


//----------------------------------------------------------------------
#endif
