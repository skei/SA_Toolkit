#ifndef sat_vm_compiler_included
#define sat_vm_compiler_included
//----------------------------------------------------------------------

/*
  TODO:
    - 2-pass, 2nd to resolve adresses to labels and words
    - macro, insert opcodes directly, instead of func call
    - do some of this in parsing...
    - jit (?)
    - comments: {} []

    label & -> @
    var   % -> #
*/

#include "base/types/sat_stack.h"
//#include "src/h_String.h"
#include "base/utils/sat_vm.h"

#define SAT_VM_COMPILER_MAX_OPCODES     65536
#define SAT_VM_COMPILER_MAX_CONDSTACK   32

//----------------------------------------------------------------------

struct SAT_VMSrcWord {
  char    name[16];
  int     pos;
};

struct SAT_VMSrcLabel {
  char    name[16];
  int     pos;
};

struct SAT_VMSrcVar {
  char    name[16];
  int     pos;
};

typedef SAT_Array<SAT_VMSrcWord>                      SAT_VMSrcWords;
typedef SAT_Array<SAT_VMSrcLabel>                     SAT_VMSrcLabels;
typedef SAT_Array<SAT_VMSrcVar>                       SAT_VMSrcVars;
typedef SAT_Stack<int,SAT_VM_COMPILER_MAX_CONDSTACK>  SAT_VMCondStack;
typedef SAT_Array<const char*>                        SAT_VMTokens;

//----------------------------------------------------------------------

//struct SAT_VMOpDef {
//  const char* name;
//  uint32_t    args_num;
//  const char* args_spec;
//};
//
///*
//  i immediate
//  l label
//  r register
//  c condition
//*/
//
//const SAT_VMOpDef SAT_VMOpcodeDefs[SAT_VM_NUM_OPCODES] = {
//  { .name = /*"OP_*/"EOF",     .args_num = 0,  .args_spec = ""   },
//  { .name = /*"OP_*/"MARK",    .args_num = 1,  .args_spec = "i"  },
//  { .name = /*"OP_*/"LABEL",   .args_num = 1,  .args_spec = "l"  },
//  { .name = /*"OP_*/"BRANCH",  .args_num = 2,  .args_spec = "cl" },
//  { .name = /*"OP_*/"EXIT",    .args_num = 1,  .args_spec = "i"  },
//  { .name = /*"OP_*/"CMP",     .args_num = 2,  .args_spec = "rr" },
//  { .name = /*"OP_*/"LOADI",   .args_num = 2,  .args_spec = "ri" },
//  { .name = /*"OP_*/"LOAD",    .args_num = 2,  .args_spec = "rr" },
//  { .name = /*"OP_*/"STORE",   .args_num = 2,  .args_spec = "rr" },
//  { .name = /*"OP_*/"ADD",     .args_num = 2,  .args_spec = "rr" },
//  { .name = /*"OP_*/"SUB",     .args_num = 2,  .args_spec = "rr" }
//};

const char* SAT_VMOpcodeNames[SAT_VM_NUM_OPCODES] = {
  "noop",   // 0
  "exit",   // 1
  "push",   // 2
  "drop",   // 3
  "dup",    // 4
  "dup2",   // 5
  "pushr",  // 6
  "popr",   // 7
  "pushd",  // 8
  "popd",   // 9
  "call",   // 10
  "ret",    // 11
  "jmp",    // 12
  "jz",     // 13
  "jnz",    // 14
  "eq",     // 15
  "gr",     // 16
  "le",     // 17
  "ne",     // 18
  "add",    // 19
  "sub",    // 20
  "mul",    // 21
  "div",    // 22
  "inc",    // 23
  "dec",    // 24
  "print",  // 25
  "ext"     // 26
};

//----------------------------------------------------------------------


//----------------------------------------------------------------------

// x86-64

