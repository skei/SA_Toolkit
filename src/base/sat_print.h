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


#ifdef SAT_PRINT_TIME
  #include <sys/time.h> // gettimeofday
#endif


//----------------------------------------------------------------------

class SAT_Print {

//------------------------------
private:
//------------------------------

  char MPrefixBuffer[SAT_MAX_STRING_LENGTH] = {0};
  char MPrintBuffer[SAT_MAX_STRING_LENGTH] = {0};

  #ifdef SAT_PRINT_SOCKET
    #ifdef SAT_LINUX
      int MSocketHandle = 0;
    #endif
    #ifdef SAT_WIN32
      SOCKET  MSocketHandle = 0;
      WSADATA MWsadata      = {};
    #endif
  #endif

  #ifdef SAT_PRINT_TIME
    double MStartTime = 0.0;
  #endif


//------------------------------
public:
//------------------------------

  SAT_Print() {
  }

  //----------

  ~SAT_Print() {
  }

//------------------------------
public:
//------------------------------

  bool initialize() {
    #ifdef SAT_PRINT_SOCKET
      setupSocket();
    #endif
    #ifdef SAT_PRINT_TIME
      timeStart();
    #endif
    //print("SAT_Print.initialize\n");
    return true;
  }

  //----------

  void cleanup() {
    #ifdef SAT_PRINT_SOCKET
      cleanupSocket();
    #endif
    #ifdef SAT_PRINT_TIME
    #endif
  }

//------------------------------
public:
//------------------------------

  void clearPrefix() {
    MPrefixBuffer[0] = 0;
  }

  //----------

  void setPrefix(const char* AFile, const char* AFunc, uint32_t ALine) {
    __SAT_UNUSED char buffer[SAT_MAX_STRING_LENGTH];
    //char buffer[SAT_MAX_STRING_LENGTH];
    strcpy(MPrefixBuffer,"[");

    strcat(MPrefixBuffer,SAT_TERM_FG_GREEN );
    strcat(MPrefixBuffer,stripPath(AFile));
    strcat(MPrefixBuffer,SAT_TERM_RESET );

    strcat(MPrefixBuffer,":");

    char line_str[16] = {0};
    sprintf(line_str, SAT_TERM_FG_YELLOW "%i", ALine);
    strcat(MPrefixBuffer,line_str);
    strcat(MPrefixBuffer,SAT_TERM_RESET );

    strcat(MPrefixBuffer,":");

    strcat(MPrefixBuffer,SAT_TERM_FG_RED );
    strcat(MPrefixBuffer,AFunc);
    strcat(MPrefixBuffer,SAT_TERM_RESET );

    strcat(MPrefixBuffer,":");

    #ifdef SAT_PRINT_TIME
      double time = timeElapsed();
      sprintf(buffer, SAT_TERM_FG_CYAN "%.6f" SAT_TERM_RESET ":",time);
      strcat(MPrefixBuffer,buffer);
    #endif

    #ifdef SAT_PRINT_THREAD
      #ifdef SAT_LINUX
        uint32_t thread_id = gettid();
      #endif
      #ifdef SAT_WIN32
        uint32_t thread_id = GetCurrentThreadId();
      #endif
      sprintf(buffer, SAT_TERM_FG_MAGENTA "%08x" SAT_TERM_RESET,thread_id);
      strcat(MPrefixBuffer,buffer);
    #endif

    strcat(MPrefixBuffer,"] ");    
  }

//------------------------------
public:
//------------------------------

  void print(const char* format, ...) {
    va_list args;
    va_start(args,format);
    vsprintf(MPrintBuffer,format,args);

    #ifdef SAT_PRINT_SOCKET
      print_socket("%s%s",MPrefixBuffer,MPrintBuffer);
    //#elif SAT_PRINT_FILE
    //  print_file("%s%s",MPrefixBuffer,MPrintBuffer);
    #else
      printf("%s%s",MPrefixBuffer,MPrintBuffer);
    #endif

    va_end(args); // move this up?
    clearPrefix();
  }

//------------------------------
private:
//------------------------------

  const char* stripPath(const char* buffer) {
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

  void setupSocket() {
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

  // //----------

  void cleanupSocket() {
    #ifdef SAT_LINUX
      close(MSocketHandle);
    #endif
    #ifdef SAT_WIN32
      closesocket(MSocketHandle);
      WSACleanup();
    #endif
  }

  // //----------

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

    void timeStart() {
      struct timeval time;
      gettimeofday(&time,NULL);
      MStartTime = (double)time.tv_sec + (double)time.tv_usec * .000001;
    }

    //----------

    double timeElapsed() {
      struct timeval time;
      gettimeofday(&time,NULL);
      double currenttime = (double)time.tv_sec + (double)time.tv_usec * .000001;
      double elapsed = currenttime - MStartTime;
      return elapsed;
    }

  #endif // print_time


};

//----------------------------------------------------------------------
#endif