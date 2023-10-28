#ifndef sat_global_included
#define sat_global_included
//----------------------------------------------------------------------

#include "base/debug/sat_debug_print.h"
#include "base/sat_log.h"
#include "base/sat_registry.h"
#include "base/sat_test.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Global {

public:

  SAT_DebugPrint  PRINT                             = {};
  SAT_Log         LOG                               = {};
  SAT_Registry    REGISTRY                          = {};
  SAT_Test        TEST                              = {};

  char            binary_path[SAT_MAX_PATH_LENGTH]  = {0};  // our binary
  char            plugin_path[SAT_MAX_PATH_LENGTH]  = {0};  // path received from host

  // HINSTANCE
  
public:

  SAT_Global() {
    //#if defined(SAT_PLUGIN_EXE)
    //  SAT_GetExePath(binary_path);
    //  DEBUG.print("exe path '%s'\n",binary_path);
    //#else
    //  SAT_GetLibPath(binary_path);
    //  DEBUG.print("lib path '%s'\n",binary_path);
    //#endif
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
    PRINT.initialize();
    LOG.initialize(&PRINT);
    REGISTRY.initialize(&PRINT);
    TEST.initialize(&PRINT);
  }

  //----------

  void cleanup() {
    TEST.cleanup();
    REGISTRY.cleanup();
    LOG.cleanup();
    PRINT.cleanup();
  }
  
  //----------
  
  void setBinaryPath(const char* APath) {
    strcpy(binary_path,APath);
  }
  
  const char* getBinaryPath() {
    return binary_path;
  }
  
  //----------
  
  void setPluginPath(const char* APath) {
    strcpy(plugin_path,APath);
  }
  
  const char* getPluginPath() {
    return plugin_path;
  }
  
//------------------------------
public:
//------------------------------

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//------------------------------
// log
//------------------------------

  #define SAT_LOG SAT_GLOBAL.LOG.print

//------------------------------
// print
//------------------------------

//#define SAT_Print SAT_GLOBAL.PRINT.print

void SAT_NoPrint(const char*,...) {}

#ifdef SAT_DEBUG_PRINT_PRETTY_FUNCTION

  #define SAT_Print \
    SAT_GLOBAL.PRINT.set_prefix( __FILE__, __PRETTY_FUNCTION__, __LINE__ ); \
    SAT_GLOBAL.PRINT.print

  #define SAT_DPrint \
    SAT_GLOBAL.PRINT.clear_prefix(); \
    SAT_GLOBAL.PRINT.print

  #define SAT_PRINT \
    SAT_GLOBAL.PRINT.set_prefix( __FILE__, __PRETTY_FUNCTION__, __LINE__ ); \
    SAT_GLOBAL.PRINT.print("\n")

#else // !pretty

  #define SAT_Print \
    SAT_GLOBAL.PRINT.set_prefix( __FILE__, __FUNCTION__, __LINE__ ); \
    SAT_GLOBAL.PRINT.print

  #define SAT_DPrint \
    SAT_GLOBAL.PRINT.clear_prefix(); \
    SAT_GLOBAL.PRINT.print

  #define SAT_PRINT \
    SAT_GLOBAL.PRINT.set_prefix( __FILE__, __FUNCTION__, __LINE__ ); \
    SAT_GLOBAL.PRINT.print("\n")
    
#endif // pretty
  
//----------

#ifndef SAT_DEBUG
#ifndef SAT_DEBUG_PRINT_ALWAYS

  #undef SAT_Print
  #undef SAT_DPrint
  #undef SAT_PRINT
  
  #define SAT_Print   SAT_NoPrint
  #define SAT_DPrint  SAT_NoPrint
  #define SAT_PRINT   {}
  
#endif // print always
#endif // debug

//----------------------------------------------------------------------
#endif
