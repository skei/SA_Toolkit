#ifndef sat_print_included
#define sat_print_included
//----------------------------------------------------------------------

#ifdef SAT_PRINT_SOCKET
  #ifdef SAT_LINUX
    #include <sys/socket.h>
    #include <sys/un.h>
  #endif
  #ifdef SAT_WIN32
    #ifndef _WIN32_WINNT
      #define _WIN32_WINNT 0x0600
    #endif
    #include <winsock2.h>
    #include <ws2tcpip.h>
  #endif
#endif

//----------

#ifdef SAT_PRINT_THREAD
  #ifdef SAT_LINUX
    #include <sys/syscall.h>
    #include <sys/unistd.h>
    #define gettid() syscall(SYS_gettid)
  #endif
  #ifdef SAT_WIN32
    #include <processthreadsapi.h>
  #endif
#endif

//----------

#ifdef SAT_PRINT_TIME
  #include <sys/time.h> // gettimeofday
#endif

//----------

#include "sat.h"

//----------------------------------------------------------------------

class SAT_DebugPrint {

//------------------------------
private:
//------------------------------

  bool MIsInitialized = false;

  char MPrefixBuffer[256] = {0};
  char MPrintBuffer[256]  = {0};
  
  #ifdef SAT_PRINT_SOCKET
    #ifdef SAT_WIN32
      SOCKET  MSocketHandle = 0;
      WSADATA MWsadata      = {};
    #endif
    #ifdef SAT_LINUX
      int MSocketHandle = 0;
    #endif
  #endif

  #ifdef SAT_PRINT_TIME
    double MStartTime = 0.0;
  #endif

//------------------------------
public:
//------------------------------

  SAT_DebugPrint() {
  }

  //----------

  ~SAT_DebugPrint() {
  }

//------------------------------
public:
//------------------------------

  void initialize() {
    #ifdef SAT_PRINT_SOCKET
      setup_socket();
    #endif
    #ifdef SAT_PRINT_TIME
      time_start();
    #endif
  }

  //----------

  void cleanup() {
    #ifdef SAT_PRINT_SOCKET
      cleanup_socket();
    #endif
  }

  //----------
  
  // void print(const char* format, ...) {
  //   va_list args;
  //   va_start(args,format);
  //   vsprintf(MPrintBuffer,format,args);
  //   //print_socket("%s%s",MPrefixBuffer,MPrintBuffer);
  //   printf("%s%s",MPrefixBuffer,MPrintBuffer);
  //   //flush_stdout
  //   va_end(args); // move this up?
  //   //clear_prefix();
  // }

//------------------------------
public: // print
//------------------------------

  void print(const char* format, ...) {
    //printf("%s %s",MPrefixBuffer,..);
    va_list args;
    va_start(args,format);
    vsprintf(MPrintBuffer,format,args);

    #ifdef SAT_PRINT_SOCKET
      print_socket("%s%s",MPrefixBuffer,MPrintBuffer);
    #else
      printf("%s%s",MPrefixBuffer,MPrintBuffer);
    #endif

    //flush_stdout
    va_end(args); // move this up?
    clear_prefix();
  }

  //----------
  
  void set_prefix(const char* file, const char* func, uint32_t line) {
    __SAT_UNUSED char buffer[256];
    strcpy(MPrefixBuffer,"[");

    #ifdef SAT_PRINT_TIME
      double time = time_elapsed();
      sprintf(buffer, SAT_TERM_FG_RED "%.6f" SAT_TERM_RESET ":",time);
      strcat(MPrefixBuffer,buffer);
    #endif

    #ifdef SAT_PRINT_THREAD
      #ifdef SAT_LINUX
        uint32_t thread_id = gettid();
      #endif
      #ifdef SAT_WIN32
        uint32_t thread_id = GetCurrentThreadId();
      #endif
      sprintf(buffer, SAT_TERM_FG_YELLOW "%08x" SAT_TERM_RESET ":",thread_id);
      strcat(MPrefixBuffer,buffer);
    #endif

    strcat(MPrefixBuffer,SAT_TERM_FG_GREEN );
    strcat(MPrefixBuffer,strip_path(file));
    strcat(MPrefixBuffer,SAT_TERM_RESET );
    strcat(MPrefixBuffer,":");
    strcat(MPrefixBuffer,SAT_TERM_FG_MAGENTA );
    strcat(MPrefixBuffer,func);
    strcat(MPrefixBuffer,SAT_TERM_RESET );
    strcat(MPrefixBuffer,":");
    char line_str[16] = {0};
    //itoa(line,line_str,10);
    sprintf(line_str, SAT_TERM_FG_CYAN "%i", line);
    strcat(MPrefixBuffer,line_str);
    strcat(MPrefixBuffer,SAT_TERM_RESET );
    strcat(MPrefixBuffer,"] ");
  }

