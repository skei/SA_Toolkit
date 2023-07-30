#ifndef sat_debug_call_stack_included
#define sat_debug_call_stack_included
//----------------------------------------------------------------------

#include "base/sat.h"

#ifdef SAT_DEBUG_CALL_STACK
#ifdef SAT_LINUX

  #include <execinfo.h>  // for backtrace
  #include <dlfcn.h>     // for dladdr
  #include <cxxabi.h>    // for __cxa_demangle
  
#endif // LINUX
#endif


//----------------------------------------------------------------------

//#ifdef SAT_DEBUG_MEMTRACE


class SAT_CallStack {

//------------------------------
public:
//------------------------------

  SAT_CallStack() {
  }
  
  //----------

  ~SAT_CallStack() {
  }
  
//------------------------------
public: // call stack
//------------------------------

  #ifdef SAT_DEBUG_CALL_STACK
  #ifdef SAT_LINUX
  
    // https://gist.github.com/fmela/591333/c64f4eb86037bb237862a8283df70cdfc25f01d3  

    void print_callstack(int skip = 1) {
//      print("\nCallstack:\n");
      void *callstack[128];
      const int nMaxFrames = sizeof(callstack) / sizeof(callstack[0]);
      int nFrames = backtrace(callstack, nMaxFrames);
      char **symbols = backtrace_symbols(callstack, nFrames);
      for (int i = skip; i < nFrames; i++) {
        Dl_info info;
        if (dladdr(callstack[i], &info)) {
          char *demangled = nullptr;
          int status;
          demangled = abi::__cxa_demangle(info.dli_sname, NULL, 0, &status);
          //snprintf(buf, sizeof(buf), "%-3d %*0p %s + %zd\n",
          //  i, 2 + sizeof(void*) * 2, callstack[i],
          //  status == 0 ? demangled : info.dli_sname,
          //  (char *)callstack[i] - (char *)info.dli_saddr);
//          const char* txt = status == 0 ? demangled : info.dli_sname;
//          print("  %i: %s\n",i,txt);
          ::free(demangled);
        }
        else {
          //snprintf(buf, sizeof(buf), "%-3d %*0p\n",
          //  i, 2 + sizeof(void*) * 2, callstack[i]);
//          print("  %i: %s\n",i,callstack[i]);
        }
        //snprintf(buf, sizeof(buf), "%s\n", symbols[i]);
        //SAT_Print("%s\n", symbols[i]);
      }
      ::free(symbols);
      if (nFrames == nMaxFrames) {
//        print("  [truncated]\n");
      }
    }

  #endif // linux
  #endif // call stack
  
};

//----------------------------------------------------------------------
#endif
