#ifndef sat_vm_compiler_included
#define sat_vm_compiler_included
//----------------------------------------------------------------------

/*
  TODO:
    - 2-pass, 2nd to resolve adresses to labels and words
    - macro, insert opcodes directly, instead of func call
    - do some of this in parsing...
    - jit (?)
    - comments: { }

    label & -> @
    var   % -> #
*/

#include "base/types/sat_stack.h"
//#include "src/h_String.h"
#include "base/utils/sat_vm.h"

#define SAT_VM_COMPILER_NUM_OPCODES     65536
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

class SAT_VMCompiler {

//------------------------------
private:
//------------------------------

  char*           m_Sourcecode      = nullptr;
  int             m_SourcecodeSize  = 0;
  SAT_VMOpcode*   m_Opcodes         = nullptr;
  int             m_NumOpcodes      = 0;
  SAT_VMTokens    m_Tokens          = {};
  SAT_VMSrcWords  m_Words           = {};
  SAT_VMSrcLabels m_Labels          = {};
  SAT_VMSrcVars   m_Vars            = {};
  SAT_VMCondStack m_CondStack       = {};
  int             m_Pos             = 0;
  bool            m_NewWord         = false;
  bool            m_NewLabel        = false;
  bool            m_NewVar          = false;

//------------------------------
public:
//------------------------------

  SAT_VMCompiler() {
    m_Opcodes = new SAT_VMOpcode[SAT_VM_COMPILER_NUM_OPCODES];
  }

  //----------

  ~SAT_VMCompiler() {
    if (m_Sourcecode) free(m_Sourcecode);
    if (m_Opcodes) delete[] m_Opcodes;
    // delete words
    // delete labels
  }

//------------------------------
//
//------------------------------

  #define WRITE_op(t,v) m_Opcodes[m_Pos].type = t;                \
                        m_Opcodes[m_Pos].value = (SAT_VMValue)v;  \
                        m_Pos++

  #define PUSH_cond     m_CondStack.push(m_Pos)
  #define POP_cond      m_CondStack.pop()


//------------------------------
public:
//------------------------------

  void parse(char* a_Source, int a_Size) {
    m_SourcecodeSize = a_Size;
    m_Sourcecode = (char*)malloc(a_Size + 1);
    memcpy(m_Sourcecode,a_Source,a_Size);
    m_Sourcecode[a_Size] = 0;
    int pos = 0;
    char prev = 0;
    while (pos < a_Size) {
      unsigned char ch = m_Sourcecode[pos];
      if (ch <= 32) {
        ch = 0;
        m_Sourcecode[pos] = 0;
      }
      else if (prev == 0) {
        const char* token = &m_Sourcecode[pos];
        m_Tokens.append(token);
      }
      prev = ch;
      pos++;
    }
  }

  //----------

  //void preprocess() {
  //}

  //----------

  void compile() {
    m_Pos = 0;
    m_NewWord = false;
    uint32_t tok = 0;
    while (tok < m_Tokens.size()) {
      //char* token = NEXT_t;
      const char* token = m_Tokens[tok++];
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
    m_NumOpcodes = m_Pos;
  }

  //----------

  int getNumOpcodes() {
    return m_NumOpcodes;
  }

  //----------

  SAT_VMOpcode* getOpcodes() {
    return m_Opcodes;
  }

  //----------

  uint32_t getNumTokens() {
    return m_Tokens.size();
  }

  const char* getToken(uint32_t AIndex) {
    return m_Tokens[AIndex];
  }

  //----------

  int findWord(char* token) {
    for (uint32_t i=0; i<m_Words.size(); i++) {
      if (strcmp(m_Words[i].name,token)==0) return m_Words[i].pos;
      }
    return -1;
  }

  //----------

  int findLabel(char* token) {
    for (uint32_t i=0; i<m_Labels.size(); i++) {
      if (strcmp(m_Labels[i].name,token)==0) return m_Labels[i].pos;
    }
    return -1;
  }

  //----------

  int findVar(char* token) {
    for (uint32_t i=0; i<m_Vars.size(); i++) {
      if (strcmp(m_Vars[i].name,token)==0) return m_Vars[i].pos;
    }
    return -1;
  }

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
    for (uint32_t i=0; i<m_Words.size(); i++) {
      if (strcmp(m_Words[i].name,token) == 0) return i;
    }
    return -1;
  }

  //----------

  int _find_label(const char* token) {
    for (uint32_t i=0; i<m_Labels.size(); i++) {
      if (strcmp(m_Labels[i].name,token) == 0) return i;
    }
    return -1;
  }

  //----------

  int _find_var(const char* token) {
    for (uint32_t i=0; i<m_Vars.size(); i++) {
      if (strcmp(m_Vars[i].name,token) == 0) return i;
    }
    return -1;
  }

//------------------------------
private:
//------------------------------

  // ( call, ) return

  bool encode_newword(const char* token) {
    if ((strcmp(token,"(") == 0)) {
      m_NewWord = true;
      return true;
    }
    else if (strcmp(token,")") == 0) {
      WRITE_op(SAT_VM_OP_TYPE_INTERNAL,SAT_OP_RET);
      m_NewWord = false;
      return true;
    }
    else if (m_NewWord) {
      SAT_VMSrcWord word;
      word.pos = m_Pos;
      strcpy(word.name,token);
      m_Words.append(word);
      m_NewWord = false;
      return true;
    }
    return false;
  }

  //----------

  // &

  bool encode_newlabel(const char* token) {
    if ((strcmp(token,"&") == 0)) {
      m_NewLabel = true;
      return true;
    }
    else if (m_NewLabel) {
      SAT_VMSrcLabel label;
      label.pos = m_Pos;
      strcpy(label.name,token);
      m_Labels.append(label);
      m_NewLabel = false;
      return true;
    }
    return false;
  }

  //----------

  // %

  bool encode_newvar(const char* token) {
    if ((strcmp(token,"%") == 0)) {
      m_NewVar = true;
      return true;
    }
    else if (m_NewVar) {
      SAT_VMSrcVar var;
      var.pos = m_Vars.size();
      strcpy(var.name,token);
      m_Vars.append(var);
      m_NewVar = false;
      return true;
    }
    return false;
  }

  //----------

  // ? : ;

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
      m_Opcodes[pos].value = (SAT_VMValue)m_Pos;
      return true;
    }
    else if ((strcmp(token,";") == 0)) {// endif
      int pos = POP_cond;
      m_Opcodes[pos].value = (SAT_VMValue)m_Pos;
      return true;
    }
    return false;
  }

  //----------

  bool encode_word(const char* token) {
    int word = _find_word(token);
    if (word >= 0) {
      WRITE_op(SAT_VM_OP_TYPE_INTERNAL,SAT_OP_CALL);
      WRITE_op(SAT_VM_OP_TYPE_CODEPTR, m_Words[word].pos);
      return true;
    }
    return false;
  }

  //----------

  bool encode_label(const char* token) {
    int label = _find_label(token);
    if (label >= 0) {
      //WRITE_op(SAT_VM_OP_TYPE_INTERNAL,SAT_OP_jmp);
      WRITE_op(SAT_VM_OP_TYPE_CODEPTR, m_Labels[label].pos);
      return true;
    }
    return false;
  }

  //----------

  // <- push
  // -> pop

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
      WRITE_op(SAT_VM_OP_TYPE_DATAPTR, m_Vars[var].pos);
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

};

//----------------------------------------------------------------------
#endif
