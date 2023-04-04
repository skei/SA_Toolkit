#ifndef sat_debug_included
#define sat_debug_included
//----------------------------------------------------------------------


// todo: SAT_DEBUG_PRINT_LOG

/*
  crash handler:
  compile with: -g -rdynamic
  // https://lasr.cs.ucla.edu/vahab/resources/signals.html
  // The only two signals for which a handler cannot be defined are SIGKILL and SIGSTOP.
*/

//----------------------------------------------------------------------

#ifndef SAT_DEBUG
  #undef SAT_DEBUG_ASSERT
  #undef SAT_DEBUG_CALL_STACK
  #undef SAT_DEBUG_CRASH_HANDLER
  #undef SAT_DEBUG_MEMTRACE
  #undef SAT_DEBUG_OBSERVER
  #undef SAT_DEBUG_PRINT_SOCKET
  #undef SAT_DEBUG_PRINT_THREAD
  #undef SAT_DEBUG_PRINT_TIME
#endif

//----------------------------------------------------------------------

#include <stdarg.h>   // va_
#include <sys/unistd.h>
//#include <unistd.h>

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifdef SAT_DEBUG_ASSERT

  #define SAT_Assert(x) {                                                               \
    if (!(x)) {                                                                         \
      SAT_Log("SAT_Assert( %s ) failed!\n",#x);                                         \
      SAT_Print("SAT_Assert( " SAT_TERM_FG_RED "%s" SAT_TERM_RESET " ) failed!\n",#x);  \
      /*SAT_Print("SAT_Assert(%s) failed!\n",#x);*/                                     \
      exit(1);                                                                          \
    }                                                                                   \
  }
  
#else

  #define SAT_Assert(x) {}
  
#endif

//------------------------------
//
//------------------------------

#ifdef SAT_DEBUG_CALL_STACK
#ifdef SAT_LINUX

  #include <execinfo.h>  // for backtrace
  #include <dlfcn.h>     // for dladdr
  #include <cxxabi.h>    // for __cxa_demangle
  
#endif // LINUX
#endif

//------------------------------
//
//------------------------------

#ifdef SAT_DEBUG_CRASH_HANDLER
#ifdef SAT_LINUX

  #include <signal.h>
  
  //bool SAT_InitCrashHandlers();
  void sat_crash_handler_callback(int sig);
  
#endif
#endif

//------------------------------
//
//------------------------------

#ifdef SAT_DEBUG_MEMTRACE

  struct SAT_MemTraceNode {
    char      file[SAT_MAX_NAME_LENGTH] = {0};
    uint32_t  line                      = 0;
    uint32_t  flag                      = 0;
    void*     ptr                       = nullptr;
    uint32_t  size                      = 0;
  };

  typedef SAT_Array<SAT_MemTraceNode> SAT_MemTraceNodes;

#endif


//------------------------------
//
//------------------------------

#ifdef SAT_DEBUG_OBSERVER

  struct SAT_Observable {
    uint32_t    type;
    void*       ptr;
    const char* desc;
  };

  typedef SAT_Array<SAT_Observable> SAT_Observables;

#endif


//------------------------------
//
//------------------------------

#ifdef SAT_DEBUG_PRINT_SOCKET

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

//------------------------------
//
//------------------------------

#ifdef SAT_DEBUG_PRINT_THREAD

  #ifdef SAT_LINUX
    //#include <sys/syscall.h>
    //#include <sys/unistd.h>
    //#define gettid() syscall(SYS_gettid)
  #endif
  
  #ifdef SAT_WIN32
    #include <processthreadsapi.h>
  #endif
  
#endif

//------------------------------
//
//------------------------------

#ifdef SAT_DEBUG_PRINT_TIME

  #include <sys/time.h> // gettimeofday
  
#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Debug {

//------------------------------
private:
//------------------------------

  char MPrefixBuffer[256] = {0};
  char MPrintBuffer[256]  = {0};

  //#ifdef SAT_DEBUG_AUDIO
    uint32_t MMaxParamFromHostToProcess   = 0;
    uint32_t MMaxParamFromProcessToGui    = 0;
    uint32_t MMaxParamFromGuiToProcess    = 0;
    uint32_t MMaxParamFromGuiToHost       = 0;
    uint32_t MMaxNoteEndFromProcessToHost = 0;
  //#endif

  //#ifdef SAT_DEBUG_CALL_STACK
  //#endif
  
  #ifdef SAT_DEBUG_CRASH_HANDLER
    const char* MSignalNames[32] = {
      "(kill(pid,0))",
      "SIGHUP (Hangup)",
      "SIGINT (Interactive attention signal)",
      "SIGQUIT (Quit)",
      "SIGILL (Illegal instruction)",
      "SIGTRAP (Trace/breakpoint trap)",
      "SIGABRT (Abnormal termination)",
      "(-)",
      "SIGFPE (Erroneous arithmetic operation)",
      "SIGKILL (Killed)",
      "SIGBUS (Bus error)",
      "SIGSEGV (Invalid access to storage)",
      "SIGSYS (Bad system call)",
      "SIGPIPE (Broken pipe)",
      "SIGALRM (Alarm clock)",
      "SIGTERM (Termination request)",
      "SIGURG (Urgent data is available at a socket)",
      "SIGSTOP (Stop, unblockable)",
      "SIGTSTP (Keyboard stop)",
      "SIGCONT (Continue)",
      "SIGCHLD (Child terminated or stopped)",
      "SIGTTIN (Background read from control terminal)",
      "SIGTTOU (Background write to control terminal)",
      "SIGPOLL (Pollable event occurred)",
      "SIGXCPU (CPU time limit exceeded)",
      "SIGXFSZ (File size limit exceeded)",
      "SIGVTALRM (Virtual timer expired)",
      "SIGPROF (Profiling timer expired)",
      "SIGWINCH (Window size change)",
      "(-)",
      "SIGUSR1 (User-defined signal 1)",
      "SIGUSR2 (User-defined signal 2)"
    };
  #endif

  #ifdef SAT_DEBUG_MEMTRACE
    SAT_MemTraceNodes MMemTraceNodes = {};
  #endif
  
  #ifdef SAT_DEBUG_OBSERVER
    //uint32_t        MNumObservables                         = 0;
    //SAT_Observable  MObservables[SAT_DEBUG_MAX_OBSERVABLES] = {0};
    SAT_Observables  MObservables = {};
  #endif

  #ifdef SAT_DEBUG_PRINT_SOCKET
    #ifdef SAT_WIN32
      //unsigned int  MSocketHandle = 0;
      SOCKET  MSocketHandle = 0;
      WSADATA MWsadata      = {};
    #endif
    #ifdef SAT_LINUX
      int MSocketHandle = 0;
    #endif
  #endif

  #ifdef SAT_DEBUG_PRINT_TIME
    double MStartTime = 0.0;
  #endif

//------------------------------
public:
//------------------------------

  SAT_Debug() {
  }

  //----------

  ~SAT_Debug() {
  }

//------------------------------
public:
//------------------------------

  void initialize() {
    #ifdef SAT_DEBUG_PRINT_SOCKET
      setup_socket();
    #endif
    #ifdef SAT_DEBUG_PRINT_TIME
      time_start();
    #endif
    #ifdef SAT_DEBUG_MEMTRACE
      setup_memtrace();
    #endif
    #ifdef SAT_DEBUG_CRASH_HANDLER
      initCrashHandlers();
    #endif
  }

  //----------

  void cleanup() {
    #ifdef SAT_DEBUG_MEMTRACE
      print_memtrace();
      cleanup_memtrace();
    #endif
    #ifdef SAT_DEBUG_PRINT_SOCKET
      cleanup_socket();
    #endif
  }

//------------------------------
public: // print
//------------------------------

  //#ifdef SAT_DEBUG_PRINT

    void set_prefix(const char* file, const char* func, uint32_t line) {
      __SAT_UNUSED char buffer[256];
      strcpy(MPrefixBuffer,"[");

      #ifdef SAT_DEBUG_PRINT_TIME
        double time = time_elapsed();
        sprintf(buffer, SAT_TERM_FG_RED "%.6f" SAT_TERM_RESET ":",time);
        strcat(MPrefixBuffer,buffer);
      #endif

      #ifdef SAT_DEBUG_PRINT_THREAD
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

    //----------

    void print(const char* format, ...) {
      //printf("%s %s",MPrefixBuffer,..);
      va_list args;
      va_start(args,format);
      vsprintf(MPrintBuffer,format,args);

      #ifdef SAT_DEBUG_PRINT_SOCKET
        print_socket();//MPrefixBuffer,MPrintBuffer);
      #else
        printf("%s%s",MPrefixBuffer,MPrintBuffer);
      #endif

      //flush_stdout
      va_end(args); // move this up?
      clear_prefix();
    }
  
  //#endif // print

//------------------------------
public: // audio
//------------------------------

  //#ifdef SAT_DEBUG_AUDIO

    void startAudioBlock() {
      MMaxParamFromHostToProcess   = 0;
      MMaxParamFromProcessToGui    = 0;
      MMaxParamFromGuiToProcess    = 0;
      MMaxParamFromGuiToHost       = 0;
      MMaxNoteEndFromProcessToHost = 0;
    }

    //----------

    void endAudioBlock() {
    }

    //----------

    void reportNumParamFromHostToProcess(uint32_t ANum) {
      if (ANum > MMaxParamFromHostToProcess) {
        MMaxParamFromHostToProcess = ANum;
      }
    }

    //----------

    void reportNumParamFromProcessToGui(uint32_t ANum) {
    }

    //----------

    void reportNumParamFromGuiToProcess(uint32_t ANum) {
    }

    //----------

    void reportNumParamFromGuiToHost(uint32_t ANum) {
    }

    //----------

    void reportNumNoteEndFromProcessToHost(uint32_t ANum) {
    }

    //----------

    void reportNumDirtyWidgets(uint32_t ANum) {
    }
    
  //#endif // audio

//------------------------------
public: // call stack
//------------------------------

  #ifdef SAT_DEBUG_CALL_STACK
  #ifdef SAT_LINUX
  
    // https://gist.github.com/fmela/591333/c64f4eb86037bb237862a8283df70cdfc25f01d3  

    void print_callstack(int skip = 1) {
      print("\nCallstack:\n");
      void *callstack[128];
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
          print("  %i: %s\n",i,txt);
          free(demangled);
        }
        else {
          //snprintf(buf, sizeof(buf), "%-3d %*0p\n",
          //  i, 2 + sizeof(void*) * 2, callstack[i]);
          print("  %i: %s\n",i,callstack[i]);
        }
        //snprintf(buf, sizeof(buf), "%s\n", symbols[i]);
        //SAT_Print("%s\n", symbols[i]);
      }
      free(symbols);
      if (nFrames == nMaxFrames) {
        print("  [truncated]\n");
      }
    }

  #endif // linux
  #endif // call stack
  
