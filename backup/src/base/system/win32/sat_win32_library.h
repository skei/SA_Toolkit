#ifndef sat_win32_library_included
#define sat_win32_library_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

class SAT_Library {

protected:

  bool  MLibLoaded;
  //void* MLibHandle;

public:

  SAT_Library() {
    MLibLoaded = false;
    //MLibHandle = nullptr;
  }

  ~SAT_Library() {
    MLibLoaded = false;
    //MLibHandle = nullptr;
  }

//public:
//  bool  loaded(void) { return MLibLoaded; }

public:

  virtual
  bool loadLib(const char* AFilename) {
   // MLibHandle = dlopen(AFilename,RTLD_LAZY | RTLD_LOCAL ); // RTLD_NOW, RTLD_LAZY
   // if (!MLibHandle) return false;
   // MLibLoaded = true;
   // return true;
   return false;
  }

  virtual
  void unloadLib(void) {
    //if (MLibLoaded && MLibHandle) {
    //  dlclose(MLibHandle);
    //  MLibLoaded = false;
    //  MLibHandle = nullptr;
    //}
  }

  virtual
  void* getLibSymbol(const char* ASymbol) {
    //void* result = dlsym(MLibHandle,ASymbol);
    //return result;
    return nullptr;
  }

};

//----------------------------------------------------------------------
#endif
