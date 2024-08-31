#ifndef sat_global_included
#define sat_global_included
//----------------------------------------------------------------------

#include "base/sat_debug.h"
#include "base/sat_log_file.h"
#include "base/sat_print.h"
#include "base/sat_registry.h"
#include "base/sat_test.h"
#include "base/sat_analytics.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Global {

//------------------------------
private:
//------------------------------

  bool          MInitialized                      = false;
  char          MPluginPath[SAT_MAX_PATH_LENGTH]  = {0};

//------------------------------
public:
//------------------------------

  SAT_Print       PRINT       = {};
  SAT_Debug       DEBUG       = {};
  SAT_LogFile     LOG         = {};
  SAT_Registry    REGISTRY    = {};
  SAT_UnitTest    TEST        = {};
  SAT_Analytics   ANALYTICS   = {};
  
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

  // initializes everything, in the correct order..

  bool initialize() {
    //SAT_PRINT("SAT_Global.initialize\n");
    if (!MInitialized) {
      if (!PRINT.initialize()) return false;
      if (!DEBUG.initialize(&PRINT)) return false;
      if (!LOG.initialize(&PRINT)) return false;
      if (!REGISTRY.initialize(&PRINT)) return false;
      if (!TEST.initialize(&PRINT)) return false;
      if (!ANALYTICS.initialize(&PRINT)) return false;
      MInitialized = true;
    }
    LOG.print("SAT_GLOBAL initialized\n");
    return true;
  }

  //----------

  // cleans up everything, in the correct order..
  // if the host just kills the process, will this be called?

  void cleanup() {
    if (MInitialized) {
      ANALYTICS.cleanup();
      TEST.cleanup();
      REGISTRY.cleanup();
      LOG.cleanup();
      DEBUG.cleanup();
      PRINT.cleanup();
      MInitialized = false;
    }
  }

//------------------------------
public:
//------------------------------

  // void clapInit(const char *APath) {
  //   setPluginPath(APath);
  // }

  //----------

  // void clapDeInit() {
  // }

  //----------

  void setPluginPath(const char* APath) {
    SAT_Strlcpy(MPluginPath,APath,SAT_MAX_PATH_LENGTH-1);
    MPluginPath[SAT_MAX_PATH_LENGTH-1] = 0;
  }

};

//----------------------------------------------------------------------
//
// SAT_GLOBAL
//
//----------------------------------------------------------------------

SAT_Global SAT_GLOBAL = {};

#include "base/sat_global_macros.h"

//----------------------------------------------------------------------
#endif