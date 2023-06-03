#ifndef sat_global_included
#define sat_global_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "base/sat_debug.h"
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

  bool          isInitialized             = false;
  char          binary_path[SAT_MAX_PATH_LENGTH] = {0};
  char          plugin_path[SAT_MAX_PATH_LENGTH] = {0};

  SAT_Debug     DEBUG                     = {};
  SAT_LogFile   LOG                       = {};
  SAT_Registry  REGISTRY                  = {};
  
//------------------------------
public:
//------------------------------

  SAT_Global() {
    
    #ifdef SAT_EXE
      SAT_GetExePath(binary_path);
      //DEBUG.print("exe path '%s'\n",binary_path);
    #endif
    
    #ifdef SAT_PLUGIN
      SAT_GetLibPath(binary_path);
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

//----------------------------------------------------------------------
#endif
