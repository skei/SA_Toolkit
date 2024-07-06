#ifndef sat_jit2_included
#define sat_jit2_included
//----------------------------------------------------------------------

#include "sat.h"
#include "base/util/sat_tokenizer.h"

//----------------------------------------------------------------------

typedef void (*SAT_Jit2EntryPoint)(void);

//----------

#define SAT_JIT2_NUM_OPCODES 4

enum {
  SAT_OP_NOP            = 0,
  SAT_OP_EXIT           = 1,
  SAT_OP_temp1          = 2,
  SAT_OP_temp2          = 3
};

const char* SAT_Jit2OpcodeNames[SAT_JIT2_NUM_OPCODES] = {
  "nop",
  "exit",
  "temp1",
  "temp2"
};

enum {
  SAT_JIT2_X86_REG_RAX = 0,
  SAT_JIT2_X86_REG_RCX = 1,
  SAT_JIT2_X86_REG_RDX = 2,
  SAT_JIT2_X86_REG_RBX = 3,
  SAT_JIT2_X86_REG_RSP = 4,
  SAT_JIT2_X86_REG_RBP = 5,
  SAT_JIT2_X86_REG_RSI = 6,
  SAT_JIT2_X86_REG_RDI = 7,
};

//  uint32_t MNumOpcodes = 2;
//
//  const char* MOpcodes[2] = {
//    "nop",
//    "exit"
//  };
  

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Jit2 {
  
//------------------------------
private:
//------------------------------

  int32_t             MPageSize             = 0;
  int32_t             MAllocatedAlign       = 0;
  uint32_t            MAllocatedSize        = 0;

  SAT_Jit2EntryPoint  MEntryPoint           = nullptr;

  bool                MSourceCodeAllocated  = false;
  char*               MSourceCode           = nullptr;
  void*               MCompiledCode         = nullptr;
  
  uint32_t            MSourceCodeSize       = 0;
  uint32_t            MCompiledCodeSize     = 0;
  
  //
  
  SAT_File            MSourceFile           = {};
  SAT_Tokenizer       MTokenizer            = {};
  
//------------------------------
public:
//------------------------------
  
  SAT_Jit2() {
  }
  
  //----------
  
  ~SAT_Jit2() {
    memory_free();
  }
  
//------------------------------
private:
//------------------------------

  /*
    The function posix_memalign() allocates size bytes and places the address of the allocated
    memory in *memptr. The address of the allocated memory will be a multiple of alignment, which
    must be a power of two and a multiple of sizeof(void *). If size is 0, then posix_memalign()
    returns either NULL, or a unique pointer value that can later be successfully passed to free    
    returns zero on success, or one of the error values listed in the next section on failure.
  */

  // Allocate guest memory aligned on a page boundary

  void* memory_alloc() {
    MPageSize = sysconf(_SC_PAGE_SIZE);
    MAllocatedAlign = MPageSize;
    MAllocatedSize  = MPageSize;
    int res = posix_memalign(&MCompiledCode,MAllocatedAlign,MAllocatedSize);
    if (res == 0) return MCompiledCode;
    else return nullptr;
  }
  
  //----------
  
  bool memory_free() {
    if (MCompiledCode) {
      free(MCompiledCode);
      return true;
    }
    return false;
  }

  //----------
  
  /*
    mprotect() changes the access protections for the calling
    process's memory pages containing any part of the address range
    in the interval [addr, addr+len-1].  addr must be aligned to a
    page boundary.
    If the calling process tries to access memory in a manner that
    violates the protections, then the kernel generates a SIGSEGV
    signal for the process.
    PROT_READ : The memory can be read.
    PROT_EXEC : The memory can be executed.      
    On success, mprotect() and pkey_mprotect() return zero.  On
    error, these system calls return -1, and errno is set to indicate
    the error.    
  */

  bool memory_protect() {
    int res = mprotect(MCompiledCode,MAllocatedSize,PROT_READ | PROT_EXEC);
    if (res == 0) return true;
    else return false;
  }
  
  //----------
  
  bool memory_unprotect() {
    int res = mprotect(MCompiledCode,MAllocatedSize,PROT_READ | PROT_WRITE);
    if (res == 0) return true;
    else return false;
  }

//------------------------------
private:
//------------------------------

//------------------------------
public: // x86-64 codegen
//------------------------------

  // add dest, src (etc..)
 
  uint32_t x86_enc_alu(uint8_t *enc, int op, int dest, int src) {
    enc[0] = 0x48;
    enc[1] = op;
    enc[2] = 0xC0 | (src & 3) << 3 | (dest & 3);
    return 3;
  }

  // push reg

  uint32_t x86_enc_push(uint8_t *enc, int reg) {
    enc[0] = 0x50 + reg;
    return 1;
  }

  // pop reg
   
  uint32_t x86_enc_pop(uint8_t *enc, int reg) {
    enc[0] = (0x58 + reg);
    return 1;
  }

  // Return
   
  uint32_t x86_enc_ret(uint8_t *enc) {
    enc[0] = 0xC3;
    return 1;
  }

  // mov reg, imm
   
  uint32_t x86_enc_movri(uint8_t *enc, int reg, uint64_t val) {
    enc[0] = 0x48;
    enc[1] = 0xB8 + reg;
    memcpy(&enc[2], &val, 8);
    return 10;
  }

  // mov [dest], src
   
  uint32_t x86_enc_movmr(uint8_t *enc, int dest, int src) {
    enc[0] = 0x48;
    enc[1] = 0x89;
    enc[2] = (src & 3) << 3 | (dest & 3);
    return 3;
  }

  // mov dest, [src]
   
  uint32_t x86_enc_movrm(uint8_t *enc, int dest, int src) {
    enc[0] = 0x48;
    enc[1] = 0x8B;
    enc[2] = (src & 3) << 3 | (dest & 3);
    return 3;
  }

  // Encode a jmp/jcc with 1 byte displacement
   
  uint32_t x86_enc_jmp(uint8_t *enc, int cc, int disp) {
    if (disp > 0) assert(disp < (125));
    else assert(disp > -126);
    enc[0] = cc;
    enc[1] = (-2 + disp);
    return 2;
  }
  
  //----------
  
  // Return
   
  uint32_t x86_enc_nop(uint8_t *enc) {
    enc[0] = 0x90;
    return 1;
  }
  
  //-----

  // 0 is token separator

  int32_t get_opcode(char* AOpcode) {
    for (uint32_t i=0; i<SAT_JIT2_NUM_OPCODES; i++) {
      if (strcmp(SAT_Jit2OpcodeNames[i],AOpcode)==0) return i;
    }
    return -1;
  }


  
//------------------------------
public:
//------------------------------

  bool load_src(const char* ASource, uint32_t ASize) {
    MSourceCodeSize = ASize;
    MSourceCode = (char*)ASource;
    SAT_Print("loading from memory: %p (%i)\n",ASource,ASize);
    return true;
  }
  
  //----------

  bool load_src(const char* APath) {
    if (MSourceFile.exists(APath)) {
      MSourceFile.open(APath,SAT_FILE_READ_TEXT);
      MSourceCodeSize = MSourceFile.length();
      MSourceCode = (char*)malloc(MSourceCodeSize);
      MSourceCodeAllocated = true;
      MSourceFile.read(MSourceCode,MSourceCodeSize);
      MSourceFile.close();
      return true;
    }
    return false;
  }
  
  //----------
  
  void unload_src() {
    if (MSourceCodeAllocated && MSourceCode) {
      free(MSourceCode);
      MSourceCode = nullptr;
      MSourceCodeSize = 0;
    }
  }

  //----------
  
  /*
    exit,noop
    stack
    arithmetic
    branching
    conditional
  */
  
  bool tokenize() {
    MTokenizer.tokenize(MSourceCode,MSourceCodeSize);
    MTokenizer.dumpTokens();
    return true;
  }
  
  //----------

  bool preprocess() {
    return false;
  }
  
  //----------

  bool compile() {
    
    memory_alloc();
    
    //MByteCode = (uint8_t*)malloc(65536);
    //MByteCodeSize = 0;
    uint8_t* code = (uint8_t*)MCompiledCode;
    uint32_t size = 0;
    // Save values of gpregs
    size += x86_enc_push(code, SAT_JIT2_X86_REG_RAX);  code += size;
    size += x86_enc_push(code, SAT_JIT2_X86_REG_RBX);  code += size;
    size += x86_enc_push(code, SAT_JIT2_X86_REG_RCX);  code += size;
    size += x86_enc_push(code, SAT_JIT2_X86_REG_RDX);  code += size;
    SAT_Print("encoding pushes.. (new size %i)\n",size);
    
    uint32_t num_tokens = MTokenizer.getNumTokens();
    SAT_Print("num_tokens %i\n",num_tokens);

    for (uint32_t i=0; i<num_tokens; i++) {
      char* opcode = MTokenizer.getToken(i);
      int32_t op = get_opcode(opcode);
      if (op >= 0) {
        SAT_Print("token %i opcode %s op %i\n",i,opcode,op);
        switch (op) {
          case SAT_OP_NOP: {
            size += x86_enc_nop(code);
            code += size;
            SAT_Print("SAT_OP_NOP (%i) (new size %i)\n",SAT_OP_NOP,size);
            break;
          }
          case SAT_OP_EXIT: {
            //size += x86_enc_ret(code);
            // FIXME: Replace with jmp to end of code
            size += x86_enc_pop(code, SAT_JIT2_X86_REG_RDX);  code += size;
            size += x86_enc_pop(code, SAT_JIT2_X86_REG_RCX);  code += size;
            size += x86_enc_pop(code, SAT_JIT2_X86_REG_RBX);  code += size;
            size += x86_enc_pop(code, SAT_JIT2_X86_REG_RAX);  code += size;
            size += x86_enc_ret(code);                        code += size;
            SAT_Print("SAT_OP_EXIT (%i) (new size %i)\n",SAT_OP_EXIT,size);
            break;
          }
        } // switch
      } // >0
    } // for
    MCompiledCodeSize = size;
    MEntryPoint = (SAT_Jit2EntryPoint)MCompiledCode;
    return true;
  }
  
  //----------

  bool execute() {
    memory_protect();
    MEntryPoint(); // crashes..
    memory_unprotect();
    return true;
  }
  
  //----------
  
};

//----------------------------------------------------------------------
#endif