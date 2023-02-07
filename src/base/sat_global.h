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
      DEBUG.initialize();
      LOG.initialize();
      REGISTRY.initialize();
      isInitialized = true;
    }    //LOG.print("SAT_GLOBAL.initialize\n");
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

SAT_Global SAT_GLOBAL = {};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//------------------------------
// log
//------------------------------

#define SAT_Log SAT_GLOBAL.LOG.print

//----------


#ifdef SAT_DEBUG

  //------------------------------
  // print
  //------------------------------

  #define SAT_Print \
    SAT_GLOBAL.DEBUG.set_prefix( __FILE__, __FUNCTION__, __LINE__ ); \
    SAT_GLOBAL.DEBUG.print

  #define SAT_DPrint \
    SAT_GLOBAL.DEBUG.clear_prefix(); \
    SAT_GLOBAL.DEBUG.print

  #define SAT_PRINT \
    SAT_GLOBAL.DEBUG.set_prefix( __FILE__, __FUNCTION__, __LINE__ ); \
    SAT_GLOBAL.DEBUG.print("\n")

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

    void* operator new (const size_t size, const char* file, unsigned int line) {
      return SAT_GLOBAL.DEBUG.malloc(size, file, line, 1);
    }

    void* operator new[] (const size_t size, const char* file, unsigned int line) {
      return SAT_GLOBAL.DEBUG.malloc(size, file, line, 1);
    }

    void operator delete (void* ptr) {
      return SAT_GLOBAL.DEBUG.free(ptr, sat_memtrace_prefix_file, sat_memtrace_prefix_line, 1);
    }

    void operator delete[] (void* ptr) {
      return SAT_GLOBAL.DEBUG.free(ptr, sat_memtrace_prefix_file, sat_memtrace_prefix_line, 1);
    }

    #define malloc(s)       SAT_GLOBAL.DEBUG.malloc(s, __FILE__, __LINE__)
    #define calloc(n, s)    SAT_GLOBAL.DEBUG.calloc(n, s, __FILE__, __LINE__)
    #define realloc(p, s)   SAT_GLOBAL.DEBUG.realloc(p, s, __FILE__, __LINE__)
    #define free(p)         SAT_GLOBAL.DEBUG.free(p, __FILE__, __LINE__)

    #define new             new(__FILE__, __LINE__)
    #define delete          if (sat_memtrace_prefix(__FILE__, __LINE__)) delete

  #endif // memtrace

#else // debug

  // print
  void SAT_NoPrint(const char*,...) {}
  #define SAT_Print    SAT_NoPrint
  #define SAT_DPrint   SAT_NoPrint
  #define SAT_PRINT {}

#endif // debug

//----------------------------------------------------------------------
#endif