//------------------------------
public: // crash handler
//------------------------------

  #ifdef SAT_DEBUG_CRASH_HANDLER

    void initCrashHandler(int sig) {
      #ifdef SAT_LINUX
        //signal(SIGSEGV,sat_crash_handler_callback);
        signal(sig,sat_crash_handler_callback);
      #endif
    }

    //----------

    bool initCrashHandlers() {
      #ifdef SAT_LINUX
        //for (int i=0; i<32; i++) SAT_InitSignalHandler(i);
        initCrashHandler(SIGILL);
        initCrashHandler(SIGABRT);
        initCrashHandler(SIGFPE);
        initCrashHandler(SIGSEGV);
      #endif
      return true;
    }
    //----------
    
    void crashHandler(int sig) {
      #ifdef SAT_LINUX
        print("\nCRASH!\n");
        print("  %i : %s\n",sig,MSignalNames[sig]);
        //SAT_GLOBAL_WATCHES.printWatches("watched:");
        //SAT_DumpCallStack;
        //SAT_DumpCallStackSkip(0); // 2
        print_callstack();
        print_observers();
        print("\n");
        exit(1); //_exit(1);
      #endif
    }  

  #endif // crash handler

//------------------------------
public: // memtrace
//------------------------------

  #ifdef SAT_DEBUG_MEMTRACE

    void setup_memtrace() {
    }

    //----------

    void cleanup_memtrace() {
    }

    //----------

    void* malloc(unsigned int size, const char* file, const unsigned int line, const unsigned int flag = 0) {
      //print("malloc size %i file %s line %i\n",size,strip_path(file),line);
      void* p = ::malloc(size);
      SAT_MemTraceNode node;
      strcpy(node.file,strip_path(file));
      node.line = line;
      node.flag = flag;
      node.ptr  = p;
      node.size = size;
      MMemTraceNodes.append(node);
      return p;
    }

    //----------

    void* calloc(const unsigned int n, unsigned int size, const char* file, const unsigned int line) {
      //print("calloc\n");
      void* p = ::calloc(n,size);
      return p;
    }

    //----------

    void* realloc(void* ptr, const unsigned int size, const char* file, const unsigned int line) {
      //print("realloc\n");
      void* p = ::realloc(ptr,size);
      return p;
    }

    //----------

    // todo: error if not found

    void free(void* ptr, const char* file, const unsigned int line, const unsigned int flag = 0) {
      //print("free ptr %p file %s line %i\n",ptr,strip_path(file),line);
      bool found = false;
      // find node
      for (uint32_t i=0; i<MMemTraceNodes.size(); i++) {
        if (MMemTraceNodes[i].ptr == ptr) {
          found = true;
          // check for malloc/fre mismatch
          if (MMemTraceNodes[i].flag != flag) {
            if (MMemTraceNodes[i].flag == 0) {
              print("Mismatch! malloc (%s/%i) delete (%s/%i)\n",MMemTraceNodes[i].file,MMemTraceNodes[i].line,strip_path(file),line);
            }
            else {
              print("Mismatch! new (%s/%i) free (%s/%i)\n",MMemTraceNodes[i].file,MMemTraceNodes[i].line,strip_path(file),line);
            }
          }
          // remove node
          MMemTraceNodes.remove(i);
        }
      }
      // not found
      if (!found) {
//        print("Error! allocation %p not found (%s/%i)\n",ptr,strip_path(file),line);
      }
      // delete memory
      ::free(ptr);
    }

    //----------

    // print() doesn't work?

    void print_memtrace() {
      if (MMemTraceNodes.size() > 0) {
        print("Leaked memory:\n");
        for (uint32_t i=0; i<MMemTraceNodes.size(); i++) {
          char*     file  = MMemTraceNodes[i].file;
          uint32_t  line  = MMemTraceNodes[i].line;
          uint32_t  flag  = MMemTraceNodes[i].flag;
          void*     ptr   = MMemTraceNodes[i].ptr;
          uint32_t  size  = MMemTraceNodes[i].size;
          print("  %i. %s, line %i (%s): ptr %p size %i\n",i,file,line,(flag==1)?"new":"malloc",ptr,size);
        }
      }
    }

  #endif // memtrace

