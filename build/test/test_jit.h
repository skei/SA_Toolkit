
#include "base/sat.h"
#include "base/utils/sat_4th.h"
#include "plugin/sat_plugin.h"
#include "audio/sat_audio_utils.h"
#include "gui/sat_widgets.h"

//----------------------------------------------------------------------
//
// 4th
//
//----------------------------------------------------------------------


//----------------------------------------------------------------------
//
// jit
//
//----------------------------------------------------------------------

// https://github.com/mborgerson/jit

#define ENABLE_PPRINT           1
#define ENABLE_BYTECODE_DUMP    0
#define ENABLE_INTERPRETER      1
#define ENABLE_JIT              1
#define ENABLE_EXECUTABLE_DUMP  0
#define ENABLE_DEBUG_PRINT      1

//----------

#define SAT_JIT_MAX_REGS    4
#define SAT_JIT_MAX_LABELS  5

//------------------------------
//
//------------------------------

// All supported instruction opcodes

enum {
  SAT_JIT_END_OF_BLOCK, // Marks the end of the IR
  SAT_JIT_OP_MARK,      // arg: addr
  SAT_JIT_OP_LABEL,     // arg: label-id        op: bb->labels[label-id] = pc
  SAT_JIT_OP_BRANCH,    // arg: type, label-id  op: if (cond(type)) goto bb->labels[label-id];
  SAT_JIT_OP_EXIT,      // arg: (none)          op: exit current block
  SAT_JIT_OP_CMP,       // arg: r1, r2          op: compare(r1, r2), sets flags (not accessible directly)
  SAT_JIT_OP_LOADI,     // arg: r1, imm         op: reg[r1] = imm
  SAT_JIT_OP_LOAD,      // arg: r1, r2          op: reg[r1] = mem[reg[r2]]
  SAT_JIT_OP_STORE,     // arg: r1, r2          op: mem[reg[r1]] = reg[r2]
  SAT_JIT_OP_ADD,       // arg: r1, r2          op: reg[r1] = reg[r1] + reg[r2]
  SAT_JIT_OP_SUB,       // arg: r1, r2          op: reg[r1] = reg[r1] - reg[r2]
  SAT_JIT_OP__MAX
};

// Branch conditions
 
enum {
  SAT_JIT_BC_ALWAYS,
  SAT_JIT_BC_EQUAL,
  SAT_JIT_BC_NOTEQUAL,
  SAT_JIT_BC__MAX
};

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


//------------------------------
//
//------------------------------

typedef void (*SAT_JitEntryPoint)(void);

// Encoding of a single instruction, 64 bits
 
union SAT_JitInstruction {
  struct {
    uint8_t  op;
    uint8_t  args_map; // if bit X is set, that argument follows as 64b value
    uint16_t args[3];
    uint64_t ext_args[];
  };
  uint64_t raw;
};

// Block of instructions
 
struct SAT_JitCodeBlock {
  SAT_JitInstruction* code;
};

// Compiled block of instructions
 
