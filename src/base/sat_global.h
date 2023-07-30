#ifndef sat_global_included
#define sat_global_included
//----------------------------------------------------------------------

#include "base/sat.h"
//#include "base/sat_debug.h"
#include "base/system/sat_paths.h"
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

  char          binary_path[SAT_MAX_PATH_LENGTH]  = {0};
  char          plugin_path[SAT_MAX_PATH_LENGTH]  = {0};
  SAT_Registry  REGISTRY                          = {};
  SAT_LogFile   LOG                               = {};
  
//------------------------------
public:
//------------------------------

  SAT_Global() {
//    #ifdef SAT_EXE
//      SAT_GetExePath(binary_path);
//      //DEBUG.print("exe path '%s'\n",binary_path);
//    #endif
//    #ifdef SAT_PLUGIN
//      SAT_GetLibPath(binary_path);
//      //DEBUG.print("lib path '%s'\n",binary_path);
//    #endif
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
    //LOG.print("SAT_GLOBAL.initialize\n");
    LOG.initialize();
    REGISTRY.initialize();
  }

  //----------

  void cleanup() {
    //LOG.print("SAT_GLOBAL.cleanup\n");
    LOG.cleanup();
    REGISTRY.cleanup();
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
  
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

SAT_Global SAT_GLOBAL = {};

#define SAT_Log SAT_GLOBAL.LOG.print

//----------------------------------------------------------------------
#endif
