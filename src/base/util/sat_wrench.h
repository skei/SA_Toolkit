#ifndef sat_wrench_included
#define sat_wrench_included
//----------------------------------------------------------------------

// https://www.northarc.com/wrench/www/

//#define WRENCH_WITHOUT_COMPILER
//#define WRENCH_DEFAULT_STACK_SIZE 64

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#include "sat.h"
//#include "extern/wrench/wrench.h"
#include "extern/wrench/wrench.cpp.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

void sat_wrench_print(WRState* w, const WRValue* argv, const int argn, WRValue& retVal, void* usr) {
  char buf[1024];
  for (int i=0; i<argn; ++i) {
    SAT_DPrint( "%s", argv[i].asString(buf,1024));
  }
}

//----------

const char* sat_wrench_source_code = 
  " print(\"Hello World!\\n\"); "
  " for (i=0; i<10; i++) {      "
  "   print(i);                 "
  " }                           "
  " print(\"\\n\");             ";

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Wrench {
  
//------------------------------
private:
//------------------------------

  WRState*  MState = nullptr;

//------------------------------
public:
//------------------------------

  /*
    create/destroy a WRState object that can run multiple contexts/threads
  */

  SAT_Wrench() {
    MState = wr_newState();
    SAT_Assert(MState);
  }
  
  //----------
  
  /*
    Destroy a context you no longer need and free up all the memory it
    was using, all contexts are freed when wr_destroyState() is called,
    so calling this is not necessary
  */
  
  ~SAT_Wrench() {
    wr_destroyState(MState);
  }
  
//------------------------------
public:
//------------------------------

  /*
    compile source code and return a new'ed block of bytecode ready
    for wr_run(), memory must be delete[]'ed
    return value is a WRError
    optionally an "errMsg" buffer can be passed which will output a
    human-readable string of what went wrong and where.
  */

  uint8_t* compile(const char* ASourceCode, uint32_t* ALength) {
    char err_msg[1024] = {0};
    unsigned char* bytecode;
    int len;
    int err = wr_compile(ASourceCode,strlen(ASourceCode),&bytecode,&len,err_msg);
    SAT_Print("wr err: %i\n",err);
    if (err == 0) {
      SAT_Print("WRENCH COMPILE: bytecode %p len %i\n",bytecode,len);
      *ALength = len;
      return bytecode;
    }
    else {
      SAT_Print("WRENCH COMPILE ERROR: %s\n",err_msg);
      *ALength = 0;
      return nullptr;
    }
  }
  
  //----------
  
  void free_bytecode(uint8_t* AByteCode) {
    if (AByteCode) delete[] AByteCode;
  }

  //----------
  
  /*
    w:          state (see wr_newState)
    block:      location of bytecode
    blockSize:  number of bytes in block
    RETURNS:    a WRContext pointer to be passed to wr_callFunction
                'global' values are NOT SHARED between contexts
  */
  
  void run(uint8_t* AByteCode, uint32_t ALen) {
    if (AByteCode && (ALen > 0)) /*WRContext* ctx =*/ wr_run(MState,AByteCode,ALen);
    WRValue* ret = wr_returnValueFromLastCall(MState);
    char buffer[256] = {0};
    char* str = ret->asString(buffer,255);
    SAT_Print("WRENCH RUN returns %s\n",str);
  }
  
  //----------
  
  /*
    after wr_run() this allows any function in the script to be
    called with the given arguments, returning a single value
    contextId:    the context in which the function was loaded
    functionName: plaintext name of the function to be called
    argv:         array of WRValues to call the function with (optional)
    argn:         how many arguments argv contains (optional, but if
                  zero then argv will be ignored)
    RETURNS:      zero for no error or WRError code
  */
  
  int callFunction(WRContext* context, const char* functionName, const WRValue* argv =0, const int argn =0) {
    return wr_callFunction(MState,context,functionName,argv,argn);
  }

  //----------
  
  /*
    exactly the same as the above but the hash is supplied directly to
    save compute time, us wr_hashStr(...) to obtain the hash of the
    functionName
  */

  int callFunction(WRContext* context, const int32_t hash, const WRValue* argv =0, const int argn =0) {
    return wr_callFunction(MState,context,hash,argv,argn);
  }

  //----------
  
  /*
    The raw function pointer can be pre-loaded with wr_getFunction() and
    passed directly
    This method is exposed so functions can be called with an absolute
    minmum of overhead
  */

  int callFunction(WRContext* context, WRFunction* function, const WRValue* argv, const int argn) {
    return wr_callFunction(MState,context,function,argv,argn);
  }

  //----------
  
  /*
    After wr_run(...) or wr_callFunction(...) has run, there is always a
    return value (default 0) this command fetches it
  */

  WRValue* returnValueFromLastCall() {
    return wr_returnValueFromLastCall(MState);
  }

  //----------
  
  /*
    once wr_run() is called the returned context object can be used to
    pre-fetch a function pointer. This reduces the overhead of calling
    that function to almost nothing.
  */

  WRFunction* getFunction(WRContext* context, const char* functionName) {
    return wr_getFunction(context,functionName);
  }
  
  //----------
  
  /*
    WR_C_CALLBACK
    typedef void (*WR_C_CALLBACK)(WRState* w, const WRValue* argv, const int argn, WRValue& retVal, void* usr );
    
    register a function inside a state that can be called (by ALL contexts)
    callback will contain:
    w:             state it was called back from
    argv:          pointer to arguments function was called
                   with (may be null!)
    argn:          how many arguments it was called with
    retVal:        this value will be passed back, default: integer zero
    usr:           opaque pointer function was registered with
  
    IMPORTANT: The values passed may be references (keepin' it real) so
    always use the getters inside the WRValue class:
    int:    .asInt();
    float:  .asFloat();
    binary: .array( unsigned int* size, char* type );
    string: .c_str( unsigned int* len );
    tests:
    .isFloat() fast check if this is a float value
    .isInt() fast check if this is a float value
    this will do its pest to represent the value as a string to the
    supplied buffer, len is maximum size allowed
    string: .asString(char* string, size_t len )
    w:        state to register with (will be available to all contexts)
    name:     name of the function
    function: callback (see typdef above)
    usr:      opaque pointer that will be passed to the callback (optional)
  */

  void registerFunction(const char* AName, WR_C_CALLBACK ACallback) {
    wr_registerFunction(MState,AName,ACallback);
  }
  
  //----------
  
  /*
    WR_LIB_CALLBACK
    typedef void (*WR_LIB_CALLBACK)( WRValue* stackTop, const int argn, WRContext* context );
    
    Wrench allows libraries to be loaded in the form of <name>::<func>
    these calls are installed in such a way that they impose ZERO
    overhead if not used, zero memory footprint, text segment, etc.
    library calls do assume a deep knowledge of the internal workings of
    wrench so the callback is an absolute minimum of info:
    stackTop: current top of the stack, arguments have been pushed here,
              and any return value must be placed here, not pushed!
              since the code path is highly optimized it might be ignore
              by the calling code, see one of the many library examples
    argn:     how many arguments this function was called with. no
              guarantees are made about matching a function signature,
              library calls validate if they care.
  
    w:         state to register with (will be available to all contexts)
    signature: library signature must be in the form of <lib>::<name>
               ex: "math::cos"
    function:  callback (see typdef above)
  
  */

  void registerLibraryFunction(const char* ASignature, WR_LIB_CALLBACK AFunction) {
    wr_registerLibraryFunction(MState,ASignature,AFunction);
  }
  
  //----------
  
  /*
    you can write your own libraries (look at std.cpp for all the
    standard ones) but several have been provided:
  */
  
  void loadAllLibs() {
    wr_loadAllLibs(MState);
  }

  void loadMathLib() {
    wr_loadMathLib(MState);
  }

  void loadStdLib() {
    wr_loadStdLib(MState);
  }

  void loadFileLib() {
    wr_loadFileLib(MState);
  }

  void loadStringLib() {
    wr_loadStringLib(MState);
  }

  void loadMessageLib() {
    wr_loadMessageLib(MState);
  }
  
  //----------
  
  /*
    The WRValue is the basic data unit inside wrench, it can be a
    float/int/array whatever... think of it as a thneed. It's 8 bytes on
    a 32 bit system and you can create one on the stack no problemo, no
    heap will be harmed
  */
  
  void makeInt(WRValue* val, int i) {
    wr_makeInt(val,i);
  }
    
  //----------
  
  void makeFloat(WRValue* val, float f) {
    wr_makeFloat(val,f);
  }
  
  //----------
  
  /*
    a string has to exist in a context so it can be worked with, but the
    memory is managed by the caller, so wr_freeString() must be called
  */
  
  void makeString(WRContext* context, WRValue* val, const unsigned char* data, const int len) {
    wr_makeString(context,val,data,len);
  }
  
  //----------
  
  /*
    WARNING: If the memory is used/saved inside the context then
    deleting it here will cause a segfault. Be sure the script is done
    using it before calling this.
  */

  void freeString(WRValue* val) {
    wr_freeString(val);
  }
  
  //----------
  
  /*
    turning a value into a container allocates a hash table which must
    be released with destroy!
  */
  
  void makeContainer(WRValue* val, const uint16_t sizeHint =0) {
    wr_makeContainer(val,sizeHint);
  }

  void destroyContainer(WRValue* val) {
    wr_destroyContainer(val);
  }

  //----------
  
  void addValueToContainer(WRValue* container, const char* name, WRValue* value) {
    wr_addValueToContainer(container,name,value);
  }

  //----------
  
  void addIntToContainer(WRValue* container, const char* name, const int32_t value) {
    wr_addIntToContainer(container,name,value);
  }

  //----------
  
  void addFloatToContainer(WRValue* container, const char* name, const float value) {
    wr_addFloatToContainer(container,name,value);
  }

  //----------
  
  void addArrayToContainer(WRValue* container, const char* name, char* array, const uint32_t size) {
    wr_addArrayToContainer(container,name,array,size);
  }
  
  //----------
  
  /*
    after wrench executes it may have set an error code, this is how to
    retreive it. This sytems is coarse at the moment. Re-entering the
    interpreter clears the last error
  */
  
  WRError getLastError() {
    return wr_getLastError(MState);
  }
  
  //----------
  
  /*
    hashing function used inside wrench, it's a stripped down murmer,
    not academically fantastic but very good and very fast
  */
  
  uint32_t hash(const void* dat, const int len) {
    return wr_hash(dat,len);
  }
  
  //----------
  
  uint32_t hashStr( const char* dat) {
    return wr_hashStr(dat);
  }

};

//----------------------------------------------------------------------
#endif



