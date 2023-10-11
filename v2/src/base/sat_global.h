#ifndef sat_global_included
#define sat_global_included
//----------------------------------------------------------------------

#include "base/sat_log.h"
#include "base/sat_registry.h"
#include "base/sat_test.h"
#include "base/sat_print.h"

class SAT_Global {

public:

  SAT_Log       LOG       = {};
  SAT_Registry  REGISTRY  = {};
  SAT_Test      TEST      = {};
  SAT_Print     PRINT     = {};

  char          binary_path[SAT_MAX_PATH_LENGTH]  = {0};  // our binary
  char          plugin_path[SAT_MAX_PATH_LENGTH]  = {0};  // path received from host

  // HINSTANCE
  
public:

  SAT_Global() {
    #if defined(SAT_PLUGIN_EXE)
      //SAT_GetExePath(binary_path);
      //DEBUG.print("exe path '%s'\n",binary_path);
    #else
      //SAT_GetLibPath(binary_path);
      //DEBUG.print("lib path '%s'\n",binary_path);
    #endif
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
    LOG.initialize();
    REGISTRY.initialize();
    TEST.initialize();
  }

  //----------

  void cleanup() {
    LOG.cleanup();
    REGISTRY.cleanup();
    TEST.cleanup();
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
#endif
