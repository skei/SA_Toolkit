#ifndef sat_jit_included
#define sat_jit_included
//----------------------------------------------------------------------

// initially based on:
// https://github.com/mborgerson/jit

#include "base/sat.h"

//----------------------------------------------------------------------
//
// jit
//
//----------------------------------------------------------------------

//#define ENABLE_DEBUG_PRINT

//----------

#define SAT_JIT_MAX_REGS    4
#define SAT_JIT_MAX_LABELS  5

//----------

typedef void (*SAT_JitEntryPoint)(void);

//------------------------------
//
//------------------------------

// instruction opcodes

enum {
  SAT_JIT_END_OF_CODE,  // Marks the end of the bytecode
  SAT_JIT_OP_MARK,      // arg: addr
  SAT_JIT_OP_LABEL,     // arg: label-id        op: bb->labels[label-id] = pc
  SAT_JIT_OP_BRANCH,    // arg: type, label-id  op: if (cond(type)) goto bb->labels[label-id];
  SAT_JIT_OP_EXIT,      // arg: (none)          op: exit
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

struct SAT_JitOpDef {
  const char* name;
  uint32_t    args_num;
  const char* args_spec;
};
  
//----------

union SAT_JitOpcode {
  struct {
    uint8_t  op;
    uint8_t  args_map; // if bit X is set, that argument follows as 64b value
    uint16_t args[3];
    uint64_t ext_args[];
  };
  uint64_t raw;
};

//------------------------------
//
//------------------------------

/*
  i immediate
  l label
  r register
  c condition
*/

const SAT_JitOpDef op_defs[SAT_JIT_OP__MAX] = {
  { .name = /*"OP_*/"EOF",     .args_num = 0,  .args_spec = ""   },
  { .name = /*"OP_*/"MARK",    .args_num = 1,  .args_spec = "i"  },
  { .name = /*"OP_*/"LABEL",   .args_num = 1,  .args_spec = "l"  },
  { .name = /*"OP_*/"BRANCH",  .args_num = 2,  .args_spec = "cl" },
  { .name = /*"OP_*/"EXIT",    .args_num = 1,  .args_spec = "i"  },
  { .name = /*"OP_*/"CMP",     .args_num = 2,  .args_spec = "rr" },
  { .name = /*"OP_*/"LOADI",   .args_num = 2,  .args_spec = "ri" },
  { .name = /*"OP_*/"LOAD",    .args_num = 2,  .args_spec = "rr" },
  { .name = /*"OP_*/"STORE",   .args_num = 2,  .args_spec = "rr" },
  { .name = /*"OP_*/"ADD",     .args_num = 2,  .args_spec = "rr" },
  { .name = /*"OP_*/"SUB",     .args_num = 2,  .args_spec = "rr" }
};

//----------

const char* SAT_JitCondNames[SAT_JIT_BC__MAX] = {
  "BC_ALWAYS",
  "BC_EQUAL",
  "BC_NOTEQUAL",
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

  //__SAT_ALIGNED(SAT_ALIGNMENT_CACHE);
  void*             m_compiled_code       = nullptr;
  uint32_t          m_compiled_code_size  = 0;
  uint32_t          m_allocated_size      = 0;
  SAT_JitEntryPoint m_entrypoint          = nullptr;

//------------------------------
public:
//------------------------------

  SAT_Jit() {
  }
  
  //----------

  ~SAT_Jit() {
    if (m_compiled_code) free(m_compiled_code);
//  if (m_Sourcecode) free(m_Sourcecode);
//  if (m_Opcodes) delete[] m_Opcodes;
//  // delete words
//  // delete labels
  }

//------------------------------
public:
//------------------------------

  void* getCompiledCode() {
    return m_compiled_code;
  }
  
  uint32_t getCompiledCodeSize() {
    return m_compiled_code_size;
  }

//------------------------------
public: // interpreter
//------------------------------

  // Get total instruction length (in number of qwords)

  uint32_t getOpcodeLength(SAT_JitOpcode *opcode) {
    uint32_t len = 1;
    for (uint32_t i = 0; i < 8; i++) {
      if (opcode->args_map & (1 << i)) {
        len++;
      }
    }
    return len;
  }

  //----------

  // Get the value of an argument from encoded instruction
 
  uint64_t getOpcodeArg(SAT_JitOpcode *opcode, uint32_t arg) {
    assert(arg < 3);
    if (!(opcode->args_map & (1 << arg))) {
      // Operand is encoded with instruction
      return opcode->args[arg];
    }
    // Operand follows instruction, determine correct offset
    int ext = 0;
    for (size_t i = 0; i < arg; i++) {
      if (opcode->args_map & (1 << i)) {
        ext++;
      }
    }
    return opcode->ext_args[ext];
  }

  //----------

  // Pretty-print a single instruction.
 
  void printOpcode(SAT_JitOpcode *opcode) {
    const SAT_JitOpDef *op_def = &op_defs[opcode->op];
    if (opcode->op == SAT_JIT_OP_MARK) {
      SAT_DPrint("  >>> ");
    } else {
      SAT_DPrint("  %-9s", op_def->name/*+3*/);
    }
    for (uint32_t j = 0; j < op_def->args_num; j++) {
      uint64_t operand_value = getOpcodeArg(opcode, j);
      if (j > 0) {
        SAT_DPrint(", ");
      }
      // Print operand according to spec
      switch (op_def->args_spec[j]) {
      case 'i': SAT_DPrint("%#lx",      operand_value); break;
      case 'l': SAT_DPrint("LBL_%ld",   operand_value); break;
      case 'r': SAT_DPrint("$r%ld",     operand_value); break;
      case 'c': SAT_DPrint("%s",        SAT_JitCondNames[operand_value]); break;
      default:  SAT_DPrint("(%lx)?",    operand_value); break;
      }
    }
    SAT_DPrint("\n");
  }
  
  //----------

   void printOpcodes(SAT_JitOpcode* opcodes) {
    SAT_DPrint("> Code at %p\n", (void*)opcodes);
    for (uint32_t i = 0; true; ) {
      SAT_JitOpcode *opcode = &opcodes[i];
      if (opcode->op == SAT_JIT_END_OF_CODE) {
        break;
      }
      printOpcode(opcode);
      i += getOpcodeLength(opcode);
    }
  }

//------------------------------
public: // interpreter
//------------------------------

  void interpret(SAT_JitOpcode* opcodes) {
    uint64_t regs[SAT_JIT_MAX_REGS];
    uint64_t labels[SAT_JIT_MAX_LABELS];
    uint64_t cmp_values[2] = {0};
    bool run = true;
    SAT_DPrint("> Interpreting Code at %p\n", (void*)opcodes);
    memset(regs, 0, sizeof(regs));
    
    // Do a quick first pass to find labels
    for (uint32_t i = 0; true; ) {
      SAT_JitOpcode *opcode = &opcodes[i];
      if (opcode->op == SAT_JIT_END_OF_CODE) {
        break;
      }
      if (opcode->op == SAT_JIT_OP_LABEL) {
        labels[getOpcodeArg(opcode, 0)] = i;
      }
      i += getOpcodeLength(opcode);
    }
    
    for (uint32_t i = 0; run; ) {
      SAT_JitOpcode *opcode = &opcodes[i];
      bool set_pc = false;
      if (opcode->op == SAT_JIT_END_OF_CODE) {
        break;
      }
      
      #ifdef ENABLE_DEBUG_PRINT
        SAT_DPrint("%04zx: ", i);
        printOpcode(opcode);
      #endif
      
      switch (opcode->op) {
        case SAT_JIT_OP_MARK:
          break;
        case SAT_JIT_OP_LABEL:
          // Note: we already grabbed these in the first pass, so just ignore.
          break;
        case SAT_JIT_OP_BRANCH: {
          uint32_t target = labels[getOpcodeArg(opcode, 1)];
          switch (getOpcodeArg(opcode, 0)) {
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
          default:
            assert(0);
          }}
          break;
        case SAT_JIT_OP_EXIT:
          run = false;
          break;
        case SAT_JIT_OP_CMP:
          cmp_values[0] = regs[getOpcodeArg(opcode, 0)];
          cmp_values[1] = regs[getOpcodeArg(opcode, 1)];
          break;
        case SAT_JIT_OP_LOADI:
          regs[getOpcodeArg(opcode, 0)] = getOpcodeArg(opcode, 1);
          break;
        case SAT_JIT_OP_LOAD:
          regs[getOpcodeArg(opcode, 0)] = *(uint64_t*)regs[getOpcodeArg(opcode, 1)];
          break;
        case SAT_JIT_OP_STORE:
           *(uint64_t*)regs[getOpcodeArg(opcode, 0)] = regs[getOpcodeArg(opcode, 1)];
          break;
        case SAT_JIT_OP_ADD:
          regs[getOpcodeArg(opcode, 0)] += regs[getOpcodeArg(opcode, 1)];
          break;
        case SAT_JIT_OP_SUB:
          regs[getOpcodeArg(opcode, 0)] -= regs[getOpcodeArg(opcode, 1)];
          break;
        default:
          // Not supported yet!
          assert(0);
          break;
      } // switch
      
      if (!set_pc) {
        i += getOpcodeLength(opcode);
      }
      
      #ifdef ENABLE_DEBUG_PRINT
        for (uint32_t i = 0; i < SAT_JIT_MAX_REGS; i++) {
          SAT_DPrint("reg[%zd] = %lx\n", i, regs[i]);
        }
        SAT_DPrint("\n");
      #endif
      
    } // for
     
    SAT_DPrint("> Done\n");
  }

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

//------------------------------
public: // compiler
//------------------------------

  // Compile bytecode to x86-64
   
  bool compile(SAT_JitOpcode* opcodes) {
    SAT_DPrint("> Compiling Code at %p to x86-64\n", (void*)opcodes);

    // Allocate guest memory aligned on a page boundary
    long pagesize = sysconf(_SC_PAGE_SIZE);
    assert(pagesize != -1);

    m_allocated_size = pagesize;
    m_compiled_code = memalign(pagesize, 4096/*m_allocated_size*/);
    assert(m_compiled_code != NULL);
    
    uint32_t n = 0;
    uint8_t *b = (uint8_t *)m_compiled_code;

    int labels[SAT_JIT_MAX_LABELS];
    
    // Save values of gpregs
    n += x86_enc_push(&b[n], SAT_JIT_X86_REG_RAX);
    n += x86_enc_push(&b[n], SAT_JIT_X86_REG_RBX);
    n += x86_enc_push(&b[n], SAT_JIT_X86_REG_RCX);
    n += x86_enc_push(&b[n], SAT_JIT_X86_REG_RDX);
    
    for (uint32_t i = 0; true; ) {

      SAT_JitOpcode *opcode = &opcodes[i];
      if (opcode->op == SAT_JIT_END_OF_CODE) {
        break;
      }
      
      #ifdef ENABLE_DEBUG_PRINT
        printOpcode(opcode);
      #endif

      switch (opcode->op) {
        case SAT_JIT_OP_MARK:
          break;
        case SAT_JIT_OP_LABEL:
          // FIXME: Check for overflow
          labels[getOpcodeArg(opcode, 0)] = n;
          break;
        case SAT_JIT_OP_BRANCH: {
          // FIXME: Does not yet support forward jumps! Add fix-up table
          int target = labels[getOpcodeArg(opcode, 1)] - n;
          switch (getOpcodeArg(opcode, 0)) {
            case SAT_JIT_BC_ALWAYS:
              n += x86_enc_jmp(&b[n], SAT_JIT_X86_JMP, target);
              break;
            case SAT_JIT_BC_EQUAL:
              n += x86_enc_jmp(&b[n], SAT_JIT_X86_JZ, target);
              break;
            case SAT_JIT_BC_NOTEQUAL:
              n += x86_enc_jmp(&b[n], SAT_JIT_X86_JNZ, target);
              break;
            default:
              assert(0);
          }
          break;
        } // branch
        case SAT_JIT_OP_EXIT:
          // FIXME: Replace with jmp to end of code
          n += x86_enc_pop(&b[n], SAT_JIT_X86_REG_RDX);
          n += x86_enc_pop(&b[n], SAT_JIT_X86_REG_RCX);
          n += x86_enc_pop(&b[n], SAT_JIT_X86_REG_RBX);
          n += x86_enc_pop(&b[n], SAT_JIT_X86_REG_RAX);
          n += x86_enc_ret(&b[n]);
          break;
        case SAT_JIT_OP_CMP: {
          int reg1 = getOpcodeArg(opcode, 0);
          int reg2 = getOpcodeArg(opcode, 1);
          assert(reg1 < 4 && reg2 < 4); // Don't support other regs yet!
          n += x86_enc_alu(&b[n], SAT_JIT_X86_OP_CMP, SAT_JIT_X86_REG_RAX+reg1, SAT_JIT_X86_REG_RAX+reg2);
          break;        
        }
        case SAT_JIT_OP_LOADI: {
          int dest = getOpcodeArg(opcode, 0);
          uint64_t value = getOpcodeArg(opcode, 1);
          assert(dest < 4); // Don't support other regs yet!
          n += x86_enc_movri(&b[n], SAT_JIT_X86_REG_RAX+dest, value);
          break;
        }
        case SAT_JIT_OP_ADD: {
          int reg1 = getOpcodeArg(opcode, 0);
          int reg2 = getOpcodeArg(opcode, 1);
          assert(reg1 < 4 && reg2 < 4); // Don't support other regs yet!
          n += x86_enc_alu(&b[n], SAT_JIT_X86_OP_ADD, SAT_JIT_X86_REG_RAX+reg1, SAT_JIT_X86_REG_RAX+reg2);
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
        default:
          // Not supported yet!
          assert(0);
      } // switch
      
      i += getOpcodeLength(opcode);
      
    } // for
    
    SAT_DPrint("> Done\n");
    m_compiled_code_size = n;
    m_entrypoint = (SAT_JitEntryPoint)m_compiled_code;
    /*int status =*/ mprotect(b, m_allocated_size, PROT_EXEC | PROT_READ);
    assert(status != -1);
    return m_compiled_code;
  }
  
  //----------

  // Execute the compiled code
   
  void execute() {
    SAT_DPrint("> Executing x86-64 code at %p\n", (void*)m_compiled_code);
    m_entrypoint();
    SAT_DPrint("> Done\n");
  }

};

//----------------------------------------------------------------------
//
// JIT Test
//
//----------------------------------------------------------------------

int test_jit_magic_value = 0xCACACACA;

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

SAT_JitOpcode test_jit_opcodes[] = {
//        8                              8            16,16             64
  { .op = SAT_JIT_OP_LOADI,  .args_map = 0, .args = { 0, 1 }},
  { .op = SAT_JIT_OP_LOADI,  .args_map = 0, .args = { 1, 5 }},
  { .op = SAT_JIT_OP_LOADI,  .args_map = 2, .args = { 3    }}, { .raw = (uint64_t)&test_jit_magic_value },
  { .op = SAT_JIT_OP_LOAD,   .args_map = 0, .args = { 3, 3 }},
  { .op = SAT_JIT_OP_LABEL,  .args_map = 0, .args = { 0    }},
  { .op = SAT_JIT_OP_ADD,    .args_map = 0, .args = { 3, 0 }},
  { .op = SAT_JIT_OP_ADD,    .args_map = 0, .args = { 2, 0 }},
  { .op = SAT_JIT_OP_CMP,    .args_map = 0, .args = { 1, 2 }},
  { .op = SAT_JIT_OP_BRANCH, .args_map = 0, .args = { SAT_JIT_BC_NOTEQUAL, 0 }},
  { .op = SAT_JIT_OP_LOADI,  .args_map = 2, .args = { 0    }}, { .raw = (uint64_t)&test_jit_magic_value },
  { .op = SAT_JIT_OP_STORE,  .args_map = 0, .args = { 0, 3 }},
  { .op = SAT_JIT_OP_EXIT                                   },
  { .op = SAT_JIT_END_OF_CODE                               }
};

//----------

//SAT_JitBytecode test_jit_bytecode = {
//  .opcodes = test_jit_code,
//};
  
//----------------------------------------------------------------------

#define ENABLE_PPRINT
#define ENABLE_INTERPRETER
#define ENABLE_JIT
#define ENABLE_BYTECODE_DUMP
#define ENABLE_COMPILED_DUMP

//----------

SAT_Jit JIT = {};

void test_jit() {

  #ifdef ENABLE_PPRINT
    JIT.printOpcodes(test_jit_opcodes);
  #endif

  #ifdef ENABLE_BYTECODE_DUMP
    // count size
    uint32_t num = 0;
    for (num = 0; true; ) {
      SAT_JitOpcode* opcode = &test_jit_opcodes[num];
      num += JIT.getOpcodeLength(opcode);
      if (opcode->op == SAT_JIT_END_OF_CODE) {
        break;
      }
    }
    // write to file
    FILE* bf = fopen("bytecode.bin", "wb");
    fwrite(test_jit_opcodes, 1, num*8, bf);
    fclose(bf);
  #endif

  #ifdef ENABLE_INTERPRETER
    test_jit_magic_value = 0xDEADC0D9;
    SAT_DPrint("Magic value before is %x\n", test_jit_magic_value);
    JIT.interpret(test_jit_opcodes);
    SAT_DPrint("Magic value after is %x\n", test_jit_magic_value);
  #endif

  #ifdef ENABLE_JIT
    JIT.compile(test_jit_opcodes);
    void* code = JIT.getCompiledCode();
    uint32_t codesize = JIT.getCompiledCodeSize();
    #ifdef ENABLE_COMPILED_DUMP
      FILE* cf = fopen("executable.bin", "wb");
      fwrite(code, 1, codesize, cf);
      fclose(cf);
    #endif
    test_jit_magic_value = 0xDEADBEEA;
    SAT_DPrint("Magic value before is %x\n", test_jit_magic_value);
    JIT.execute();
    SAT_DPrint("Magic value after is %x\n", test_jit_magic_value);
  #endif
  
}

//----------

#undef ENABLE_PPRINT
#undef ENABLE_INTERPRETER
#undef ENABLE_JIT
#undef ENABLE_BYTECODE_DUMP
#undef ENABLE_COMPILED_DUMP

#undef ENABLE_DEBUG_PRINT

//----------------------------------------------------------------------
#endif