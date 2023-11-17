#ifndef sat_debug_included
#define sat_debug_included
//----------------------------------------------------------------------
/*

  * flagging mechanisms for fake positives?
    (if ptr == defined_false_positive) print("false positive..")
  * max memory allocated
  * max events in queues
  * timing
  * reality checks (did we quit normally?)
  * memtrace: destructors mark themselves in separate list (destroyed objects)
    maybe new to? ([un-]registerAllocation(classname,ptr)
    then we can compare lists..

  * can we have line pos, etc from signal, crash handler?

*/
//----------------------------------------------------------------------

#include "sat.h"
#include "base/debug/sat_debug_assert.h"
#include "base/debug/sat_debug_breakpoint.h"
#include "base/debug/sat_debug_callstack.h"
#include "base/debug/sat_debug_crash_handler.h"
#include "base/debug/sat_debug_memtrace.h"
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
//private:
public:
//------------------------------

  SAT_DebugPrint*   MPrint        = nullptr;

  SAT_CallStack     CALLSTACK     = {};
  SAT_CrashHandler  CRASHHANDLER  = {};
  SAT_MemTrace      MEMTRACE      = {};

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
      CALLSTACK.initialize(APrint);
      CRASHHANDLER.initialize(APrint,&CALLSTACK);
      MEMTRACE.initialize(APrint);
    #endif
  }

  //----------

  void cleanup() {
    //MPrint->print("debug cleanup\n");
    #if defined (SAT_DEBUG)
      MEMTRACE.cleanup();
      CRASHHANDLER.cleanup();
      CALLSTACK.cleanup();
    #else
    #endif
  }

//------------------------------
public:
//------------------------------

  void printCallStack() {
    CALLSTACK.print();
  }

  //----------

  void crashHandler(int sig) {
    CRASHHANDLER.crashHandler(sig);
  }

};

//----------------------------------------------------------------------
#endif