  //----------

  void clear_prefix() {
    MPrefixBuffer[0] = 0;
  }

//------------------------------
private:
//------------------------------

  const char* strip_path(const char* buffer) {
    const char *slash;
    slash = strrchr(buffer,'/');
    if (slash) {
      return slash + 1;
    }
    return buffer;
  }

//------------------------------
private: // socket
//------------------------------

  #ifdef SAT_PRINT_SOCKET

    void setup_socket() {
      #ifdef SAT_LINUX
        MSocketHandle = socket(PF_UNIX,SOCK_STREAM,0);
        sockaddr_un address;
        memset(&address,0,sizeof(sockaddr_un));
        address.sun_family = AF_UNIX;
        snprintf(address.sun_path,108,"/tmp/sat.socket"); // max 108?
        connect(MSocketHandle,reinterpret_cast<sockaddr*>(&address),sizeof(sockaddr_un));
      #endif
      #ifdef SAT_WIN32
        if (WSAStartup(MAKEWORD(2,2),&MWsadata)) {
          //fprintf(stderr, "Failed to initialize.\n");
        }
        MSocketHandle = socket(PF_UNIX,SOCK_STREAM,0);
        //sockaddr_un address;
        //memset(&address,0,sizeof(sockaddr_un));
        //address.sun_family = 0; // TODO!
        //snprintf(address.sun_path,108,"/tmp/sat.socket"); // max 108?
        //connect(MSocketHandle,reinterpret_cast<sockaddr*>(&address),sizeof(sockaddr_un));
      #endif
    }

    //----------

    void cleanup_socket() {
      #ifdef SAT_LINUX
        close(MSocketHandle);
      #endif
      #ifdef SAT_WIN32
        closesocket(MSocketHandle);
        WSACleanup();
      #endif
    }

    //----------

    //void print_socket() {
    void print_socket(const char* fmt, const char* pre, const char* str) {
      #ifdef SAT_LINUX
        //dprintf(MSocketHandle,"%s%s",MPrefixBuffer,MPrintBuffer);
        dprintf(MSocketHandle,fmt,pre,str);
      #else
        //TODO (print socket in windows)
        printf(fmt,pre,str);
      #endif
    }

  #endif

//------------------------------
private: // time
//------------------------------

  #ifdef SAT_PRINT_TIME

    void time_start() {
      struct timeval time;
      gettimeofday(&time,NULL);
      MStartTime = (double)time.tv_sec + (double)time.tv_usec * .000001;
    }

    //----------

    double time_elapsed() {
      struct timeval time;
      gettimeofday(&time,NULL);
      double currenttime = (double)time.tv_sec + (double)time.tv_usec * .000001;
      double elapsed = currenttime - MStartTime;
      return elapsed;
    }

  #endif // print_time

};

//----------------------------------------------------------------------

//#define SAT_Print SAT_GLOBAL.PRINT.print

void SAT_NoPrint(const char*,...) {}

#ifdef SAT_PRINT_PRETTY_FUNCTION

  #define SAT_Print \
    SAT_GLOBAL.PRINT.set_prefix( __FILE__, __PRETTY_FUNCTION__, __LINE__ ); \
    SAT_GLOBAL.PRINT.print

  #define SAT_DPrint \
    SAT_GLOBAL.PRINT.clear_prefix(); \
    SAT_GLOBAL.PRINT.print

  #define SAT_PRINT \
    SAT_GLOBAL.PRINT.set_prefix( __FILE__, __PRETTY_FUNCTION__, __LINE__ ); \
    SAT_GLOBAL.PRINT.print("\n")

#else // !pretty

  #define SAT_Print \
    SAT_GLOBAL.PRINT.set_prefix( __FILE__, __FUNCTION__, __LINE__ ); \
    SAT_GLOBAL.PRINT.print

  #define SAT_DPrint \
    SAT_GLOBAL.PRINT.clear_prefix(); \
    SAT_GLOBAL.PRINT.print

  #define SAT_PRINT \
    SAT_GLOBAL.PRINT.set_prefix( __FILE__, __FUNCTION__, __LINE__ ); \
    SAT_GLOBAL.PRINT.print("\n")
    
#endif // pretty
  
//----------

#ifndef SAT_DEBUG
#ifndef SAT_PRINT_ALWAYS

  #undef SAT_Print
  #undef SAT_DPrint
  #undef SAT_PRINT
  
  #define SAT_Print   SAT_NoPrint
  #define SAT_DPrint  SAT_NoPrint
  #define SAT_PRINT   {}
  
#endif // print always
#endif // debug

//----------------------------------------------------------------------
#endif
