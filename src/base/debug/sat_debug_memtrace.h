#ifndef sat_debug_memtrace_included
#define sat_debug_memtrace_included
//----------------------------------------------------------------------

#ifdef SAT_DEBUG_MEMTRACE

  #include "sat.h"
  #include "base/types/sat_array.h"

  #include <stdarg.h>       // va_
  #include <sys/unistd.h>
  //#include <unistd.h>

  //----------------------------------------------------------------------

  struct SAT_MemTraceNode {
    char      file[SAT_MAX_NAME_LENGTH] = {0};
    uint32_t  line                      = 0;
    uint32_t  flag                      = 0;
    void*     ptr                       = nullptr;
    uint32_t  size                      = 0;
  };

  typedef SAT_Array<SAT_MemTraceNode> SAT_MemTraceNodes;

  //----------------------------------------------------------------------
  //
  //
  //
  //----------------------------------------------------------------------

  class SAT_MemTrace {
    
  //------------------------------
  private:
  //------------------------------

    SAT_DebugPrint*   MPrint          = nullptr;
    SAT_MemTraceNodes MMemTraceNodes  = {};

  //------------------------------
  public:
  //------------------------------


    SAT_MemTrace() {
      //init..
    }
    
    //----------

    ~SAT_MemTrace() {
      //cleanup..
    }
    
//------------------------------
public:
//------------------------------

  void initialize(SAT_DebugPrint* APrint) {
    MPrint = APrint;
  }

  //----------

  void cleanup() {
    //MPrint->print("memtrace cleanup\n");
    print_memtrace();
  }

  //------------------------------
  public: // memtrace
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
      bool found = false;
      for (uint32_t i=0; i<MMemTraceNodes.size(); i++) {
        if (MMemTraceNodes[i].ptr == ptr) {
          found = true;
          MMemTraceNodes[i].ptr = p;
          MMemTraceNodes[i].size = size;
        }
      }
      if (!found) {
        //SAT_DPrint("Error! allocation %p not found (%s/%i)\n",ptr,strip_path(file),line);
      }
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
              MPrint->print("Mismatch! malloc (%s/%i) delete (%s/%i)\n",MMemTraceNodes[i].file,MMemTraceNodes[i].line,strip_path(file),line);
            }
            else {
              MPrint->print("Mismatch! new (%s/%i) free (%s/%i)\n",MMemTraceNodes[i].file,MMemTraceNodes[i].line,strip_path(file),line);
            }

//            SAT_PrintCallStack();

          }
          // remove node
          MMemTraceNodes.remove(i);
        }
      }
      // not found
      // x11, nanovg prints a lot of these.. :-/
//      if (!found) {
//        SAT_DPrint("Error! allocation %p not found (%s/%i)\n",ptr,strip_path(file),line);
//      }
      // delete memory
      ::free(ptr);
    }

    //----------

    // print() doesn't work?

    void print_memtrace() {
      if (MMemTraceNodes.size() > 0) {
        MPrint->print("Leaked memory:\n");
        for (uint32_t i=0; i<MMemTraceNodes.size(); i++) {
          char*     file  = MMemTraceNodes[i].file;
          uint32_t  line  = MMemTraceNodes[i].line;
          uint32_t  flag  = MMemTraceNodes[i].flag;
          void*     ptr   = MMemTraceNodes[i].ptr;
          uint32_t  size  = MMemTraceNodes[i].size;
          MPrint->print("  %i. %s, line %i (%s): ptr %p size %i\n",i,file,line,(flag==1)?"new":"malloc",ptr,size);
        }
      }
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
    
  };

#else

  class SAT_MemTrace {
  public:
    void initialize(SAT_DebugPrint* APrint) {}
    void cleanup() {}
  };

#endif // SAT_DEBUG_MEMTRACE

//----------------------------------------------------------------------
#endif
