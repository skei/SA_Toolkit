#ifndef sat_wrench_included
#define sat_wrench_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "extern/wrench/wrench.cpp"
//#include "extern/wrench/wrench.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

// test: wrench
// https://www.northarc.com/wrench/www/


void wr_print(WRState* w, const WRValue* argv, const int argn, WRValue& retVal, void* usr) {
  char buf[1024];
  for (int i=0; i<argn; ++i) {
    SAT_DPrint( "%s", argv[i].asString(buf,1024));
  }
}

const char* wr_source_code = 
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
  
  virtual ~SAT_Wrench() {
    wr_destroyState(MState);
  }
  
//------------------------------
public:
//------------------------------

  virtual void registerFunction(const char* AName, WR_C_CALLBACK ACallback) {
    wr_registerFunction(MState,AName,ACallback);
  }
  
  //----------
  
  virtual uint8_t* compile(const char* ASourceCode, uint32_t* ALength) {
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
      SAT_DPrint("WRENCH COMPILE ERROR: %s\n",err_msg);
      *ALength = 0;
      return nullptr;
    }
  }
  
  //----------
  
  virtual void free_bytecode(uint8_t* AByteCode) {
    if (AByteCode) delete[] AByteCode;
  }

  //----------
  
  virtual void run(uint8_t* AByteCode, uint32_t ALen) {
    if (AByteCode && (ALen > 0)) WRContext* ctx = wr_run(MState,AByteCode,ALen);
    WRValue* ret = wr_returnValueFromLastCall(MState);
    char buffer[256] = {0};
    char* str = ret->asString(buffer,255);
    SAT_Print("WRENCH RUN returns %s\n",str);
  }
  
};

//----------------------------------------------------------------------
#endif