enum {
  SAT_JIT_X86_REG_RAX = 0,
  SAT_JIT_X86_REG_RCX = 1,
  SAT_JIT_X86_REG_RDX = 2,
  SAT_JIT_X86_REG_RBX = 3,
  SAT_JIT_X86_REG_RSP = 4,
  SAT_JIT_X86_REG_RBP = 5,
  SAT_JIT_X86_REG_RSI = 6,
  SAT_JIT_X86_REG_RDI = 7,
};

enum {
  SAT_JIT_X86_OP_ADD = 0x01,
  SAT_JIT_X86_OP_SUB = 0x29,
  SAT_JIT_X86_OP_CMP = 0x39,
};

enum {
  SAT_JIT_X86_JMP = 0xEB,
  SAT_JIT_X86_JZ  = 0x74,
  SAT_JIT_X86_JNZ = 0x75,
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_VMCompiler {

//------------------------------
private:
//------------------------------

  char*           MSourcecode       = nullptr;
  int             MSourcecodeSize   = 0;
  SAT_VMTokens    MTokens           = {};
  
  SAT_VMOpcode*   MOpcodes          = nullptr;
  int             MNumOpcodes       = 0;

  SAT_VMSrcWords  MWords            = {};
  SAT_VMSrcLabels MLabels           = {};
  SAT_VMSrcVars   MVars             = {};
  SAT_VMCondStack MCondStack        = {};

  int             MPos              = 0;
  bool            MNewWord          = false;
  bool            MNewLabel         = false;
  bool            MNewVar           = false;
  
  // jit
  
  int32_t         MAllocatedSize    = 0;
  int32_t         MCompiledCodeSize = 0;
  void*           MCompiledCode     = nullptr;  
  void*           MEntrypoint       = nullptr;
  
  

//------------------------------
public:
//------------------------------

  SAT_VMCompiler() {
    MOpcodes = new SAT_VMOpcode[SAT_VM_NUM_OPCODES];
  }

  //----------

  ~SAT_VMCompiler() {
    if (MSourcecode) free(MSourcecode);
    if (MOpcodes) delete[] MOpcodes;
    // delete words
    // delete labels
  }

//------------------------------
//
//------------------------------

  #define WRITE_op(t,v) MOpcodes[MPos].type = t;                \
                        MOpcodes[MPos].value = (SAT_VMValue)v;  \
                        MPos++

  #define PUSH_cond     MCondStack.push(MPos)
  #define POP_cond      MCondStack.pop()


//------------------------------
public:
//------------------------------

  bool parse(char* a_Source, int a_Size) {
    MSourcecodeSize = a_Size;
    MSourcecode = (char*)malloc(a_Size + 1);
    memcpy(MSourcecode,a_Source,a_Size);
    MSourcecode[a_Size] = 0;
    int pos = 0;
    char prev = 0;
    while (pos < a_Size) {
      unsigned char ch = MSourcecode[pos];
      if (ch <= 32) {
        ch = 0;
        MSourcecode[pos] = 0;
      }
      else if (prev == 0) {
        const char* token = &MSourcecode[pos];
        MTokens.append(token);
      }
      prev = ch;
      pos++;
    }
    return true;
  }

  //----------

  //void preprocess() {
  //}

  //----------

  bool compile() {
    MPos = 0;
    MNewWord = false;
    uint32_t tok = 0;
    while (tok < MTokens.size()) {
      //char* token = NEXT_t;
      const char* token = MTokens[tok++];
      // try them all, until one reports 'true'
      if (!encode_newword(token))     // ()
      if (!encode_newlabel(token))    // &
      if (!encode_newvar(token))      // %
      if (!encode_conditional(token)) // ?:;
      if (!encode_word(token))
      if (!encode_label(token))
      if (!encode_var(token))
      if (!encode_internal(token))
      encode_number(token);
    } // while tok
    WRITE_op(SAT_VM_OP_TYPE_INTERNAL,SAT_OP_EXIT);
    MNumOpcodes = MPos;
    return true;
  }

  //----------

//  int getNumOpcodes() {
//    return MNumOpcodes;
//  }
//
//  //----------
//
//  SAT_VMOpcode* getOpcodes() {
//    return MOpcodes;
//  }
//
//  //----------
//
//  uint32_t getNumTokens() {
//    return MTokens.size();
//  }
//
//  const char* getToken(uint32_t AIndex) {
//    return MTokens[AIndex];
//  }
//
//  //----------
//
//  int findWordPos(char* token) {
//    for (uint32_t i=0; i<MWords.size(); i++) {
//      if (strcmp(MWords[i].name,token)==0) return MWords[i].pos;
//    }
//    return -1;
//  }
//
//  //----------
//
//  int findLabelPos(char* token) {
//    for (uint32_t i=0; i<MLabels.size(); i++) {
//      if (strcmp(MLabels[i].name,token)==0) return MLabels[i].pos;
//    }
//    return -1;
//  }
//
//  //----------
//
//  int findVarPos(char* token) {
//    for (uint32_t i=0; i<MVars.size(); i++) {
//      if (strcmp(MVars[i].name,token)==0) return MVars[i].pos;
//    }
//    return -1;
//  }

//------------------------------
private:
//------------------------------

  int _find_opcode(const char* token) {
    for (int i=0; i<SAT_VM_NUM_OPCODES; i++) {
      if (strcmp(SAT_VMOpcodeNames[i],token) == 0) return i;
    }
    return -1;
  }

  //----------

  int _find_word(const char* token) {
    for (uint32_t i=0; i<MWords.size(); i++) {
      if (strcmp(MWords[i].name,token) == 0) return i;
    }
    return -1;
  }

  //----------

  int _find_label(const char* token) {
    for (uint32_t i=0; i<MLabels.size(); i++) {
      if (strcmp(MLabels[i].name,token) == 0) return i;
    }
    return -1;
  }

  //----------

  int _find_var(const char* token) {
    for (uint32_t i=0; i<MVars.size(); i++) {
      if (strcmp(MVars[i].name,token) == 0) return i;
    }
    return -1;
  }

//------------------------------
private:
//------------------------------

  // ( name <code> )

  bool encode_newword(const char* token) {
    if ((strcmp(token,"(") == 0)) {
      MNewWord = true;
      return true;
    }
    else if (strcmp(token,")") == 0) {
      WRITE_op(SAT_VM_OP_TYPE_INTERNAL,SAT_OP_RET);
      MNewWord = false;
      return true;
    }
    else if (MNewWord) {
      SAT_VMSrcWord word;
      word.pos = MPos;
      strcpy(word.name,token);
      MWords.append(word);
      MNewWord = false;
      return true;
    }
    return false;
  }

  //----------

  // & name

  bool encode_newlabel(const char* token) {
    if ((strcmp(token,"&") == 0)) {
      MNewLabel = true;
      return true;
    }
    else if (MNewLabel) {
      SAT_VMSrcLabel label;
      label.pos = MPos;
      strcpy(label.name,token);
      MLabels.append(label);
      MNewLabel = false;
      return true;
    }
    return false;
  }

  //----------

  // % name

  bool encode_newvar(const char* token) {
    if ((strcmp(token,"%") == 0)) {
      MNewVar = true;
      return true;
    }
    else if (MNewVar) {
      SAT_VMSrcVar var;
      var.pos = MVars.size();
      strcpy(var.name,token);
      MVars.append(var);
      MNewVar = false;
      return true;
    }
    return false;
  }

  //----------

  // cond ? if-true : if-false ;

  bool encode_conditional(const char* token) {
    if ((strcmp(token,"?") == 0) ) { // if
      WRITE_op(SAT_VM_OP_TYPE_INTERNAL,SAT_OP_JZ);
      PUSH_cond;
      WRITE_op(SAT_VM_OP_TYPE_CODEPTR,0); // temp
      return true;
    }
    else if ((strcmp(token,":") == 0)) {// else
      int pos = POP_cond;
      WRITE_op(SAT_VM_OP_TYPE_INTERNAL,SAT_OP_JMP);
      PUSH_cond;
      WRITE_op(SAT_VM_OP_TYPE_CODEPTR,0); // temp
      MOpcodes[pos].value = (SAT_VMValue)MPos;
      return true;
    }
    else if ((strcmp(token,";") == 0)) {// endif
      int pos = POP_cond;
      MOpcodes[pos].value = (SAT_VMValue)MPos;
      return true;
    }
    return false;
  }

  //----------

  bool encode_word(const char* token) {
    int word = _find_word(token);
    if (word >= 0) {
      WRITE_op(SAT_VM_OP_TYPE_INTERNAL,SAT_OP_CALL);
      WRITE_op(SAT_VM_OP_TYPE_CODEPTR, MWords[word].pos);
      return true;
    }
    return false;
  }

  //----------

  bool encode_label(const char* token) {
    int label = _find_label(token);
    if (label >= 0) {
      //WRITE_op(SAT_VM_OP_TYPE_INTERNAL,SAT_OP_jmp);
      WRITE_op(SAT_VM_OP_TYPE_CODEPTR, MLabels[label].pos);
      return true;
    }
    return false;
  }

  //----------

  // <- name (push)
  // -> name (pop)

  bool encode_var(const char* token) {
    if ((strcmp(token,"->") == 0)) {
      WRITE_op(SAT_VM_OP_TYPE_INTERNAL,SAT_OP_POPD);
      return true;
    }
    else if ((strcmp(token,"<-") == 0)) {
      WRITE_op(SAT_VM_OP_TYPE_INTERNAL,SAT_OP_PUSHD);
      return true;
    }
    //else
    int var = _find_var(token);
    if (var >= 0) {
      WRITE_op(SAT_VM_OP_TYPE_DATAPTR, MVars[var].pos);
      return true;
    }
    return false;
  }

  //----------

  bool encode_internal(const char* token) {
    int internal = _find_opcode(token);
    if (internal >= 0) {
      WRITE_op(SAT_VM_OP_TYPE_INTERNAL,internal);
      return true;
    }
    return false;
  }

  //----------

  bool encode_number(const char* token) {
    if (strchr(token,'.') != nullptr) { // float
      float f = atof(token);
      WRITE_op(SAT_VM_OP_TYPE_INTERNAL,SAT_OP_PUSH);
      WRITE_op(SAT_VM_OP_TYPE_FLOAT, *(SAT_VMValue*)&f );   // warning: dereferencing type-punned pointer will break strict-aliasing rules...
      return true;                                          // TODO: the _may_alias 'trick ?
    }
    else { // int
      int i = atoi(token);
      WRITE_op(SAT_VM_OP_TYPE_INTERNAL,SAT_OP_PUSH);
      WRITE_op(SAT_VM_OP_TYPE_INT, i );
      return true;
    }
    return false;
  }

  //------------------------------

  #undef WRITE_op
  #undef PUSH_cond
  #undef POP_cond
  
//------------------------------
public: // jit
//------------------------------

  // --- a mess.. in progress.. ---

  // Get total instruction length (in number of qwords)

//  uint32_t getOpcodeLength(SAT_VMOpcode *opcode) {
//    uint32_t len = 1;
//    for (uint32_t i = 0; i < 8; i++) {
//      if (opcode->args_map & (1 << i)) {
//        len++;
//      }
//    }
//    return len;
//  }
//
//  //----------
//
//  // Get the value of an argument from encoded instruction
// 
//  uint64_t getOpcodeArg(SAT_VMOpcode *opcode, uint32_t arg) {
//    assert(arg < 3);
//    if (!(opcode->args_map & (1 << arg))) {
//      // Operand is encoded with instruction
//      return opcode->args[arg];
//    }
//    // Operand follows instruction, determine correct offset
//    int ext = 0;
//    for (size_t i = 0; i < arg; i++) {
//      if (opcode->args_map & (1 << i)) {
//        ext++;
//      }
//    }
//    return opcode->ext_args[ext];
//  }

  //----------
  
/*
        case SAT_JIT_OP_LOADI: {
          int dest = getOpcodeArg(opcode, 0);
          uint64_t value = getOpcodeArg(opcode, 1);
          assert(dest < 4); // Don't support other regs yet!
          n += x86_enc_movri(&b[n], SAT_JIT_X86_REG_RAX+dest, value);
          break;
        }
        case SAT_JIT_OP_LOAD: {
          int dest = getOpcodeArg(opcode, 0);
          int src = getOpcodeArg(opcode, 1);
          assert(dest < 4 && src < 4); // Don't support other regs yet!
          n += x86_enc_movrm(&b[n], SAT_JIT_X86_REG_RAX+dest, SAT_JIT_X86_REG_RAX+src);
          break;
        }
        case SAT_JIT_OP_STORE: {
          int dest = getOpcodeArg(opcode, 0);
          int src = getOpcodeArg(opcode, 1);
          assert(dest < 4 && src < 4); // Don't support other regs yet!
          n += x86_enc_movmr(&b[n], SAT_JIT_X86_REG_RAX+dest, SAT_JIT_X86_REG_RAX+src);
          break;
        }
  
*/

  bool jit_compile() {
    // Allocate guest memory aligned on a page boundary
    int32_t pagesize = sysconf(_SC_PAGE_SIZE);
    SAT_Assert(pagesize != -1);

    MAllocatedSize = pagesize;
    MCompiledCode = memalign(pagesize, MAllocatedSize );
    SAT_Assert(MCompiledCode);
    uint32_t pos = 0;
    uint8_t* buffer = (uint8_t *)MCompiledCode;
    
    // Save values of gpregs
    pos += x86_enc_push( &buffer[pos], SAT_JIT_X86_REG_RAX );
    pos += x86_enc_push( &buffer[pos], SAT_JIT_X86_REG_RBX );
    pos += x86_enc_push( &buffer[pos], SAT_JIT_X86_REG_RCX );
    pos += x86_enc_push( &buffer[pos], SAT_JIT_X86_REG_RDX );
    
    // pre, count (words, labels, etc?
    
    for (int32_t i=0; i<MNumOpcodes; i++) {
      
      SAT_VMOpcode* op = &MOpcodes[i];
      
      switch (op->type) {
        
        //case SAT_VM_OP_TYPE_NONE: {
        //  break;
        //}
        
        case SAT_VM_OP_TYPE_INTERNAL: {
          //SAT_VMOpcode* op_next = op + 1;
          //switch (op_next->type) {
          //  case SAT_VM_OP_TYPE_INT:
          //}
          break;
        }
        
        case SAT_VM_OP_TYPE_EXTERNAL: {
          break;
        }
        
        case SAT_VM_OP_TYPE_WORD: {
          break;
        }
        
        //case SAT_VM_OP_TYPE_INT: {
        //  break;
        //}
        
        //case SAT_VM_OP_TYPE_FLOAT: {
        //  break;
        //}
        
        //case SAT_VM_OP_TYPE_PTR: {
        //  break;
        //}
        
        //case SAT_VM_OP_TYPE_DATAPTR: {
        //  break;
        //}
        
        //case SAT_VM_OP_TYPE_CODEPTR: {
        //  break;
        //}
          
        
#if 0

        case SAT_OP_EXIT: {
          ////SAT_VMValue r = (intptr_t)VM_POP;
          //return;
          // FIXME: Replace with jmp to end of code
          pos += x86_enc_pop( &buffer[pos], SAT_JIT_X86_REG_RDX );
          pos += x86_enc_pop( &buffer[pos], SAT_JIT_X86_REG_RCX );
          pos += x86_enc_pop( &buffer[pos], SAT_JIT_X86_REG_RBX );
          pos += x86_enc_pop( &buffer[pos], SAT_JIT_X86_REG_RAX );
          pos += x86_enc_ret( &buffer[pos]);
          break;
        }
        
        case SAT_OP_PUSH: { // push const
          //VM_PUSH(VM_NEXT_cell);
          //VM_NEXT;
          break;
        }
        
        case SAT_OP_PUSHR: { // push register
          //VM_PUSH( MRegisters[VM_NEXT_cell] );
          //VM_NEXT;
          break;
        }

        case SAT_OP_PUSHD: { // push from data
          //SAT_VMValue *dp = (SAT_VMValue*)VM_NEXT_cell;
          //VM_PUSH(*dp);
          //VM_NEXT;
          break;
        }

        case SAT_OP_POPR: { // pop register
          //SAT_VMValue reg = (SAT_VMValue)VM_NEXT_cell;
          //MRegisters[reg] = (SAT_VMValue)VM_POP;
          //VM_NEXT;
          break;
        }

        case SAT_OP_POPD: { // pop to data
          break;
        }

        case SAT_OP_DROP: {
          break;
        }

        case SAT_OP_DUP: {
          break;
        }

        case SAT_OP_DUP2: {
          break;
        }

        case SAT_OP_CALL: {
          break;
        }

        case SAT_OP_RET: {
          break;
        }

        //          // FIXME: Does not yet support forward jumps! Add fix-up table
        //          int target = labels[getOpcodeArg(opcode, 1)] - n;
        //          switch (getOpcodeArg(opcode, 0)) {
        //            case SAT_JIT_BC_ALWAYS:
        //              n += x86_enc_jmp(&b[n], SAT_JIT_X86_JMP, target);
        //              break;
        //            case SAT_JIT_BC_EQUAL:
        //              n += x86_enc_jmp(&b[n], SAT_JIT_X86_JZ, target);
        //              break;
        //            case SAT_JIT_BC_NOTEQUAL:
        //              n += x86_enc_jmp(&b[n], SAT_JIT_X86_JNZ, target);
        //              break;
        //            default:
        //              assert(0);
        //          }

        case SAT_OP_JMP: {
          break;
        }

        case SAT_OP_JZ: {
          break;
        }

        case SAT_OP_JNZ: {
          break;
        }

        //        case SAT_JIT_OP_CMP: {
        //          int reg1 = getOpcodeArg(opcode, 0);
        //          int reg2 = getOpcodeArg(opcode, 1);
        //          assert(reg1 < 4 && reg2 < 4); // Don't support other regs yet!
        //          n += x86_enc_alu(&b[n], SAT_JIT_X86_OP_CMP, SAT_JIT_X86_REG_RAX+reg1, SAT_JIT_X86_REG_RAX+reg2);

        case SAT_OP_EQ: {
          break;
        }

        case SAT_OP_GR: {
          break;
        }

        case SAT_OP_LE: {
          break;
        }

        case SAT_OP_NE: {
          break;
        }

        case SAT_OP_ADD: {
          //int reg1 = getOpcodeArg(opcode, 0);
          //int reg2 = getOpcodeArg(opcode, 1);
          //SAT_Assert(reg1 < 4 && reg2 < 4);
          //n += x86_enc_alu(&b[n], SAT_JIT_X86_OP_ADD, SAT_JIT_X86_REG_RAX+reg1, SAT_JIT_X86_REG_RAX+reg2);
          break;
        }

        case SAT_OP_SUB: {
          break;
        }

        case SAT_OP_MUL: {
          break;
        }

        case SAT_OP_DIV: {
          break;
        }

        case SAT_OP_INC: {
          break;
        }

        case SAT_OP_DEC: {
          break;
        }

        case SAT_OP_PRINT: {
          break;
        }

        case SAT_OP_EXT: {
          break;
        }
        
#endif // 0

      } // switch
      
//      i += getOpcodeLength(op);

    } // for
    
    MCompiledCodeSize = pos;
    MEntrypoint = MCompiledCode;
    int32_t status = mprotect(buffer, MAllocatedSize, PROT_EXEC | PROT_READ);
    SAT_Assert(status != -1);
    return true;
    
  }
  
//------------------------------
private: // x86-64 codegen
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
  

};

//----------------------------------------------------------------------
#endif
