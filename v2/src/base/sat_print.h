#ifndef sat_print_included
#define sat_print_included
//----------------------------------------------------------------------

class SAT_Print {

//------------------------------
private:
//------------------------------

  char MPrefixBuffer[256] = {0};
  char MPrintBuffer[256]  = {0};

//------------------------------
public:
//------------------------------

  void initialize() {
  }

  //----------

  void cleanup() {
  }

//------------------------------
public:
//------------------------------

  void print(const char* format, ...) {
    va_list args;
    va_start(args,format);
    vsprintf(MPrintBuffer,format,args);
    //print_socket("%s%s",MPrefixBuffer,MPrintBuffer);
    printf("%s%s",MPrefixBuffer,MPrintBuffer);
    //flush_stdout
    va_end(args); // move this up?
    //clear_prefix();
  }

};

//----------------------------------------------------------------------

//#define SAT_Print SAT_GLOBAL.PRINT.print

//----------------------------------------------------------------------
#endif