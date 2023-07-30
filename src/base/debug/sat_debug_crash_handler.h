#ifndef sat_debug_crash_handler_included
#define sat_debug_crash_handler_included
//----------------------------------------------------------------------

/*
  crash handler:
  compile with: -g -rdynamic
  // https://lasr.cs.ucla.edu/vahab/resources/signals.html
  // The only two signals for which a handler cannot be defined are SIGKILL and SIGSTOP.
*/

#ifdef SAT_DEBUG_CRASH_HANDLER

  #ifndef SAT_LINUX
    #error only linux supported..
  #endif

  //----------

  #include <signal.h>

  #include "base/sat.h"
  #include "base/debug/sat_debug_call_stack.h"
  #include "base/debug/sat_debug_observer.h"

  //----------

  void sat_crash_handler_callback_(int sig);

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
      initCrashHandlers();
    }
    
    //----------

    ~SAT_CrashHandler() {
      cleanupCrashHandlers();
    }

  //------------------------------
  public: // crash handler
  //------------------------------

    void initCrashHandler(int sig) {
      #ifdef SAT_LINUX
        //signal(SIGSEGV,sat_crash_handler_callback);
        signal(sig,sat_crash_handler_callback_);
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
    
    bool cleanupCrashHandlers() {
      return true;
    }
    
    //----------
    
    void crashHandler(int sig) {
      #ifdef SAT_LINUX
        SAT_DPrint("\nCRASH!\n");
        SAT_DPrint("  %i : %s\n",sig,MSignalNames[sig]);
        //SAT_GLOBAL_WATCHES.printWatches("watched:");
        //SAT_DumpCallStack;
        //SAT_DumpCallStackSkip(0); // 2
        SAT_PrintCallStack();
        #ifdef SAT_DEBUG_OBSERVER
        SAT_PrintObservers();
        #endif
        SAT_DPrint("\n");
        exit(1); //_exit(1);
      #endif
    }  

  //------------------------------
  private:
  //------------------------------

  };

  //----------------------------------------------------------------------
  //
  // TODO: -> SAT_GLOBAL.DEBUG
  //
  //----------------------------------------------------------------------

  SAT_CrashHandler SAT_GLOBAL_CRASH_HANDLER = {};

  //----------

  void sat_crash_handler_callback_(int sig) {
    //SAT_GLOBAL.DEBUG.crashHandler(sig);
    SAT_GLOBAL_CRASH_HANDLER.crashHandler(sig);
  }

#endif // SAT_DEBUG_CRASH_HANDLER

//----------------------------------------------------------------------
#endif
