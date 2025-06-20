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

  bool    MInitialized                        = false;
  char    MPluginPath[SAT_MAX_PATH_LENGTH]    = {0};

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
    //LOG.print("SAT_GLOBAL initialized\n");
    return true;
  }

  //----------

  // cleans up everything, in the correct order..
  // .. if the host just kills the process, this will not be called, will it?

  void cleanup() {
    if (MInitialized) {
      ANALYTICS.cleanup();
      TEST.cleanup();
      REGISTRY.cleanup();
      //LOG.cleanup();
      DEBUG.cleanup();
      PRINT.cleanup();
      MInitialized = false;
    }
  }

  //----------

  // void clapInit(const char *APath) {
  //   setPluginPath(APath);
  // }

  //----------

  // void clapDeInit() {
  // }

//------------------------------
public:
//------------------------------

  const char* getOSName() {
    #ifdef SAT_LINUX
      return "Linux";
    #endif
    #ifdef SAT_MAC
      return "Mac";
    #endif
    #ifdef SAT_WIN32
      return "Windows";
    #endif
    return "Unknown";
  }

  //----------

  uint32_t getOS() {
    #ifdef SAT_LINUX
      return SAT_OS_LINUX;
    #endif
    #ifdef SAT_MAC
      return SAT_OS_MAC;
    #endif
    #ifdef SAT_WIN32
      return SAT_OS_WINDOWS;
    #endif
    return SAT_OS_UNKNOWN;
  }


  //----------

  void setPluginPath(const char* APath) {
    SAT_Strlcpy(MPluginPath,APath,SAT_MAX_PATH_LENGTH);
    MPluginPath[SAT_MAX_PATH_LENGTH-1] = 0;
  }

};

//----------------------------------------------------------------------
#endif