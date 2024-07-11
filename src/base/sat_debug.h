#ifndef sat_debug_included
#define sat_debug_included
//----------------------------------------------------------------------

#include "base/sat_print.h"

#ifdef SAT_DEBUG_ASSERT
  #include "base/debug/sat_debug_assert.h"
#endif

#ifdef SAT_DEBUG_BREAKPOINT
  #include "base/debug/sat_debug_breakpoint.h"
#endif

#ifdef SAT_DEBUG_CALLSTACK
  #include "base/debug/sat_debug_callstack.h"
#endif

#ifdef SAT_DEBUG_CRASH_HANDLER
  #include "base/debug/sat_debug_crash_handler.h"
#endif

#ifdef SAT_DEBUG_MEMTRACE
  #include "base/debug/sat_debug_memtrace.h"
#endif

#ifdef SAT_DEBUG_OBSERVER
  #include "base/debug/sat_debug_observer.h"
#endif

// #ifdef SAT_DEBUG_WINDOW
//   #include "base/debug/sat_debug_window.h"
// #endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Debug {

//------------------------------
private:
//------------------------------

  SAT_Print*  MPrint = nullptr;

//------------------------------
public:
//------------------------------

  // assert
  // breakpoint

  #ifdef SAT_DEBUG_CALLSTACK
    SAT_CallStack CALL_STACK = {};
  #endif

  #ifdef SAT_DEBUG_CRASH_HANDLER
    SAT_CrashHandler CRASH_HANDLER = {};
  #endif

  #ifdef SAT_DEBUG_MEMTRACE
    SAT_MemTrace MEMTRACE = {};
  #endif

  #ifdef SAT_DEBUG_OBSERVER
    SAT_Observer OBSERVER = {};
  #endif

  // #ifdef SAT_DEBUG_WINDOW
  //   SAT_DebugWindow WINDOW = {};
  // #endif

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

  bool initialize(SAT_Print* APrint) {
    //APrint->print("SAT_Debug.initialize\n");
    MPrint = APrint;

    #ifdef SAT_DEBUG_CALLSTACK
      if (!CALL_STACK.initialize(APrint)) return false;
    #endif

    #ifdef SAT_DEBUG_CRASH_HANDLER
      #ifdef SAT_DEBUG_CALLSTACK
        #ifdef SAT_DEBUG_OBSERVER
          if (!CRASH_HANDLER.initialize(APrint,&CALL_STACK,&OBSERVER)) return false;
        #else
          if (!CRASH_HANDLER.initialize(APrint,&CALL_STACK)) return false;
        #endif
      #else
        #ifdef SAT_DEBUG_OBSERVER
          if (!CRASH_HANDLER.initialize(APrint,&OBSERVER)) return false;
        #else
          if (!CRASH_HANDLER.initialize(APrint)) return false;
        #endif
      #endif
    #endif

    #ifdef SAT_DEBUG_MEMTRACE
      if (!MEMTRACE.initialize(APrint)) return false;
    #endif

    #ifdef SAT_DEBUG_OBSERVER
      if (!OBSERVER.initialize(APrint)) return false;
    #endif

    // #ifdef SAT_DEBUG_WINDOW
    //   if (!WINDOW.initialize(APrint)) return false;
    // #endif

    return true;
  }

  //----------

  void cleanup() {

    // #ifdef SAT_DEBUG_WINDOW
    //   WINDOW.cleanup();
    // #endif

    #ifdef SAT_DEBUG_OBSERVER
      OBSERVER.cleanup();
    #endif

    #ifdef SAT_DEBUG_MEMTRACE
      MEMTRACE.cleanup();
    #endif

    #ifdef SAT_DEBUG_CRASH_HANDLER
      CRASH_HANDLER.cleanup();
    #endif

    #ifdef SAT_DEBUG_CALLSTACK
      CALL_STACK.cleanup();
    #endif
  }
};

//----------------------------------------------------------------------
#endif