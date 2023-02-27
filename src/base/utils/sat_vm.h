#ifndef sat_gcc_vm_included
#define sat_gcc_vm_included
//----------------------------------------------------------------------

/*
  uses calculated gotos ('goto **ip++')..
  .sat_script // text -> parse -> bytecode
  .sat_bytecode

*/

//----------

#include <stdio.h>  // printf
#include "base/types/sat_stack.h"

//----------

#define SAT_VM_CALL_STACK_SIZE   1024
#define SAT_VM_DATA_STACK_SIZE   1024
#define SAT_VM_CODE_SEGMENT_SIZE 65536
#define SAT_VM_DATA_SEGMENT_SIZE 65536
#define SAT_VM_MAX_EXTERNALS     256
#define SAT_VM_NUM_REGISTERS     256

#define SAT_VM_OP_TYPE_NONE      0
#define SAT_VM_OP_TYPE_INTERNAL  1
#define SAT_VM_OP_TYPE_EXTERNAL  2
#define SAT_VM_OP_TYPE_WORD      3
#define SAT_VM_OP_TYPE_INT       4
#define SAT_VM_OP_TYPE_FLOAT     5
#define SAT_VM_OP_TYPE_PTR       6
#define SAT_VM_OP_TYPE_DATAPTR   7
#define SAT_VM_OP_TYPE_CODEPTR   8

enum SAT_EVMOpcodes {
  SAT_OP_NOOP = 0,  // "noop",  // 0
  SAT_OP_EXIT,      // "exit",   // 1
  SAT_OP_PUSH,      // "push",   // 2
  SAT_OP_DROP,      // "drop",   // 3
  SAT_OP_DUP,       // "dup",    // 4
  SAT_OP_DUP2,      // "dup2",   // 5
  SAT_OP_PUSHR,     // "pushr",  // 6
  SAT_OP_POPR,      // "popr",   // 7
  SAT_OP_PUSHD,     // "pushd",  // 8
  SAT_OP_POPD,      // "popd",   // 9
  SAT_OP_CALL,      // "call",   // 10
  SAT_OP_RET,       // "ret",    // 11
  SAT_OP_JMP,       // "jmp",    // 12
  SAT_OP_JZ,        // "jz",     // 13
  SAT_OP_JNZ,       // "jnz",    // 14
  SAT_OP_EQ,        // "eq",     // 15
  SAT_OP_GR,        // "gr",     // 16
  SAT_OP_LE,        // "le",     // 17
  SAT_OP_NE,        // "ne",     // 18
  SAT_OP_ADD,       // "add",    // 19
  SAT_OP_SUB,       // "sub",    // 20
  SAT_OP_MUL,       // "mul",    // 21
  SAT_OP_DIV,       // "div",    // 22
  SAT_OP_INC,       // "inc",    // 23
  SAT_OP_DEC,       // "dec",    // 24
  SAT_OP_PRINT,     // "print",  // 25
  SAT_OP_EXT,       // "ext"     // 26

  SAT_VM_NUM_OPCODES

};

//----------------------------------------------------------------------

//typedef union {
//  double    f;
//  uint64_t  i;
//  void*     p;
//} SAT_VMValue;

typedef intptr_t SAT_VMValue;

struct SAT_VMOpcode {
  int         type;
  SAT_VMValue value;
};


class SAT_VirtualMachine;
typedef void (*SAT_VMExternal)(SAT_VirtualMachine* vm);

