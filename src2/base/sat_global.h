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
    return true;
  }

  //----------

  // cleans up everything, in the correct order..

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

  void clapInit(const char *APath) {
    setPluginPath(APath);
  }

  //----------

  void clapDeInit() {
  }

  //----------

  void setPluginPath(const char* APath) {
    SAT_Strlcpy(MPluginPath,APath,SAT_MAX_PATH_LENGTH-1);
    MPluginPath[SAT_MAX_PATH_LENGTH-1] = 0;
  }

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
// crash handler
//------------------------------

#ifdef SAT_DEBUG_CRASH_HANDLER

  void sat_crash_handler_callback(int sig) {
    SAT_GLOBAL.DEBUG.CRASH_HANDLER.crashHandler(sig);
  }

#endif

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

  // NOT safe!
  // a hack, but it works.. kind of..
  
  unsigned int sat_memtrace_prefix(const char* file, const unsigned int line) {
    sat_memtrace_prefix_file = (char*)file;
    sat_memtrace_prefix_line = line;
    return 1;
  }

  /*
    void* operator new  (std::size_t count );
    void* operator new[](std::size_t count );
    void* operator new  (std::size_t count, const std::nothrow_t& tag);
    void* operator new[](std::size_t count, const std::nothrow_t& tag);

    void operator delete  (void* ptr); 	
    void operator delete[](void* ptr);
    void operator delete  (void* ptr, const std::nothrow_t& tag);
    void operator delete[](void* ptr, const std::nothrow_t& tag);
    void operator delete  (void* ptr, std::size_t sz);
    void operator delete[](void* ptr, std::size_t sz);
  */

  // //void operator delete (void* ptr) /*throw()*/ {}
  // void operator delete (void* ptr) _GLIBCXX_USE_NOEXCEPT {}

  // //void operator delete[] (void* ptr) /*throw()*/ {0
  // void operator delete [] (void* ptr) _GLIBCXX_USE_NOEXCEPT {0


  void* operator  new       (const size_t size, const char* file, unsigned int line)  { return SAT_GLOBAL.DEBUG.MEMTRACE.malloc(size, file, line, 1); }
  void* operator  new[]     (const size_t size, const char* file, unsigned int line)  { return SAT_GLOBAL.DEBUG.MEMTRACE.malloc(size, file, line, 1); }

  void  operator  delete    (void* ptr)                                               { return SAT_GLOBAL.DEBUG.MEMTRACE.free(ptr, sat_memtrace_prefix_file, sat_memtrace_prefix_line, 1); }
  void  operator  delete[]  (void* ptr)                                               { return SAT_GLOBAL.DEBUG.MEMTRACE.free(ptr, sat_memtrace_prefix_file, sat_memtrace_prefix_line, 1); }

  #define new     new(__FILE__, __LINE__)
  #define delete  if (sat_memtrace_prefix(__FILE__, __LINE__)) delete

  #define malloc(s)     SAT_GLOBAL.DEBUG.MEMTRACE.malloc(  s,    __FILE__, __LINE__ )
  #define calloc(n,s)   SAT_GLOBAL.DEBUG.MEMTRACE.calloc(  n, s, __FILE__, __LINE__ )
  #define realloc(p,s)  SAT_GLOBAL.DEBUG.MEMTRACE.realloc( p, s, __FILE__, __LINE__ )
  #define free(p)       SAT_GLOBAL.DEBUG.MEMTRACE.free(    p,    __FILE__, __LINE__ )

#endif

//------------------------------
// print
//------------------------------

//#define SAT_PRINT SAT_GLOBAL.PRINT.print

void SAT_NoPrint(const char*,...) {}

#ifdef SAT_PRINT_PRETTY_FUNCTION

  #define SAT_PRINT \
    SAT_GLOBAL.PRINT.setPrefix( __FILE__, __PRETTY_FUNCTION__, __LINE__ ); \
    SAT_GLOBAL.PRINT.print

  #define SAT_DPRINT \
    SAT_GLOBAL.PRINT.clearPrefix(); \
    SAT_GLOBAL.PRINT.print

  #define SAT_TRACE \
    SAT_GLOBAL.PRINT.setPrefix( __FILE__, __PRETTY_FUNCTION__, __LINE__ ); \
    SAT_GLOBAL.PRINT.print("\n")

#else // !pretty

  #define SAT_PRINT \
    SAT_GLOBAL.PRINT.setPrefix( __FILE__, __FUNCTION__, __LINE__ ); \
    SAT_GLOBAL.PRINT.print

  #define SAT_DPRINT \
    SAT_GLOBAL.PRINT.clearPrefix(); \
    SAT_GLOBAL.PRINT.print

  #define SAT_TRACE \
    SAT_GLOBAL.PRINT.setPrefix( __FILE__, __FUNCTION__, __LINE__ ); \
    SAT_GLOBAL.PRINT.print("\n")
    
#endif // pretty
  
//----------

#ifndef SAT_DEBUG
#ifndef SAT_DEBUG_PRINT_ALWAYS

  #undef SAT_PRINT
  #undef SAT_DPRINT
  #undef SAT_TRACE
  
  #define SAT_PRINT   SAT_NoPrint
  #define SAT_DPRINT  SAT_NoPrint
  #define SAT_TRACE   {}
  
#endif // print always
#endif // debug

//------------------------------
// tests
//------------------------------

// see SAT_Global.h

#ifdef SAT_INCLUDE_TESTS

  #define SAT_TEST(name,func)                                         \
                                                                      \
    bool sat__test_register_ ##func() {                               \
      SAT_GLOBAL.TEST.addTest(name,func);                             \
      return true;                                                    \
    }                                                                 \
                                                                      \
    bool sat__test_registered_ ##func = sat__test_register_ ##func();

  #define SAT_RUN_TESTS SAT_GLOBAL.TEST.runAllTests()

#else // !SAT_TESTS

  #define SAT_TEST(name,func)
  //#define SAT_RUN_TESTS SAT_GLOBAL.TEST.runAllTests();
  #define SAT_RUN_TESTS

#endif

//----------------------------------------------------------------------
#endif