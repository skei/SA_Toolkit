#ifndef sat_wrench_included
#define sat_wrench_included
//----------------------------------------------------------------------

// https://www.northarc.com/wrench/www/

//WRState
//WRContext
//WRValue
//WRFunction

//----------------------------------------------------------------------

#include "base/sat.h"
#include "extern/wrench/wrench.cpp"
//#include "extern/wrench/wrench.h"

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

  SAT_Wrench() {
    MState = wr_newState();
    SAT_Assert(MState);
  }
  
  //----------
  
  ~SAT_Wrench() {
    wr_destroyState(MState);
  }
  
//------------------------------
public:
//------------------------------

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
  
  void run(uint8_t* AByteCode, uint32_t ALen) {
    if (AByteCode && (ALen > 0)) /*WRContext* ctx =*/ wr_run(MState,AByteCode,ALen);
    WRValue* ret = wr_returnValueFromLastCall(MState);
    char buffer[256] = {0};
    char* str = ret->asString(buffer,255);
    SAT_Print("WRENCH RUN returns %s\n",str);
  }
  
  //----------
  
  int callFunction(WRContext* context, const char* functionName, const WRValue* argv =0, const int argn =0) {
    return wr_callFunction(MState,context,functionName,argv,argn);
  }

  int callFunction(WRContext* context, const int32_t hash, const WRValue* argv =0, const int argn =0) {
    return wr_callFunction(MState,context,hash,argv,argn);
  }

  int callFunction(WRContext* context, WRFunction* function, const WRValue* argv, const int argn) {
    return wr_callFunction(MState,context,function,argv,argn);
  }

  WRValue* returnValueFromLastCall() {
    return wr_returnValueFromLastCall(MState);
  }

  WRFunction* getFunction(WRContext* context, const char* functionName) {
    return wr_getFunction(context,functionName);
  }
  
  //----------

  void registerFunction(const char* AName, WR_C_CALLBACK ACallback) {
    wr_registerFunction(MState,AName,ACallback);
  }
  
  //----------

  void registerLibraryFunction(const char* ASignature, WR_LIB_CALLBACK AFunction) {
    wr_registerLibraryFunction(MState,ASignature,AFunction);
  }
  
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
  
  //
  
  void makeInt(WRValue* val, int i) {
    wr_makeInt(val,i);
  }
    
  void makeFloat(WRValue* val, float f) {
    wr_makeFloat(val,f);
  }
  
  void makeString(WRContext* context, WRValue* val, const unsigned char* data, const int len) {
    wr_makeString(context,val,data,len);
  }
  
  void freeString(WRValue* val) {
    wr_freeString(val);
  }
  
  void makeContainer(WRValue* val, const uint16_t sizeHint =0) {
    wr_makeContainer(val,sizeHint);
  }

  void destroyContainer(WRValue* val) {
    wr_destroyContainer(val);
  }

  void addValueToContainer(WRValue* container, const char* name, WRValue* value) {
    wr_addValueToContainer(container,name,value);
  }

  void addIntToContainer(WRValue* container, const char* name, const int32_t value) {
    wr_addIntToContainer(container,name,value);
  }

  void addFloatToContainer(WRValue* container, const char* name, const float value) {
    wr_addFloatToContainer(container,name,value);
  }

  void addArrayToContainer(WRValue* container, const char* name, char* array, const uint32_t size) {
    wr_addArrayToContainer(container,name,array,size);
  }
  
  WRError getLastError() {
    return wr_getLastError(MState);
  }
  
  uint32_t hash(const void* dat, const int len) {
    return wr_hash(dat,len);
  }
  
  uint32_t hashStr( const char* dat) {
    return wr_hashStr(dat);
  }

};

//----------------------------------------------------------------------
#endif



