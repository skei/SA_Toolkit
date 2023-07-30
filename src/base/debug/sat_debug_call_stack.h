#ifndef sat_debug_call_stack_included
#define sat_debug_call_stack_included
//----------------------------------------------------------------------

/*
  callstack:
  https://gist.github.com/fmela/591333/c64f4eb86037bb237862a8283df70cdfc25f01d3  
  "If you are linking your binary using GNU ld you need to add --export-dynamic
  or most of your symbols will just be resolved to the name of the binary."
  (linker flag, not a compiler flag. You might need to try -Wl,--export-dynamic)
*/

#ifdef SAT_DEBUG_CALL_STACK

  #ifndef SAT_LINUX
    #error only linux supported..
  #endif

  //----------

  #include "base/sat.h"

  #include <execinfo.h>  // for backtrace
  #include <dlfcn.h>     // for dladdr
  #include <cxxabi.h>    // for __cxa_demangle
    
  //----------------------------------------------------------------------

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

    // https://gist.github.com/fmela/591333/c64f4eb86037bb237862a8283df70cdfc25f01d3  

    void print(int skip = 1) {
      SAT_DPrint("\nCallstack:\n");
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
          SAT_DPrint("  %i: %s\n",i,txt);
          ::free(demangled);
        }
        else {
          //snprintf(buf, sizeof(buf), "%-3d %*0p\n",
          //  i, 2 + sizeof(void*) * 2, callstack[i]);
          SAT_Print("  %i: %s\n",i,(char*)callstack[i]);
        }
        //snprintf(buf, sizeof(buf), "%s\n", symbols[i]);
        //SAT_Print("%s\n", symbols[i]);
      }
      ::free(symbols);
      if (nFrames == nMaxFrames) {
        SAT_DPrint("  [truncated]\n");
      }
    }

  };

  //----------

  SAT_CallStack SAT_GLOBAL_CALL_STACK = {};


  //----------

  void SAT_PrintCallStack(int skip=1) {
    //SAT_GLOBAL.DEBUG.crashHandler(sig);
    SAT_GLOBAL_CALL_STACK.print(skip);
  }
  
//------------------------------

#else // SAT_DEBUG_CALL_STACK

  void SAT_PrintCallStack(int skip=1) {
  }

#endif // SAT_DEBUG_CALL_STACK

//----------------------------------------------------------------------
#endif
