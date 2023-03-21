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

  #include <stdio.h>
  #include <sys/unistd.h>
  #include <cxxabi.h>
  #include <execinfo.h>
  
  struct sat_callstack_symbol_t {
    const char*  func;    // name of function containing address of function.
    const char*  file;    // file where symbol is defined, might not work on all platforms.
    unsigned int line;    // line in file where symbol is defined, might not work on all platforms.
    unsigned int ofs;     // offset from start of function where call was made.
  };

  struct sat_callstack_stringbuffer_t {
    char*       out_ptr;
    const char* end_ptr;
  };
  
#endif

//------------------------------
//
//------------------------------

#ifdef SAT_DEBUG_CRASH_HANDLER

  #include <signal.h>
  
  //bool SAT_InitCrashHandlers();
  void sat_crash_handler_callback(int sig);
  
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
      dump_memtrace();
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

    const char* callstack_alloc_string( sat_callstack_stringbuffer_t* buf, const char* str, size_t str_len ) {
      char* res;
      if ((size_t)(buf->end_ptr-buf->out_ptr)<(str_len+1)) return "out of memory";
      res = buf->out_ptr;
      buf->out_ptr += str_len + 1;
      memcpy(res,str,str_len);
      res[str_len] = '\0';
      return res;
    }

    //----------

    FILE* run_addr2line( void** addresses, int num_addresses, char* tmp_buffer, size_t tmp_buf_len ) {
      const char addr2line_run_string[] = "addr2line -e /proc/%u/exe";
      size_t start = (size_t)snprintf(tmp_buffer,tmp_buf_len,addr2line_run_string,getpid());
      for (int i=0; i<num_addresses; ++i) {
        start += (size_t)snprintf(tmp_buffer+start,tmp_buf_len-start," %p",addresses[i]);
      }
      return popen(tmp_buffer,"r");
    }

    //----------

    char* demangle_symbol( char* symbol, char* buffer, size_t buffer_size ) {
      int status;
      char* demangled_symbol = abi::__cxa_demangle( symbol, buffer, &buffer_size, &status );
      return status != 0 ? symbol : demangled_symbol;
    }

    //----------

    /*
      Generate a callstack from the current location in the code.
      skip_frames:    number of frames to skip in output to addresses.
      addresses:      is a pointer to a buffer where to store addresses in callstack.
      num_addresses:  size of addresses.
      return:         number of addresses in callstack.
    */

    int generate_callstack(int skip_frames, void** addresses, int num_addresses) {
      ++skip_frames;
      void* trace[256];
      int fetched = backtrace(trace,num_addresses+skip_frames)-skip_frames;
      memcpy(addresses,trace+skip_frames,(size_t)fetched*sizeof(void*));
      return fetched;
    }

    //----------

    /*
      Translate addresses from, for example, callstack to symbol-names.
      addresses:      list of pointers to translate.
      out_syms:       list of sat_callstack_symbol_t to fill with translated data, need to fit as many strings as there are ptrs in addresses.
      num_addresses:  number of addresses in addresses
      memory:         memory used to allocate strings stored in out_syms.
      mem_size:       size of addresses.
      return:         number of addresses translated.
    */

    int translate_addresses(void** addresses, sat_callstack_symbol_t* out_syms, int num_addresses, char* memory, int mem_size) {
      int num_translated = 0;
      sat_callstack_stringbuffer_t outbuf = { memory, memory + mem_size };
      memset(out_syms,0x0,(size_t)num_addresses*sizeof(sat_callstack_symbol_t));
      char** syms = backtrace_symbols(addresses,num_addresses);
      size_t tmp_buf_len = 1024 * 32;
      char*  tmp_buffer  = (char*)::malloc(tmp_buf_len);
      FILE* addr2line = run_addr2line(addresses,num_addresses,tmp_buffer,tmp_buf_len);
      for (int i=0; i<num_addresses; ++i) {
        char* symbol = syms[i];
        unsigned int offset = 0;
        // find function name and offset
        char* name_start = strchr(symbol,'(');
        char* offset_start = name_start ? strchr(name_start,'+') : 0x0;
        if (name_start && offset_start) {
          // zero terminate all strings
          ++name_start;
          *offset_start = '\0';
          ++offset_start;
        }
        if (name_start && offset_start ) {
          offset = (unsigned int)strtoll(offset_start,0x0,16);
          symbol = demangle_symbol(name_start,tmp_buffer,tmp_buf_len);
        }
        out_syms[i].func  = callstack_alloc_string(&outbuf,symbol,strlen(symbol));
        out_syms[i].ofs   = offset;
        out_syms[i].file  = "failed to lookup file";
        out_syms[i].line  = 0;
        if( addr2line != 0x0 ) {
          if ( fgets(tmp_buffer,(int)tmp_buf_len, addr2line) != 0x0) {
            char* line_start = strchr(tmp_buffer,':');
            *line_start = '\0';
            if (tmp_buffer[0] != '?' && tmp_buffer[1] != '?') {
              out_syms[i].file = callstack_alloc_string(&outbuf,tmp_buffer,strlen(tmp_buffer));
            }
            out_syms[i].line = (unsigned int)strtoll(line_start+1,0x0,10);
          }
        }
        ++num_translated;
      }
      ::free(syms);
      ::free(tmp_buffer);
      fclose(addr2line);
      return num_translated;
    }

    //----------

    //#define MIP_DumpCallStackSkip(s) {
    //  void* adr[256];
    //  int num = generate_callstack(0,adr,256);
    //  print_callstack(adr,num,2,s);
    //}

    /*
      two first entries (0,1) = crash handler itself (and glib?), #2 = positiof of crash..
      You can't call exit() safely from a signal handler. Use _exit() or _Exit()
    */
    
    void print_callstack(void** AAddresses=nullptr, int ANumAddresses=0, int skip_last=0, int skip_first=0) {
      sat_callstack_symbol_t symbols[256];
      char symbols_buffer[1024];
      int num_addresses;
      if (AAddresses && (ANumAddresses>0)) {
        num_addresses = translate_addresses(AAddresses,symbols,ANumAddresses,symbols_buffer,1024);
      }
      else {
        void* adr[256];
        int num = generate_callstack(0,adr,256);
        num_addresses = translate_addresses(adr,symbols,num,symbols_buffer,1024);
      }
      //SAT_DPrint("\n----------------------------------------------------------------------\n");
      print("\ncallstack:\n");
      //SAT_DPrint("----------------------------------------------------------------------\n");

      /*
        the last two entries are
        - __libc_start_main
        - mip_debug()
      */
      num_addresses -= skip_last;

      for (int i=skip_first; i<num_addresses; i++) {
        print("%3i : %s\n", i-skip_first, symbols[i].func);
        print("      %s\n", symbols[i].file);
        print("      line %i offset %i\n", symbols[i].line, symbols[i].ofs);
      }
      //MIP_DPrint("----------------------------------------------------------------------\n\n");
    }

  #endif // call stack
  
