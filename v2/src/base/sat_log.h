#ifndef sat_log_included
#define sat_log_included
//----------------------------------------------------------------------

#include "base/debug/sat_debug_print.h"

class SAT_Log {

//------------------------------
public:
//------------------------------

  SAT_Log() {
    #if !defined (SAT_NO_LOG)
    #else
    #endif
  }

  //----------

  ~SAT_Log() {
    #if !defined (SAT_NO_LOG)
    #else
    #endif
  }

//------------------------------
public:
//------------------------------

  void initialize(SAT_DebugPrint* APrint) {
    #if !defined (SAT_NO_LOG)
    #else
    #endif
  }

  //----------

  void cleanup() {
    #if !defined (SAT_NO_LOG)
    #else
    #endif
  }

};

//----------------------------------------------------------------------
#endif
