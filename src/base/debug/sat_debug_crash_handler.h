#ifndef sat_debug_crash_handler_included
#define sat_debug_crash_handler_included
//----------------------------------------------------------------------

#ifdef SAT_DEBUG_CRASH_HANDLER

/*
  crash handler:
  compile with: -g -rdynamic
  // https://lasr.cs.ucla.edu/vahab/resources/signals.html
  // The only two signals for which a handler cannot be defined are SIGKILL and SIGSTOP.
*/


#include "sat.h"

// #if defined(SAT_WIN32)
//   #undef SAT_DEBUG_CRASH_HANDLER
// #endif

// #if !defined (SAT_LINUX)
//   #error only linux supported..
// #endif

  //----------

#include "sat.h"
#include "base/debug/sat_debug_callstack.h"

#ifdef SAT_DEBUG_OBSERVER
  #include "base/debug/sat_debug_observer.h"
#endif

#include <signal.h>

//----------

//void sat_crash_handler_callback_(int sig);


extern 
void sat_crash_handler_callback(int sig);

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_CrashHandler {

//------------------------------
private:
//------------------------------

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
  
//------------------------------
public:
//------------------------------

  SAT_CrashHandler() {
  }
  
  //----------

  ~SAT_CrashHandler() {
  }

//------------------------------
private:
//------------------------------

  SAT_Print*      MPrint      = nullptr;
  SAT_CallStack*  MCallStack  = nullptr;

  #ifdef SAT_DEBUG_OBSERVER
  SAT_Observer*   MObserver   = nullptr;
  #endif

//------------------------------
public:
//------------------------------

  #ifdef SAT_DEBUG_CALLSTACK

    #ifdef SAT_DEBUG_OBSERVER

      bool initialize(SAT_Print* APrint, SAT_CallStack* ACallStack, SAT_Observer* AObserver) {
        MPrint = APrint;
        MCallStack = ACallStack;
        MObserver = AObserver;
        setupCrashHandlers();
        return true;
      }

    #else

      bool initialize(SAT_Print* APrint, SAT_CallStack* ACallStack) {
        MPrint = APrint;
        MCallStack = ACallStack;
        setupCrashHandlers();
        return true;
      }

    #endif

  #else

    #ifdef SAT_DEBUG_OBSERVER

      bool initialize(SAT_Print* APrint, SAT_Observer* AObserver) {
        MPrint = APrint;
        MObserver = AObserver;
        setupCrashHandlers();
        return true;
      }

    #else

      bool initialize(SAT_Print* APrint) {
        MPrint = APrint;
        setupCrashHandlers();
        return true;
      }

    #endif

  #endif

  //----------


  //----------

  void cleanup() {
    cleanupCrashHandlers();
  }

//------------------------------
public: // crash handler
//------------------------------

  void setupCrashHandler(int sig) {
    #ifdef SAT_LINUX
      //signal(SIGSEGV,sat_crash_handler_callback);
      signal(sig,sat_crash_handler_callback);
    #endif
  }

  //----------

  bool setupCrashHandlers() {
    #ifdef SAT_LINUX
      //for (int i=0; i<32; i++) SAT_InitSignalHandler(i);
      setupCrashHandler(SIGILL);
      setupCrashHandler(SIGABRT);
      setupCrashHandler(SIGFPE);
      setupCrashHandler(SIGSEGV);
    #endif
    return true;
  }

  //----------
  
  bool cleanupCrashHandlers() {
    return true;
  }
  
  //----------

  void crashHandler(int sig) {
    #ifdef SAT_LINUX
      if (MPrint) {
        //MPrint->set_prefix( __FILE__, __PRETTY_FUNCTION__, __LINE__ );
        //MPrint->print("\n");
        MPrint->print("CRASH!\n");
        MPrint->print("  %i : %s\n",sig,MSignalNames[sig]);
      }
      //SAT_GLOBAL_WATCHES.printWatches("watched:");
      if (MCallStack) MCallStack->print();
      #ifdef SAT_DEBUG_OBSERVER
        MObserver->print_observers();
      #endif
      //if (MPrint) MPrint->print("\n");
      exit(1); //_exit(1);
    #endif
  }  

//------------------------------
private:
//------------------------------

  // static
  // void sat_crash_handler_callback(int sig) {
  //   //SAT_GLOBAL.DEBUG.crashHandler(sig);
  //   //SAT_GLOBAL.DEBUG.crashHandler(sig);
  // }


};

#endif // SAT_DEBUG_CRASH_HANDLER

//----------------------------------------------------------------------
#endif