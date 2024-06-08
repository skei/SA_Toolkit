#ifndef sat_linux_library_included
#define sat_linux_library_included
//----------------------------------------------------------------------

/*
  -fno-gnu-unique
  On systems with recent GNU assembler and C library, the C++ compiler uses the
  "STB_GNU_UNIQUE" binding to make sure that definitions of template static
  data members and static local variables in inline functions are unique even
  in the presence of "RTLD_LOCAL"; this is necessary to avoid problems with a
  library used by two different "RTLD_LOCAL" plugins depending on a definition
  in one of them and therefore disagreeing with the other one about the binding
  of the symbol. But this causes "dlclose" to be ignored for affected DSOs; if
  your program relies on reinitialization of a DSO via "dlclose" and "dlopen",
  you can use -fno-gnu-unique.
*/

//----------------------------------------------------------------------

#include <dlfcn.h>

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

class SAT_Library {
  protected:
    bool  MLibLoaded;
    void* MLibHandle;

  public:

    SAT_Library() {
      MLibLoaded = false;
      MLibHandle = nullptr;
    }

    ~SAT_Library() {
      MLibLoaded = false;
      MLibHandle = nullptr;
    }

  //public:
  //  bool  loaded(void) { return MLibLoaded; }

  public:

    virtual
    bool loadLib(const char* AFilename) {
      MLibHandle = dlopen(AFilename,RTLD_LAZY | RTLD_LOCAL ); // RTLD_NOW, RTLD_LAZY
      if (!MLibHandle) return false;
      MLibLoaded = true;
      return true;
    }

    virtual
    void unloadLib(void) {
      if (MLibLoaded && MLibHandle) {
        dlclose(MLibHandle);
        MLibLoaded = false;
        MLibHandle = nullptr;
      }
    }

    virtual
    void* getLibSymbol(const char* ASymbol) {
      void* result = dlsym(MLibHandle,ASymbol);
      return result;
    }

};

//----------------------------------------------------------------------
#endif
