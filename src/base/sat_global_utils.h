#ifndef sat_global_utils_included
#define sat_global_utils_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "base/sat_global.h"
//#include "base/sat_debug.h"
//#include "base/utils/sat_log_file.h"
//#include "plugin/sat_registry.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//------------------------------
// log
//------------------------------

// always log..

#define SAT_Log SAT_GLOBAL.LOG.print

//------------------------------
// 
//------------------------------

#ifdef SAT_DEBUG

  //------------------------------
  // crash handler
  //------------------------------

  #ifdef SAT_DEBUG_CRASH_HANDLER
  
    void sat_crash_handler_callback(int sig) {
      SAT_GLOBAL.DEBUG.crashHandler(sig);
    }

  #endif // crash handler

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
    
    // more variations?

    #define malloc(s)       SAT_GLOBAL.DEBUG.malloc(  s,    __FILE__, __LINE__ )
    #define calloc(n, s)    SAT_GLOBAL.DEBUG.calloc(  n, s, __FILE__, __LINE__ )
    #define realloc(p, s)   SAT_GLOBAL.DEBUG.realloc( p, s, __FILE__, __LINE__ )
    #define free(p)         SAT_GLOBAL.DEBUG.free(    p,    __FILE__, __LINE__ )

    #define new             new(__FILE__, __LINE__)
    #define delete          if (sat_memtrace_prefix(__FILE__, __LINE__)) delete

  #endif // memtrace

  //------------------------------
  // observables
  //------------------------------

  #define SAT_Observe \
    SAT_GLOBAL.DEBUG.observe

  #define SAT_Unobserve \
    SAT_GLOBAL.DEBUG.unobserve

  #define SAT_PrintObservers \
    SAT_GLOBAL.DEBUG.print_observers

  //------------------------------
  // print
  //------------------------------
  
  #ifdef SAT_DEBUG_PRINT_PRETTY_FUNCTION

    #define SAT_Print \
      SAT_GLOBAL.DEBUG.set_prefix( __FILE__, __PRETTY_FUNCTION__, __LINE__ ); \
      SAT_GLOBAL.DEBUG.print

    #define SAT_DPrint \
      SAT_GLOBAL.DEBUG.clear_prefix(); \
      SAT_GLOBAL.DEBUG.print

    #define SAT_PRINT \
      SAT_GLOBAL.DEBUG.set_prefix( __FILE__, __PRETTY_FUNCTION__, __LINE__ ); \
      SAT_GLOBAL.DEBUG.print("\n")


  #else

    #define SAT_Print \
      SAT_GLOBAL.DEBUG.set_prefix( __FILE__, __FUNCTION__, __LINE__ ); \
      SAT_GLOBAL.DEBUG.print

    #define SAT_DPrint \
      SAT_GLOBAL.DEBUG.clear_prefix(); \
      SAT_GLOBAL.DEBUG.print

    #define SAT_PRINT \
      SAT_GLOBAL.DEBUG.set_prefix( __FILE__, __FUNCTION__, __LINE__ ); \
      SAT_GLOBAL.DEBUG.print("\n")
      
  #endif
  
//------------------------------
#else // ! debug
//------------------------------

  void SAT_NoPrint(const char*,...) {}

  #define SAT_Print   SAT_NoPrint
  #define SAT_DPrint  SAT_NoPrint
  #define SAT_PRINT   {}
  
  #define SAT_Observe(x,y,z) {}

#endif // debug

//----------------------------------------------------------------------
//
// debug window (hack alert!)
//
//----------------------------------------------------------------------

//#ifdef SAT_DEBUG_WINDOW
//
//  #include "base/debug/sat_debug_window.h"
//  
//  SAT_DebugWindow* DEBUG_WINDOW = nullptr;
//  
//  class _sat_debug_window_class {
//
//  public:
//    _sat_debug_window_class() {
//      SAT_PRINT;
//      SAT_Assert(!DEBUG_WINDOW);
//      DEBUG_WINDOW = new SAT_DebugWindow(640,480);
//      SAT_Assert(DEBUG_WINDOW);
//      DEBUG_WINDOW->show();
//    }
//    
//    ~_sat_debug_window_class() {
//      SAT_PRINT;
//      SAT_Assert(DEBUG_WINDOW);
//      DEBUG_WINDOW->hide();
//      delete DEBUG_WINDOW;
//      DEBUG_WINDOW = nullptr;
//    }
//  
//  };
//  
//  _sat_debug_window_class SAT_GLOBAL_DEBUG_WINDOW = {};
//
//#endif

//----------------------------------------------------------------------
#endif
