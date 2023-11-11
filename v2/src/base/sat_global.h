#ifndef sat_global_included
#define sat_global_included
//----------------------------------------------------------------------

#include "base/debug/sat_debug_print.h"
#include "base/utils/sat_strutils.h"
#include "base/sat_debug.h"
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
  SAT_Debug       DEBUG                             = {};
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
    DEBUG.initialize(&PRINT);
    LOG.initialize(&PRINT);
    REGISTRY.initialize(&PRINT);
    TEST.initialize(&PRINT);
  }

  //----------

  void cleanup() {
    TEST.cleanup();
    REGISTRY.cleanup();
    LOG.cleanup();
    DEBUG.cleanup();
    PRINT.cleanup();
  }
  
  //----------
  
  void setBinaryPath(const char* APath) {
    SAT_Strlcpy(binary_path,APath,SAT_MAX_PATH_LENGTH);
  }
  
  const char* getBinaryPath() {
    return binary_path;
  }
  
  //----------
  
  void setPluginPath(const char* APath) {
    SAT_Strlcpy(plugin_path,APath,SAT_MAX_PATH_LENGTH);
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

SAT_Global SAT_GLOBAL = {};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//------------------------------
// debug
//------------------------------

void sat_crash_handler_callback(int sig) {
  SAT_GLOBAL.DEBUG.crashHandler(sig);
}

//------------------------------
// log
//------------------------------

  #define SAT_LOG SAT_GLOBAL.LOG.print

//------------------------------
// memtrace
//------------------------------

  #ifdef SAT_DEBUG_MEMTRACE

    static __thread char*         sat_memtrace_prefix_file;
    static __thread unsigned int  sat_memtrace_prefix_line;

    unsigned int sat_memtrace_prefix(const char* file, const unsigned int line) {
      sat_memtrace_prefix_file = (char*)file;
      sat_memtrace_prefix_line = line;
      return 1;
    }

    void* operator  new       (const size_t size, const char* file, unsigned int line)  { return SAT_GLOBAL.DEBUG.MEMTRACE.malloc(size, file, line, 1); }
    void* operator  new[]     (const size_t size, const char* file, unsigned int line)  { return SAT_GLOBAL.DEBUG.MEMTRACE.malloc(size, file, line, 1); }
    void  operator  delete    (void* ptr)                                               { return SAT_GLOBAL.DEBUG.MEMTRACE.free(ptr, sat_memtrace_prefix_file, sat_memtrace_prefix_line, 1); }
    void  operator  delete[]  (void* ptr)                                               { return SAT_GLOBAL.DEBUG.MEMTRACE.free(ptr, sat_memtrace_prefix_file, sat_memtrace_prefix_line, 1); }

    #define malloc(s)     SAT_GLOBAL.DEBUG.MEMTRACE.malloc(  s,    __FILE__, __LINE__ )
    #define calloc(n,s)   SAT_GLOBAL.DEBUG.MEMTRACE.calloc(  n, s, __FILE__, __LINE__ )
    #define realloc(p,s)  SAT_GLOBAL.DEBUG.MEMTRACE.realloc( p, s, __FILE__, __LINE__ )
    #define free(p)       SAT_GLOBAL.DEBUG.MEMTRACE.free(    p,    __FILE__, __LINE__ )

    #define new           new(__FILE__, __LINE__)
    #define delete        if (sat_memtrace_prefix(__FILE__, __LINE__)) delete

  #endif

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
