#ifndef sat_global_included
#define sat_global_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "base/sat_debug.h"
#include "base/utils/sat_log_file.h"
#include "plugin/sat_registry.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Global {

//------------------------------
public:
//------------------------------

  bool          isInitialized = false;
  SAT_Debug     DEBUG         = {};
  SAT_LogFile   LOG           = {};
  SAT_Registry  REGISTRY      = {};
  
//------------------------------
public:
//------------------------------

  SAT_Global() {
    initialize();
  }

  //----------

  ~SAT_Global() {
    cleanup();
  }

//------------------------------
public:
//------------------------------

  void initialize() {
    if (!isInitialized) {
      //LOG.print("SAT_GLOBAL.initialize\n");
      DEBUG.initialize();
      LOG.initialize();
      REGISTRY.initialize();
      isInitialized = true;
    }
  }

  //----------

  void cleanup() {
    //LOG.print("SAT_GLOBAL.cleanup\n");
    if (isInitialized) {
      DEBUG.cleanup();
      LOG.cleanup();
      REGISTRY.cleanup();
      isInitialized = false;
    }
  }
  
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

SAT_Global SAT_GLOBAL = {};

//----------------------------------------------------------------------
#endif