struct SAT_JitCompiledBlock {
  SAT_JitCodeBlock* ir;
  size_t            code_len;
  size_t            alloc_len;
  void*             code;
  SAT_JitEntryPoint entry;
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Jit {

//------------------------------
private:
//------------------------------

  struct SAT_JitOpDef {
    const char* name;
    int         args_num;
    const char* args_spec;
  };
  
  // Instruction names and argument types for pretty-printing.
   
  const SAT_JitOpDef op_defs[SAT_JIT_OP__MAX] = {
    { .name = "OP_EOF",     .args_num = 0,  .args_spec = ""   },
    { .name = "OP_MARK",    .args_num = 1,  .args_spec = "i"  },
    { .name = "OP_LABEL",   .args_num = 1,  .args_spec = "l"  },
    { .name = "OP_BRANCH",  .args_num = 2,  .args_spec = "cl" },
    { .name = "OP_EXIT",    .args_num = 1,  .args_spec = "i"  },
    { .name = "OP_CMP",     .args_num = 2,  .args_spec = "rr" },
    { .name = "OP_LOADI",   .args_num = 2,  .args_spec = "ri" },
    { .name = "OP_LOAD",    .args_num = 2,  .args_spec = "rr" },
    { .name = "OP_STORE",   .args_num = 2,  .args_spec = "rr" },
    { .name = "OP_ADD",     .args_num = 2,  .args_spec = "rr" },
    { .name = "OP_SUB",     .args_num = 2,  .args_spec = "rr" }
  };

  // Branch condition names, used for pretty-printing
   
  const char* SAT_JitCondNames[SAT_JIT_BC__MAX] = {
    "BC_ALWAYS",
    "BC_EQUAL",
    "BC_NOTEQUAL",
  };

//------------------------------
public:
//------------------------------

  SAT_Jit() {
  }
  
  //----------

  ~SAT_Jit() {
  }

//------------------------------
public:
//------------------------------

  // Get total instruction length (in number of qwords)

  size_t getInstructionLength(SAT_JitInstruction *inst) {
    size_t len = 1;
    for (size_t i = 0; i < 8; i++) {
      if (inst->args_map & (1 << i)) {
        len++;
      }
    }
    return len;
  }

  //----------

  // Get the value of an argument from encoded instruction
 
  uint64_t getArg(SAT_JitInstruction *inst, size_t arg) {
    assert(arg < 3);
    if (!(inst->args_map & (1 << arg))) {
      // Operand is encoded with instruction
      return inst->args[arg];
    }
    // Operand follows instruction, determine correct offset
    int ext = 0;
    for (size_t/*int*/ i = 0; i < arg; i++) {
      if (inst->args_map & (1 << i)) {
        ext++;
      }
    }
    return inst->ext_args[ext];
  }

  //----------

  // Pretty-print a single instruction.
 
  void prettyPrintInst(SAT_JitInstruction *inst) {
    const SAT_JitOpDef *op_def = &op_defs[inst->op];
    if (inst->op == SAT_JIT_OP_MARK) {
      printf("  >>> ");
    } else {
      printf("  %-9s", op_def->name+3);
    }
    for (int/*size_t*/ j = 0; j < op_def->args_num; j++) {
      uint64_t operand_value = getArg(inst, j);
      if (j > 0) {
        printf(", ");
      }
      // Print operand according to spec
      switch (op_def->args_spec[j]) {
      case 'i': printf("%#lx",      operand_value); break;
      case 'l': printf("LBL_%ld",   operand_value); break;
      case 'r': printf("$r%ld",     operand_value); break;
      case 'c': printf("%s",        SAT_JitCondNames[operand_value]); break;
      default:  printf("(%lx)?",    operand_value); break;
      }
    }
    printf("\n");
  }
  
  //----------

  // Pretty-print the instructions of a block.

   void prettyPrint(SAT_JitCodeBlock *block) {
    printf("[*] IR Block at %p\n", (void*)block);
    for (size_t i = 0; true; ) {
      SAT_JitInstruction *inst = &block->code[i];
      if (inst->op == SAT_JIT_END_OF_BLOCK) {
        break;
      }
      prettyPrintInst(inst);
      i += getInstructionLength(inst);
    }
  }

//------------------------------
public: // interpreter
//------------------------------

  // Interpret and execute the instructions of a block
 
  void interpret(SAT_JitCodeBlock *block) {
    uint64_t regs[SAT_JIT_MAX_REGS];
    uint64_t labels[SAT_JIT_MAX_LABELS];
    uint64_t cmp_values[2];
    bool run = true;
    printf("[*] Interpreting IR Block at %p\n", (void*)block);
    memset(regs, 0, sizeof(regs));
    // Do a quick first pass to find labels
    for (size_t i = 0; true; ) {
      SAT_JitInstruction *inst = &block->code[i];
      if (inst->op == SAT_JIT_END_OF_BLOCK) {
        break;
      }
      if (inst->op == SAT_JIT_OP_LABEL) {
        labels[getArg(inst, 0)] = i;
      }
      i += getInstructionLength(inst);
    }
    for (size_t i = 0; run; ) {
      SAT_JitInstruction *inst = &block->code[i];
      bool set_pc = false;
      if (inst->op == SAT_JIT_END_OF_BLOCK) {
        break;
      }
      
      #ifdef ENABLE_DEBUG_PRINT
        // Pretty print the instruction
        printf("%04zx: ", i);
        prettyPrintInst(inst);
      #endif
      
      switch (inst->op) {
      case SAT_JIT_OP_MARK:
        break;
      case SAT_JIT_OP_LABEL:
        // Note: we already grabbed these in the first pass, so just ignore.
        break;
      case SAT_JIT_OP_BRANCH: {
        size_t target = labels[getArg(inst, 1)];
        switch (getArg(inst, 0)) {
        case SAT_JIT_BC_ALWAYS:
          i = target;
          set_pc = true;
          break;
        case SAT_JIT_BC_EQUAL:
          if (cmp_values[0] == cmp_values[1]) {
            i = target;
            set_pc = true;
          }
          break;
        case SAT_JIT_BC_NOTEQUAL:
          if (cmp_values[0] != cmp_values[1]) {
            i = target;
            set_pc = true;
          }
          break;
        default: assert(0);
        }}
        break;
      case SAT_JIT_OP_EXIT:
        run = false;
        break;
      case SAT_JIT_OP_CMP:
        cmp_values[0] = regs[getArg(inst, 0)];
        cmp_values[1] = regs[getArg(inst, 1)];
        break;
      case SAT_JIT_OP_LOADI:
        regs[getArg(inst, 0)] = getArg(inst, 1);
        break;
      case SAT_JIT_OP_LOAD:
        regs[getArg(inst, 0)] = *(uint64_t*)regs[getArg(inst, 1)];
        break;
      case SAT_JIT_OP_STORE:
         *(uint64_t*)regs[getArg(inst, 0)] = regs[getArg(inst, 1)];
        break;
      case SAT_JIT_OP_ADD:
        regs[getArg(inst, 0)] += regs[getArg(inst, 1)];
        break;
      case SAT_JIT_OP_SUB:
        regs[getArg(inst, 0)] -= regs[getArg(inst, 1)];
        break;
      default:
        // Not supported yet!
        assert(0);
        break;
      }
      if (!set_pc) {
        i += getInstructionLength(inst);
      }
      
      #ifdef ENABLE_DEBUG_PRINT
        // Print regs for debugging
        for (size_t i = 0; i < SAT_JIT_MAX_REGS; i++) {
          printf("reg[%zd] = %lx\n", i, regs[i]);
        }
        printf("\n");
      #endif
      
    }
    printf("[*] Done\n");
  }

//------------------------------
public: // x86-64 codegen
//------------------------------

  // add dest, src (etc..)
 
  size_t x86_enc_alu(uint8_t *enc, int op, int dest, int src) {
    enc[0] = 0x48;
    enc[1] = op;
    enc[2] = 0xC0 | (src & 3) << 3 | (dest & 3);
    return 3;
  }

  // push reg

  size_t x86_enc_push(uint8_t *enc, int reg) {
    enc[0] = 0x50 + reg;
    return 1;
  }

  // pop reg
   
  size_t x86_enc_pop(uint8_t *enc, int reg) {
    enc[0] = (0x58 + reg);
    return 1;
  }

  // Return
   
  size_t x86_enc_ret(uint8_t *enc) {
    enc[0] = 0xC3;
    return 1;
  }

  // mov reg, imm
   
  size_t x86_enc_movri(uint8_t *enc, int reg, uint64_t val) {
    enc[0] = 0x48;
    enc[1] = 0xB8 + reg;
    memcpy(&enc[2], &val, 8);
    return 10;
  }

  // mov [dest], src
   
  size_t x86_enc_movmr(uint8_t *enc, int dest, int src) {
    enc[0] = 0x48;
    enc[1] = 0x89;
    enc[2] = (src & 3) << 3 | (dest & 3);
    return 3;
  }

  // mov dest, [src]
   
  size_t x86_enc_movrm(uint8_t *enc, int dest, int src) {
    enc[0] = 0x48;
    enc[1] = 0x8B;
    enc[2] = (src & 3) << 3 | (dest & 3);
    return 3;
  }

  // Encode a jmp/jcc with 1 byte displacement
   
  size_t x86_enc_jmp(uint8_t *enc, int cc, int disp) {
    if (disp > 0) assert(disp < (125));
    else assert(disp > -126);
    enc[0] = cc;
    enc[1] = (-2 + disp);
    return 2;
  }

//------------------------------
public: // compiler
//------------------------------

  // Compile IR block to x86-64
   
  SAT_JitCompiledBlock* compile(SAT_JitCodeBlock *block) {
    printf("[*] Compiling IR Block at %p to native code\n", (void*)block);
    SAT_JitCompiledBlock *tb = (SAT_JitCompiledBlock *)malloc(sizeof(SAT_JitCompiledBlock));
    assert(tb != NULL);
    // Allocate guest memory aligned on a page boundary
    long pagesize = sysconf(_SC_PAGE_SIZE);
    assert(pagesize != -1);
    tb->alloc_len = pagesize;
    tb->code = memalign(pagesize, tb->alloc_len);
    assert(tb->code != NULL);
    uint8_t *b = (uint8_t *)tb->code;
    size_t n = 0;
    int labels[SAT_JIT_MAX_LABELS];
    // Save values of gpregs
    n += x86_enc_push(&b[n], SAT_JIT_X86_REG_RAX);
    n += x86_enc_push(&b[n], SAT_JIT_X86_REG_RBX);
    n += x86_enc_push(&b[n], SAT_JIT_X86_REG_RCX);
    n += x86_enc_push(&b[n], SAT_JIT_X86_REG_RDX);
    for (size_t i = 0; true; ) {
      SAT_JitInstruction *inst = &block->code[i];
      if (inst->op == SAT_JIT_END_OF_BLOCK) {
        break;
      }
      #ifdef ENABLE_DEBUG_PRINT
        // Pretty print the instruction
        prettyPrintInst(inst);
      #endif
      switch (inst->op) {
      case SAT_JIT_OP_MARK:
        break;
      case SAT_JIT_OP_LABEL:
        // FIXME: Check for overflow
        labels[getArg(inst, 0)] = n;
        break;
      case SAT_JIT_OP_BRANCH: {
        // FIXME: Does not yet support forward jumps! Add fix-up table
        int target = labels[getArg(inst, 1)] - n;
        switch (getArg(inst, 0)) {
        case SAT_JIT_BC_ALWAYS:
          n += x86_enc_jmp(&b[n], SAT_JIT_X86_JMP, target);
          break;
        case SAT_JIT_BC_EQUAL:
          n += x86_enc_jmp(&b[n], SAT_JIT_X86_JZ, target);
          break;
        case SAT_JIT_BC_NOTEQUAL:
          n += x86_enc_jmp(&b[n], SAT_JIT_X86_JNZ, target);
          break;
        default: assert(0);
        }}
        break;
      case SAT_JIT_OP_EXIT:
        // FIXME: Replace with jmp to end of block
        n += x86_enc_pop(&b[n], SAT_JIT_X86_REG_RDX);
        n += x86_enc_pop(&b[n], SAT_JIT_X86_REG_RCX);
        n += x86_enc_pop(&b[n], SAT_JIT_X86_REG_RBX);
        n += x86_enc_pop(&b[n], SAT_JIT_X86_REG_RAX);
        n += x86_enc_ret(&b[n]);
        break;
      case SAT_JIT_OP_CMP: {
        int reg1 = getArg(inst, 0);
        int reg2 = getArg(inst, 1);
        assert(reg1 < 4 && reg2 < 4); // Don't support other regs yet!
        n += x86_enc_alu(&b[n], SAT_JIT_X86_OP_CMP, SAT_JIT_X86_REG_RAX+reg1, SAT_JIT_X86_REG_RAX+reg2);
        } break;
      case SAT_JIT_OP_LOADI: {
        int dest = getArg(inst, 0);
        uint64_t value = getArg(inst, 1);
        assert(dest < 4); // Don't support other regs yet!
        n += x86_enc_movri(&b[n], SAT_JIT_X86_REG_RAX+dest, value);
        } break;
      case SAT_JIT_OP_ADD: {
        int reg1 = getArg(inst, 0);
        int reg2 = getArg(inst, 1);
        assert(reg1 < 4 && reg2 < 4); // Don't support other regs yet!
        n += x86_enc_alu(&b[n], SAT_JIT_X86_OP_ADD, SAT_JIT_X86_REG_RAX+reg1, SAT_JIT_X86_REG_RAX+reg2);
        } break;
      case SAT_JIT_OP_LOAD: {
        int dest = getArg(inst, 0);
        int src = getArg(inst, 1);
        assert(dest < 4 && src < 4); // Don't support other regs yet!
        n += x86_enc_movrm(&b[n], SAT_JIT_X86_REG_RAX+dest, SAT_JIT_X86_REG_RAX+src);
        } break;
      case SAT_JIT_OP_STORE: {
        int dest = getArg(inst, 0);
        int src = getArg(inst, 1);
        assert(dest < 4 && src < 4); // Don't support other regs yet!
        n += x86_enc_movmr(&b[n], SAT_JIT_X86_REG_RAX+dest, SAT_JIT_X86_REG_RAX+src);
        } break;
      default:
        // Not supported yet!
        assert(0);
      }
      i += getInstructionLength(inst);
    }
    printf("[*] Done\n");
    tb->code_len = n;
    tb->entry = (SAT_JitEntryPoint)tb->code;
    int status = mprotect(b, tb->alloc_len, PROT_EXEC | PROT_READ);
    assert(status != -1);
    return tb;
  }
  
  //----------

  // Execute the compiled block
   
  void executeBlock(SAT_JitCompiledBlock *tb) {
    printf("[*] Executing compiled block at %p\n", (void*)tb);
    tb->entry();
    printf("[*] Done\n");
  }

};

//------------------------------
//
// JIT Test
//
//------------------------------

int jit_magic_value = 0xCACACACA;

//----------

/*
  This little program will load one register with a value from memory, loop a
  number of times to increment that value, then write the value back into
  memory at the same spot.
  
  LOADI    $r0, 0x1             // reg[0] = 1
  LOADI    $r1, 0x5             // reg[1] = 5
  LOADI    $r3, 0x5594f0d1755c  // reg[3] = &magic_value
  LOAD     $r3, $r3             // reg[3] = mem[reg[3]]
  LABEL    LBL_0                // label0:
  ADD      $r3, $r0             // reg[3] = reg[3] + reg[0]
  ADD      $r2, $r0             // reg[2] = reg[2] + reg[0]
  CMP      $r1, $r2             // if (reg[1] != reg[2])
  BRANCH   BC_NOTEQUAL, LBL_0   // goto label0
  LOADI    $r0, 0x5594f0d1755c  // reg[0] = &magic_value
  STORE    $r0, $r3             // mem[reg[0]] = reg[3] 
  EXIT     0                    // exit
*/

SAT_JitInstruction test_jit_code[] = {
  { .op = SAT_JIT_OP_LOADI,  .args_map = 0, .args = { 0,1 }},
  { .op = SAT_JIT_OP_LOADI,  .args_map = 0, .args = { 1,5 }},
  { .op = SAT_JIT_OP_LOADI,  .args_map = 2, .args = { 3   }}, { .raw = (uint64_t)&jit_magic_value },
  { .op = SAT_JIT_OP_LOAD,   .args_map = 0, .args = { 3,3 }},
  { .op = SAT_JIT_OP_LABEL,  .args_map = 0, .args = { 0   }},
  { .op = SAT_JIT_OP_ADD,    .args_map = 0, .args = { 3,0 }},
  { .op = SAT_JIT_OP_ADD,    .args_map = 0, .args = { 2,0 }},
  { .op = SAT_JIT_OP_CMP,    .args_map = 0, .args = { 1,2 }},
  { .op = SAT_JIT_OP_BRANCH, .args_map = 0, .args = { SAT_JIT_BC_NOTEQUAL, 0 }},
  { .op = SAT_JIT_OP_LOADI,  .args_map = 2, .args = { 0   }}, { .raw = (uint64_t)&jit_magic_value },
  { .op = SAT_JIT_OP_STORE,  .args_map = 0, .args = { 0,3 }},
  { .op = SAT_JIT_OP_EXIT },
  { .op = SAT_JIT_END_OF_BLOCK }  
};

//----------

SAT_JitCodeBlock test_jit_block = {
  .code = test_jit_code,
};
  
//------------------------------
//
//------------------------------


//----------

void test_jit() {

  SAT_Jit JIT = {};

  #if ENABLE_PPRINT
    JIT.prettyPrint(&test_jit_block);
  #endif

  #if ENABLE_BYTECODE_DUMP
    size_t len = 0;
    for (len = 0; true; ) {
      SAT_JitInstruction *inst = &test_jit_block.code[len];
      len += JIT.getInstructionLength(inst);
      if (inst->op == SAT_JIT_END_OF_BLOCK) {
        break;
      }
    }
    FILE *irf = fopen("bytecode.bin", "wb");
    fwrite(test_jit_block.code, 1, len*8, irf);
    fclose(irf);
  #endif

  #if ENABLE_INTERPRETER
    jit_magic_value = 0xDEADC0D9;
    printf("Magic value before is %x\n", jit_magic_value);
    JIT.interpret(&test_jit_block);
    printf("Magic value after is %x\n", jit_magic_value);
  #endif

  #if ENABLE_JIT
    SAT_JitCompiledBlock *tb = JIT.compile(&test_jit_block);
    #if ENABLE_EXECUTABLE_DUMP
      FILE *tbf = fopen("executable.bin", "wb");
      fwrite(tb->code, 1, tb->code_len, tbf);
      fclose(tbf);
    #endif
    jit_magic_value = 0xDEADC0D9;
    printf("Magic value before is %x\n", jit_magic_value);
    JIT.executeBlock(tb);
    printf("Magic value after is %x\n", jit_magic_value);
  #endif
  
}

//----------

#undef ENABLE_PPRINT
#undef ENABLE_IR_DUMP
#undef ENABLE_INTERPRETER
#undef ENABLE_JIT
#undef ENABLE_TB_DUMP

//#endif // 0

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t myDescriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "me/myPlugin/0.0.1",
  .name         = "myPlugin",
  .vendor       = "skei.audio",
  .url          = "",
  .manual_url   = "",
  .support_url  = "",
  .version      = "0.0.1",
  .description  = "a nice plugin",
  .features     = (const char* []) {
                    CLAP_PLUGIN_FEATURE_AUDIO_EFFECT,
                    nullptr
                  }
};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class myPlugin
: public SAT_Plugin {
  
//------------------------------
private:
//------------------------------


//------------------------------
public:
//------------------------------

  SAT_PLUGIN_DEFAULT_CONSTRUCTOR(myPlugin)
  
//------------------------------
public:
//------------------------------

  static
  void tcc_error(void* opaque, const char* msg) {
    printf("[TCC:ERR] %s\n", msg);
  }

//------------------------------
public:
//------------------------------

  bool init() final {
    
    test_jit();
    
    registerDefaultExtensions();    
    appendStereoAudioInputPort();
    appendStereoAudioOutputPort();
    
    SAT_Parameter* param1 = new SAT_Parameter("Param1", .025);
    appendParameter(param1);
    
    SAT_Parameter* param2 = new SAT_Parameter("Param2", 0.75);
    appendParameter(param2);
    
    setInitialEditorSize(1024,768,1.333);
    
    return SAT_Plugin::init();
  }
  
  //----------

  bool initEditorWindow(SAT_Editor* AEditor/*, SAT_Window* AWindow*/) final {
    SAT_Window* window = AEditor->getWindow();
    SAT_PanelWidget* panel = new SAT_PanelWidget(0);
    window->appendRootWidget(panel);
    
    panel->setFillGradient(true);
    panel->setGradientColors(SAT_Green,0.2,0.2);
    
    return true;
  }

  //----------
  
  bool handleParamValue(const clap_event_param_value_t* event) override {
    SAT_Print("param_id %i value %f\n",event->param_id,event->value);
    return true;
  }
  
  //----------

  void processAudio(SAT_ProcessContext* AContext) final {
    float** inputs = AContext->process->audio_inputs[0].data32;
    float** outputs = AContext->process->audio_outputs[0].data32;
    uint32_t length = AContext->process->frames_count;
    SAT_CopyStereoBuffer(outputs,inputs,length);
    double scale = getParameterValue(0) + getModulationValue(0);
    scale = SAT_Clamp(scale,0,1);
    SAT_ScaleStereoBuffer(outputs,scale,length);
  }
  
};

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

#include "plugin/sat_entry.h"
SAT_PLUGIN_ENTRY(myDescriptor,myPlugin)
