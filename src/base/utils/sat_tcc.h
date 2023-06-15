#ifndef sat_tcc_included
#define sat_tcc_included
//----------------------------------------------------------------------
/*

  Tiny C Compiler

  https://bellard.org/tcc/
    sudo apt-get install tcc
    sudo apt-get install libtcc-dev
    /usr/lib/x86_64-linux-gnu/libtcc.a
    /usr/lib/x86_64-linux-gnu/tcc/
    /usr/share/doc/tcc/

file:///usr/share/doc/tcc/tcc-doc.html


*/
//----------------------------------------------------------------------

//#include <dirent.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include <memory.h>

#include "extern/libtcc.h"

/*
  /usr/lib/x86_64-linux-gnu/tcc/include
  (/usr/lib/tcc/include)
    float.h
    stdarg.h
    stdbool.h
    stddef.h
    tcclib.h
    varargs.h
  /usr/lib/tcc/x86-64/libtcc1.a
*/

//#iefine MAX_SCRIPT_NAME_SIZE 32
//typedef void  (*script_error_func_t)(void *opaque, const char *msg);

#define SAT_TCC_FILETYPE_BINARY   1
#define SAT_TCC_FILETYPE_C        2
#define SAT_TCC_FILETYPE_ASM      3
#define SAT_TCC_FILETYPE_ASM_PP   4

#define SAT_TCC_OUTPUT_MEMORY     1   // output will be run in memory (default)
#define SAT_TCC_OUTPUT_EXE        2   // executable file
#define SAT_TCC_OUTPUT_DLL        3   // dynamic library
#define SAT_TCC_OUTPUT_OBJ        4   // object file
#define SAT_TCC_OUTPUT_PREPROCESS 5   // only preprocess (used internally)
#define SAT_TCC_RELOCATE_AUTO     (void*)1

//----------------------------------------------------------------------

class SAT_Tcc {

//------------------------------
private:
//------------------------------

  TCCState* MState = nullptr;

//------------------------------
public:
//------------------------------

  /*
    create a new TCC compilation context
  */

  SAT_Tcc() {
    MState = tcc_new();
  }

  //----------

  /*
    free a TCC compilation context
  */

  ~SAT_Tcc() {
    tcc_delete(MState);
  }

//------------------------------
public:
//------------------------------

  bool loadFile(const char* filename) {
    bool result = false;
    //FILE* file = fopen(filename, "rt");
    //if (file != NULL) {
    //  fseek(file, 0, SEEK_END);
    //  size_t end = ftell(file);
    //  fseek(file, 0, SEEK_SET);
    //  source_buffer = (char*)malloc(end);
    //  source_allocated = true;
    //  unsigned int r = fread(source_buffer, sizeof(char), end, file);
    //  fclose(file);
    //  source_buffer[end] = '\0';
    //  if (r != end) result = false;
    //  else result = true;
    //}
    return result;
  }

  //----------

  void loadMemory(char* source) {
    //source_buffer = source;
    //source_allocated = false;
  }

  //----------

  void unload() {
    //if (source_allocated && source_buffer) free(source_buffer);
    //source_buffer = NULL;
    //source_allocated = false;
  }

//------------------------------
public:
//------------------------------

  /*
    set CONFIG_TCCDIR at runtime
  */

  void setLibPath(const char *path) {
    tcc_set_lib_path(MState,path);
  }

  //----------

  /*
    set error/warning display callback
  */

  void setErrorFunc(void *error_opaque, void (*error_func)(void *opaque, const char *msg)) {
    tcc_set_error_func(MState,error_opaque,error_func);
  }

  //----------

  /*
    set options as from command line (multiple supported)
  */

  void setOptions(const char *str) {
    tcc_set_options(MState,str);
  }

//------------------------------
public: // preprocessor
//------------------------------

  /*
    add include path
  */

  int addIncludePath(const char *pathname) {
    return tcc_add_include_path(MState,pathname);
  }

  //----------

  /*
    add in system include path
  */

  int addSysIncludePath(const char *pathname) {
    return tcc_add_sysinclude_path(MState,pathname);
  }

  //----------

  /*
    define preprocessor symbol 'sym'. Can put optional value
  */

  void defineSymbol(const char *sym, const char *value) {
    tcc_define_symbol(MState,sym,value);
  }

  //----------

  /*
    undefine preprocess symbol 'sym'
  */

  void undefineSymbol(const char *sym) {
    tcc_undefine_symbol(MState,sym);
  }

//------------------------------
public: // compiling
//------------------------------

  /*
    add a file (C file, dll, object, library, ld script). Return -1 if error.
  */

  int addFile(const char *filename) {
    return tcc_add_file(MState,filename);
  }

  //----------

  /*
    compile a string containing a C source. Return -1 if error.
  */

  int compileString(const char *buf) {
    return tcc_compile_string(MState,buf);
  }

//------------------------------
public: // linking
//------------------------------

  // #define TCC_OUTPUT_MEMORY     1 // output will be run in memory (default)
  // #define TCC_OUTPUT_EXE        2 // executable file
  // #define TCC_OUTPUT_DLL        3 // dynamic library
  // #define TCC_OUTPUT_OBJ        4 // object file
  // #define TCC_OUTPUT_PREPROCESS 5 // only preprocess (used internally)

  /*
    set output type. MUST BE CALLED before any compilation
  */

  int setOutputType(int output_type) {
    return tcc_set_output_type(MState,output_type);
  }

  //----------

  /*
    equivalent to -Lpath option
  */

  int addLibraryPath(const char *pathname) {
    return tcc_add_library_path(MState,pathname);
  }

  //----------

  /*
    the library name is the same as the argument of the '-l' option
  */

  int addLibrary(const char *libraryname) {
    return tcc_add_library(MState,libraryname);
  }

  //----------

  /*
    add a symbol to the compiled program
  */

  int addSymbol(const char *name, const void *val) {
    return tcc_add_symbol(MState,name,val);
  }

  //----------

  /*
    output an executable, library or object file.
    DO NOT call tcc_relocate() before.
  */

  int outputFile(const char *filename) {
    return tcc_output_file(MState,filename);
  }

  //----------

  /*
    link and run main() function and return its value.
    DO NOT call tcc_relocate() before.
  */

  int run(int argc, char **argv) {
    return tcc_run(MState,argc,argv);
  }

  //----------

  // #define TCC_RELOCATE_AUTO (void*)1

  /*
    do all relocations (needed before using tcc_get_symbol())
    possible values for 'ptr':
    - TCC_RELOCATE_AUTO : Allocate and manage memory internally
    - NULL              : return required memory size for the step below
    - memory address    : copy code to memory passed by the caller
    returns -1 if error.
  */

  int relocate(void *ptr) {
    //if (relocated_buffer) free(relocated_buffer);
    //relocated_buffer = malloc(tcc_relocate(state,NULL));
    //tcc_relocate(state,relocated_buffer);
    return tcc_relocate(MState,ptr);
  }

  //----------

  /*
    return symbol value or NULL if not found
  */

  void* getSymbol(const char *name) {
    return tcc_get_symbol(MState,name);
  }

};

//----------------------------------------------------------------------
#endif