typedef SAT_Stack<SAT_VMValue, SAT_VM_DATA_STACK_SIZE> SAT_VMDataStack;
typedef SAT_Stack<SAT_VMValue*,SAT_VM_CALL_STACK_SIZE> SAT_VMCallStack;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_VirtualMachine {

//------------------------------
private:
//------------------------------

  int             MCodeSegment_size                       = 0;
  SAT_VMValue     MCodeSegment[SAT_VM_CODE_SEGMENT_SIZE]  = {0};
  SAT_VMValue     MDataSegment[SAT_VM_DATA_SEGMENT_SIZE]  = {0};
  SAT_VMValue     MRegisters[SAT_VM_NUM_REGISTERS]        = {0};
  uint32_t        MNumExternals                           = 0;
  SAT_VMExternal  MExternals[SAT_VM_MAX_EXTERNALS]        = {0};
  int             MNumOpcodes                             = 0;
  SAT_VMOpcode*   MOpcodes                                = nullptr;
  SAT_VMValue*    MIP                                     = nullptr;
  SAT_VMCallStack MCallStack  = {};
  SAT_VMDataStack MDataStack  = {};

//------------------------------
public:
//------------------------------

  SAT_VirtualMachine() {
  }

  //----------

  ~SAT_VirtualMachine() {
  }

//------------------------------
public:
//------------------------------

  SAT_VMValue*  getRegisters()                    { return MRegisters; }
  SAT_VMValue   getRegister(int i)                { return MRegisters[i]; }
  void          setRegister(int i, SAT_VMValue v) { MRegisters[i] = v; }

//------------------------------
public:
//------------------------------

  int registerExternal(SAT_VMExternal AExternal) {
    int index = MNumExternals;
    MExternals[MNumExternals++] = AExternal;
    return index;
  }

  //----------

  // http://gcc.gnu.org/onlinedocs/gcc/Labels-as-Values.html
  // You may not use this mechanism to jump to code in a different function.
  // If you do that, totally unpredictable things will happen.

  void translate(SAT_VMOpcode* AOpcodes, int ASize) {
    MOpcodes = AOpcodes;
    MNumOpcodes = ASize;
    execute(-1);
  }

  //----------

  void execute(int APosition=0) {
    if (APosition >= 0) {

//------------------------------
// execute
//------------------------------

      #define VM_NEXT       goto **MIP++
      #define VM_NEXT_cell  *MIP++
      #define VM_PUSH_call  MCallStack.push(MIP)
      #define VM_POP_call   MIP = MCallStack.pop()
      #define VM_PUSH(x)    MDataStack.push((SAT_VMValue)x)
      #define VM_POP        MDataStack.pop()
      #define VM_DROP       MDataStack.drop()
      #define VM_DUP        MDataStack.dup()
      #define VM_DUP2       MDataStack.dup2()
      #define VM_POS(x)     MCodeSegment[x]

      MIP = &MCodeSegment[APosition];
      VM_NEXT;

      //----- opcodes -----

      _op_noop:
        VM_NEXT;

      _op_exit:
        //SAT_VMValue r = (intptr_t)VM_POP;
        return;

      //----- stack -----

      _op_push: // psh const
        VM_PUSH(VM_NEXT_cell);
        VM_NEXT;

      _op_pushr: // push register
        VM_PUSH( MRegisters[VM_NEXT_cell] );
        VM_NEXT;

      _op_popr: // pop register
        {
          SAT_VMValue reg = (SAT_VMValue)VM_NEXT_cell;
          MRegisters[reg] = (SAT_VMValue)VM_POP;
        VM_NEXT;
        }

      _op_pushd: // push from data
        {
          SAT_VMValue *dp = (SAT_VMValue*)VM_NEXT_cell;
          VM_PUSH(*dp);
        }
        VM_NEXT;

      _op_popd: // pop to data
        {
          SAT_VMValue *dp = (SAT_VMValue*)VM_NEXT_cell;
          *dp = (SAT_VMValue)VM_POP;
        }
        VM_NEXT;

      _op_drop:
        VM_DROP;
        VM_NEXT;

      _op_dup:
        VM_DUP;
        VM_NEXT;

      _op_dup2:
        VM_DUP2;
        VM_NEXT;

      //----- branching -----

      _op_call: // call (gosub)
        {
          SAT_VMValue *ip = (SAT_VMValue*)VM_NEXT_cell;
          VM_PUSH_call;
          MIP = ip;
        }
        VM_NEXT;

      _op_ret: // return
        VM_POP_call;
        VM_NEXT;

      _op_jmp: // unconditional jump (goto)
        {
          SAT_VMValue *ip = (SAT_VMValue*)VM_NEXT_cell;
          MIP = ip;
        }
        VM_NEXT;

      _op_jz: // jump if zero
        {
          SAT_VMValue* ip = (SAT_VMValue*)VM_NEXT_cell;
          if ((SAT_VMValue)VM_POP == 0 ) MIP = ip;
        }
        VM_NEXT;

      _op_jnz: // jump if not zero
        {
          SAT_VMValue* ip = (SAT_VMValue*)VM_NEXT_cell;
          if ((SAT_VMValue)VM_POP != 0) MIP = ip;
        }
        VM_NEXT;

      //----- conditional -----

      _op_eq: // equal
        {
          SAT_VMValue v1 = (SAT_VMValue)VM_POP;
          SAT_VMValue v2 = (SAT_VMValue)VM_POP;
          VM_PUSH((v2 == v1));
        }
        VM_NEXT;

      _op_gr: // greater
        {
          SAT_VMValue v1 = (SAT_VMValue)VM_POP;
          SAT_VMValue v2 = (SAT_VMValue)VM_POP;
          VM_PUSH((v2 > v1));
        }
        VM_NEXT;

      _op_le: // less
        {
          SAT_VMValue v1 = (SAT_VMValue)VM_POP;
          SAT_VMValue v2 = (SAT_VMValue)VM_POP;
          VM_PUSH((v2 < v1));
        }
        VM_NEXT;

      _op_ne: // not equal
        {
          SAT_VMValue v1 = (SAT_VMValue)VM_POP;
          SAT_VMValue v2 = (SAT_VMValue)VM_POP;
          VM_PUSH((v2 != v1));
        }
        VM_NEXT;

      //_op_feq: {
      //}
      //
      //_op_fgr: {
      //}
      //
      //_op_fle: {
      //}
      //
      //_op_fne: {
      //}

      //----- math -----

      _op_add: // s += s2
        {
          SAT_VMValue v1 = (SAT_VMValue)VM_POP;
          SAT_VMValue v2 = (SAT_VMValue)VM_POP;
          VM_PUSH((v2 + v1));
        }
        VM_NEXT;

      _op_sub: // s -= s2
        {
          SAT_VMValue v1 = (SAT_VMValue)VM_POP;
          SAT_VMValue v2 = (SAT_VMValue)VM_POP;
          VM_PUSH((v2 - v1));
        }
        VM_NEXT;

      _op_mul: // s *= s2
        {
          SAT_VMValue v1 = (SAT_VMValue)VM_POP;
          SAT_VMValue v2 = (SAT_VMValue)VM_POP;
          VM_PUSH((v2 * v1));
        }
        VM_NEXT;

      _op_div: // s /= s2
        {
          SAT_VMValue v1 = (SAT_VMValue)VM_POP;
          SAT_VMValue v2 = (SAT_VMValue)VM_POP;
          VM_PUSH((v2 / v1));
        }
        VM_NEXT;

      //_op_fadd: {
      //  }
      //
      //_op_fsub: {
      //  }
      //
      //_op_fmul: {
      //  }
      //
      //_op_fdiv: {
      //  }

      _op_inc: // s += 1
        {
          SAT_VMValue* top = (SAT_VMValue*)MDataStack.getTop();
          *top += 1;
        }
        VM_NEXT;

      _op_dec: // s -= 1
        {
          SAT_VMValue* top = (SAT_VMValue*)MDataStack.getTop();
          *top -= 1;
        }
        VM_NEXT;

      //----- io -----

      _op_print: // print (int)
        {
          SAT_VMValue v = (SAT_VMValue)VM_POP;
          //printf("%i\n",(int)v);
          SAT_DPrint("%i\n",(int)v);
        }
        VM_NEXT;

      //----- external -----

      // call external

      _op_ext: // call external
        {
          SAT_VMValue external = (SAT_VMValue)VM_POP;
          MExternals[external](this);
        }
        VM_NEXT;

      //------------------------------

      #undef VM_NEXT
      #undef VM_NEXT_cell
      #undef VM_PUSH_call
      #undef VM_POP_call
      #undef VM_PUSH
      #undef VM_POP
      #undef VM_DROP
      #undef VM_DUP
      #undef VM_DUP2
      #undef VM_POS

    } // execute

    // relocate
    else if (APosition == -1) {

//------------------------------
// translate (relocate?)
//------------------------------

      #define VM_WRITE(op) MCodeSegment[pos++] = (SAT_VMValue)op;

      void* op_adr[] = {
        &&_op_noop,
        &&_op_exit,
        &&_op_push,
        &&_op_drop,
        &&_op_dup,
        &&_op_dup2,
        &&_op_pushr,
        &&_op_popr,
        &&_op_pushd,
        &&_op_popd,
        &&_op_call,
        &&_op_ret,
        &&_op_jmp,
        &&_op_jz,
        &&_op_jnz,
        &&_op_eq,
        &&_op_gr,
        &&_op_le,
        &&_op_ne,
        &&_op_add,
        &&_op_sub,
        &&_op_mul,
        &&_op_div,
        &&_op_inc,
        &&_op_dec,
        &&_op_print,
        &&_op_ext
      };

      int pos = 0;
      for (int i=0; i<MNumOpcodes; i++) {
        int type = MOpcodes[i].type;
        SAT_VMValue value = MOpcodes[i].value;
        switch (type) {
          //case SAT_VM_OP_TYPE_NONE:
          //  break;
          case SAT_VM_OP_TYPE_INTERNAL:
            VM_WRITE( op_adr[ value ] );
            break;
          case SAT_VM_OP_TYPE_EXTERNAL:
          case SAT_VM_OP_TYPE_WORD:
          case SAT_VM_OP_TYPE_INT:
          case SAT_VM_OP_TYPE_FLOAT:
          case SAT_VM_OP_TYPE_PTR:
            VM_WRITE(value);
            break;
          case SAT_VM_OP_TYPE_DATAPTR:
            VM_WRITE( &MDataSegment[ value ] );
            break;
          case SAT_VM_OP_TYPE_CODEPTR:
            VM_WRITE( &MCodeSegment[ value ] );
            break;
          //default:
          //  //error
          //  break;
        } // switch type
      } // for size
      MCodeSegment_size = pos;

      #undef VM_WRITE

    } // relocate

  }

};

//----------------------------------------------------------------------
#endif
