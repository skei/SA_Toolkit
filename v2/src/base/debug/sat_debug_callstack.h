#ifndef sat_debug_callstack_included
#define sat_debug_callstack_included
//----------------------------------------------------------------------

#include "sat.h"

/*
  callstack:
  https://gist.github.com/fmela/591333/c64f4eb86037bb237862a8283df70cdfc25f01d3  
  "If you are linking your binary using GNU ld you need to add --export-dynamic
  or most of your symbols will just be resolved to the name of the binary."
  (linker flag, not a compiler flag. You might need to try -Wl,--export-dynamic)
*/

#if defined (SAT_WIN32)
  #undef SAT_DEBUG_CALLSTACK
#endif

#if !defined (SAT_LINUX)
  #error only linux supported..
#endif

#if defined(SAT_DEBUG_CALLSTACK)
  #include <execinfo.h>  // for backtrace
  #include <dlfcn.h>     // for dladdr
  #include <cxxabi.h>    // for __cxa_demangle
#endif
    
//----------------------------------------------------------------------

class SAT_CallStack {

//------------------------------
private:
//------------------------------

  SAT_DebugPrint* MPrint = nullptr;

//------------------------------
public:
//------------------------------

  SAT_CallStack() {
  }
  
  //----------

  ~SAT_CallStack() {
    
  }
    
//------------------------------
public:
//------------------------------

  void initialize(SAT_DebugPrint* APrint) {
    MPrint = APrint;
  }

  //----------

  void cleanup() {
  }

//------------------------------
public: // call stack
//------------------------------

  // https://gist.github.com/fmela/591333/c64f4eb86037bb237862a8283df70cdfc25f01d3  

  #if defined (SAT_DEBUG_CALLSTACK)

    void print(int skip = 1) {
      if (MPrint) MPrint->print("Callstack:\n");
      void* callstack[128];
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
          const char* txt = status == 0 ? demangled : info.dli_sname;
          if (MPrint) MPrint->print("  %i: %s\n",i,txt);
          ::free(demangled);
        }
        else {
          //snprintf(buf, sizeof(buf), "%-3d %*0p\n",
          //  i, 2 + sizeof(void*) * 2, callstack[i]);
          if (MPrint) MPrint->print("  %i: %s\n",i,(char*)callstack[i]);
        }
        //snprintf(buf, sizeof(buf), "%s\n", symbols[i]);
        //SAT_Print("%s\n", symbols[i]);
      }
      ::free(symbols);
      if (nFrames == nMaxFrames) {
        if (MPrint) MPrint->print("  [truncated]\n");
      }
    }

  #else

    void print(int skip = 1) {
    }

  #endif

};

//----------------------------------------------------------------------
#endif
