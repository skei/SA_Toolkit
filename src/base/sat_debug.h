#ifndef sat_debug_included
#define sat_debug_included
//----------------------------------------------------------------------

#include <stdarg.h>   // va_
#include <sys/unistd.h>
//#include <unistd.h>

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

#ifdef SAT_DEBUG_PRINT_TIME
  #include <sys/time.h> // gettimeofday
#endif

//----------------------------------------------------------------------
//
// assert
//
//----------------------------------------------------------------------

#define SAT_Assert(x) {                       \
  if (!(x)) {                                 \
    SAT_Log("SAT_Assert(%s) failed!\n",#x);   \
    SAT_Print("SAT_Assert(%s) failed!\n",#x); \
    exit(1);                                  \
  }                                           \
}

//----------------------------------------------------------------------
//
// memtrace
//
//----------------------------------------------------------------------

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

//----------------------------------------------------------------------
//
// debug class
//
//----------------------------------------------------------------------

class SAT_Debug {

//------------------------------
private:
//------------------------------

  char MPrefixBuffer[256] = {0};
  char MPrintBuffer[256]  = {0};

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

  #ifdef SAT_DEBUG_MEMTRACE
    SAT_MemTraceNodes MMemTraceNodes = {};
  #endif

  uint32_t MMaxParamFromHostToProcess   = 0;
  uint32_t MMaxParamFromProcessToGui    = 0;
  uint32_t MMaxParamFromGuiToProcess    = 0;
  uint32_t MMaxParamFromGuiToHost       = 0;
  uint32_t MMaxNoteEndFromProcessToHost = 0;

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
  }

  //----------

  void cleanup() {

    #ifdef SAT_DEBUG_MEMTRACE
      dump_memtrace();
    #endif

    #ifdef SAT_DEBUG_MEMTRACE
      cleanup_memtrace();
    #endif
    #ifdef SAT_DEBUG_PRINT_SOCKET
      cleanup_socket();
    #endif
  }

//------------------------------
public: // print
//------------------------------

  void set_prefix(const char* file, const char* func, uint32_t line) {
    __SAT_UNUSED char buffer[256];
    strcpy(MPrefixBuffer,"[");

    #ifdef SAT_DEBUG_PRINT_TIME
      double time = time_elapsed();
      sprintf(buffer,"%.6f:",time);
      strcat(MPrefixBuffer,buffer);
    #endif

    #ifdef SAT_DEBUG_PRINT_THREAD
      #ifdef SAT_LINUX
        uint32_t thread_id = gettid();
      #endif
      #ifdef SAT_WIN32
        uint32_t thread_id = GetCurrentThreadId();
      #endif
      sprintf(buffer,"%08x:",thread_id);
      strcat(MPrefixBuffer,buffer);
    #endif

    strcat(MPrefixBuffer,strip_path(file));
    strcat(MPrefixBuffer,":");
    strcat(MPrefixBuffer,func);
    strcat(MPrefixBuffer,":");
    char line_str[16] = {0};
    //itoa(line,line_str,10);
    sprintf(line_str,"%i",line);
    strcat(MPrefixBuffer,line_str);
    strcat(MPrefixBuffer,"]");
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
      printf("%s %s",MPrefixBuffer,MPrintBuffer);
    #endif

    //flush_stdout
    va_end(args); // move this up?
    clear_prefix();
  }

//------------------------------
public:
//------------------------------

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

//------------------------------
private: // socket
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
      dprintf(MSocketHandle,"%s %s",MPrefixBuffer,MPrintBuffer);
    #else
      //TODO (print socket in windows)
      printf("%s %s",MPrefixBuffer, MPrintBuffer);
    #endif
  }

  #endif

  #ifdef SAT_DEBUG_MEMTRACE

//------------------------------
private: // memtrace
//------------------------------

  void setup_memtrace() {
  }

  //----------

  void cleanup_memtrace() {
  }

//------------------------------
public:
//------------------------------

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
      print("Error! allocation %p not found (%s/%i)\n",ptr,strip_path(file),line);
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
private: // helpers
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

  //----------

};

//----------------------------------------------------------------------
#endif