//------------------------------
public: // crash handler
//------------------------------

  #ifdef SAT_DEBUG_CRASH_HANDLER

    void initCrashHandler(int sig) {
      //signal(SIGSEGV,sat_crash_handler_callback);
      signal(sig,sat_crash_handler_callback);
    }

    //----------

    bool initCrashHandlers() {
      //for (int i=0; i<32; i++) SAT_InitSignalHandler(i);
      initCrashHandler(SIGILL);
      initCrashHandler(SIGABRT);
      initCrashHandler(SIGFPE);
      initCrashHandler(SIGSEGV);
      return true;
    }
    //----------
    
    void crashHandler(int sig) {
      print("\n");
      print("CRASH!\n");
      print("%i : %s\n",sig,MSignalNames[sig]);
      //SAT_GLOBAL_WATCHES.printWatches("watched:");

      //SAT_DumpCallStack;
      //SAT_DumpCallStackSkip(0); // 2
      print_callstack();

      print("\n");
      exit(1); //_exit(1);
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

    void dump_memtrace() {
      if (MMemTraceNodes.size() > 0) {
        print("Leaked memory:\n");
        for (uint32_t i=0; i<MMemTraceNodes.size(); i++) {
          char*     file  = MMemTraceNodes[i].file;
          uint32_t  line  = MMemTraceNodes[i].line;
          uint32_t  flag  = MMemTraceNodes[i].flag;
          void*     ptr   = MMemTraceNodes[i].ptr;
          uint32_t  size  = MMemTraceNodes[i].size;
          print("> %s, line %i (%s): ptr %p size %i\n",file,line,(flag==1)?"new":"malloc",ptr,size);
        }
      }
    }

  #endif // memtrace

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
