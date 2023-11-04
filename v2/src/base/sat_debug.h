#ifndef sat_debug_included
#define sat_debug_included
//----------------------------------------------------------------------

#include "sat.h"
#include "base/debug/sat_debug_assert.h"
#include "base/debug/sat_debug_breakpoint.h"
#include "base/debug/sat_debug_callstack.h"
#include "base/debug/sat_debug_crash_handler.h"
//#include "base/debug/sat_debug_memtrace.h"
//#include "base/debug/sat_debug_observer.h"
#include "base/debug/sat_debug_print.h"
//#include "base/debug/sat_debug_window.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Debug {

//------------------------------
private:
//------------------------------

  SAT_DebugPrint*   MPrint        = nullptr;
  SAT_CallStack     MCallStack    = {};
  SAT_CrashHandler  MCrashHandler = {};
  //SAT_MemTrace      MMemTrace     = {};
  //SAT_Observer      MObserver     = {};
  //SAT_DebugWindow*  MDebugWindow  = nullptr{};

//------------------------------
public:
//------------------------------

  SAT_Debug() {
  }

  //----------

  ~SAT_Debug() {
  }

//------------------------------
public:
//------------------------------

  void initialize(SAT_DebugPrint* APrint) {
    MPrint = APrint;
    #if defined (SAT_DEBUG)
      MCallStack.initialize(APrint);
      MCrashHandler.initialize(APrint,&MCallStack);
    #endif
  }

  //----------

  void cleanup() {
    #if defined (SAT_DEBUG)
      MCrashHandler.cleanup();
      MCallStack.cleanup();
    #else
    #endif
  }

//------------------------------
public:
//------------------------------

  void printCallStack() {
    MCallStack.print();
  }

  //----------

  void crashHandler(int sig) {
    MCrashHandler.crashHandler(sig);
  }

};

//----------------------------------------------------------------------
#endif