//------------------------------
public: // observer
//------------------------------

  #ifdef SAT_DEBUG_OBSERVER
  
    void observe(uint32_t AType, void* APtr, const char* ADesc) {
      //MObservables[MNumObservables].type = AType;
      //MObservables[MNumObservables].ptr  = APtr;
      //MObservables[MNumObservables].desc = ADesc;
      SAT_Observable obs;
      obs.type = AType;
      obs.ptr  = APtr;
      obs.desc = ADesc;
      MObservables.append(obs);
    }
    
    //----------
  
    void unobserve(void* APtr) {
      for (uint32_t i=0; i<MObservables.size(); i++) {
        if (MObservables[i].ptr == APtr) {
          MObservables.remove(i);
          return;
        }
      }
    }
    
    //----------
  
    void print_observers() {
      if (MObservables.size() > 0) {
        print("\nObserved:\n");
        for (uint32_t i=0; i<MObservables.size(); i++) {
          switch (MObservables[i].type) {
            
            case SAT_OBSERVE_DOUBLE: {
              print("  %i. %s = %.3f\n",i,MObservables[i].desc,*(double*)MObservables[i].ptr);
              break;
            }
            
          }
        }
        //print("\n");
      }
    }
  
  #endif

//------------------------------
public: // socket
//------------------------------

  #ifdef SAT_DEBUG_PRINT_SOCKET

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

    void print_socket() {
      #ifdef SAT_LINUX
        dprintf(MSocketHandle,"%s%s",MPrefixBuffer,MPrintBuffer);
      #else
        //TODO (print socket in windows)
        printf("%s%s",MPrefixBuffer, MPrintBuffer);
      #endif
    }

  #endif

//------------------------------
public: // time
//------------------------------

  #ifdef SAT_DEBUG_PRINT_TIME

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

  //----------


  //----------

};

//----------------------------------------------------------------------
#endif
