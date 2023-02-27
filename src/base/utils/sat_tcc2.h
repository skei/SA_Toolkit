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
//#include <libtcc.h>

#include <dlfcn.h>

//----------------------------------------------------------------------

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

struct TCCState;
typedef struct TCCState TCCState;

typedef TCCState* (*tcc_new_t)(void);
typedef void      (*tcc_delete_t)(TCCState *s);
typedef void      (*tcc_set_lib_path_t)(TCCState *s, const char *path);
typedef void      (*tcc_set_error_func_t)(TCCState *s, void *error_opaque, void (*error_func)(void *opaque, const char *msg));
typedef void      (*tcc_set_options_t)(TCCState *s, const char *str);
typedef int       (*tcc_add_include_path_t)(TCCState *s, const char *pathname);
typedef int       (*tcc_add_sysinclude_path_t)(TCCState *s, const char *pathname);
typedef void      (*tcc_define_symbol_t)(TCCState *s, const char *sym, const char *value);
typedef void      (*tcc_undefine_symbol_t)(TCCState *s, const char *sym);
typedef int       (*tcc_add_file_t)(TCCState *s, const char *filename);
typedef int       (*tcc_compile_string_t)(TCCState *s, const char *buf);
typedef int       (*tcc_set_output_type_t)(TCCState *s, int output_type);
typedef int       (*tcc_add_library_path_t)(TCCState *s, const char *pathname);
typedef int       (*tcc_add_library_t)(TCCState *s, const char *libraryname);
typedef int       (*tcc_add_symbol_t)(TCCState *s, const char *name, const void *val);
typedef int       (*tcc_output_file_t)(TCCState *s, const char *filename);
typedef int       (*tcc_run_t)(TCCState *s, int argc, char **argv);
typedef int       (*tcc_relocate_t)(TCCState *s1, void *ptr);
typedef void*     (*tcc_get_symbol_t)(TCCState *s, const char *name);

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Tcc {

//------------------------------
private:
//------------------------------

  bool      MInitialized  = false;
  void*     MLibHandle    = nullptr;
  TCCState* MTccState     = nullptr;

//------------------------------
public:
//------------------------------

// path = path to tcc.so

  SAT_Tcc(const char* path) {
    //MInitialized = load_lib(path);

  }

  //----------

  ~SAT_Tcc() {
    //unload_lib();
  }

//------------------------------
public:
//------------------------------

  tcc_new_t                 tcc_new                 = nullptr;
  tcc_delete_t              tcc_delete              = nullptr;
  tcc_set_lib_path_t        tcc_set_lib_path        = nullptr;
  tcc_set_error_func_t      tcc_set_error_func      = nullptr;
  tcc_set_options_t         tcc_set_options         = nullptr;
  tcc_add_include_path_t    tcc_add_include_path    = nullptr;
  tcc_add_sysinclude_path_t tcc_add_sysinclude_path = nullptr;
  tcc_define_symbol_t       tcc_define_symbol       = nullptr;
  tcc_undefine_symbol_t     tcc_undefine_symbol     = nullptr;
  tcc_add_file_t            tcc_add_file            = nullptr;
  tcc_compile_string_t      tcc_compile_string      = nullptr;
  tcc_set_output_type_t     tcc_set_output_type     = nullptr;
  tcc_add_library_path_t    tcc_add_library_path    = nullptr;
  tcc_add_library_t         tcc_add_library         = nullptr;
  tcc_add_symbol_t          tcc_add_symbol          = nullptr;
  tcc_output_file_t         tcc_output_file         = nullptr;
  tcc_run_t                 tcc_run                 = nullptr;
  tcc_relocate_t            tcc_relocate            = nullptr;
  tcc_get_symbol_t          tcc_get_symbol          = nullptr;

//------------------------------
public:
//------------------------------

  bool load_lib(const char* path) {
    MLibHandle = dlopen(path,RTLD_LAZY | RTLD_LOCAL);
    if (!MLibHandle) return false;
    tcc_new                 = (tcc_new_t)dlsym(MLibHandle,"tcc_new");
    tcc_delete              = (tcc_delete_t)dlsym(MLibHandle,"tcc_delete");
    tcc_set_lib_path        = (tcc_set_lib_path_t)dlsym(MLibHandle,"tcc_set_lib_path");
    tcc_set_error_func      = (tcc_set_error_func_t)dlsym(MLibHandle,"tcc_set_error_func");
    tcc_set_options         = (tcc_set_options_t)dlsym(MLibHandle,"tcc_set_options");
    tcc_add_include_path    = (tcc_add_include_path_t)dlsym(MLibHandle,"tcc_add_include_path");
    tcc_add_sysinclude_path = (tcc_add_sysinclude_path_t)dlsym(MLibHandle,"tcc_add_sysinclude_path");
    tcc_define_symbol       = (tcc_define_symbol_t)dlsym(MLibHandle,"tcc_define_symbol");
    tcc_undefine_symbol     = (tcc_undefine_symbol_t)dlsym(MLibHandle,"tcc_undefine_symbol");
    tcc_add_file            = (tcc_add_file_t)dlsym(MLibHandle,"tcc_add_file");
    tcc_compile_string      = (tcc_compile_string_t)dlsym(MLibHandle,"tcc_compile_string");
    tcc_set_output_type     = (tcc_set_output_type_t)dlsym(MLibHandle,"tcc_set_output_type");
    tcc_add_library_path    = (tcc_add_library_path_t)dlsym(MLibHandle,"tcc_add_library_path");
    tcc_add_library         = (tcc_add_library_t)dlsym(MLibHandle,"tcc_add_library");
    tcc_add_symbol          = (tcc_add_symbol_t)dlsym(MLibHandle,"tcc_add_symbol");
    tcc_output_file         = (tcc_output_file_t)dlsym(MLibHandle,"tcc_output_file");
    tcc_run                 = (tcc_run_t)dlsym(MLibHandle,"tcc_run");
    tcc_relocate            = (tcc_relocate_t)dlsym(MLibHandle,"tcc_relocate");
    tcc_get_symbol          = (tcc_get_symbol_t)dlsym(MLibHandle,"tcc_get_symbol");
    MTccState = tcc_new();
    SAT_Assert(MTccState);
    return (MTccState);
  }

  //----------

  void unload_lib() {
    tcc_delete(MTccState);
    if (MLibHandle) dlclose(MLibHandle);
  }

//------------------------------
public:
//------------------------------

  /*
    create a new TCC compilation context
  */

  //TCCState* (*tcc_new_t)(void) {
  //}

  /*
    free a TCC compilation context
  */

  //void (*tcc_delete_t)(TCCState *s) {
  //}

  //----------

  /*
    set CONFIG_TCCDIR at runtime
  */

  void setLibPath(const char *path) {
    tcc_set_lib_path(MTccState,path);
  }

  /*
    set error/warning display callback
  */

  void setErrorFunc(void *error_opaque, void (*error_func)(void *opaque, const char *msg)) {
    tcc_set_error_func(MTccState,error_opaque,error_func);
  }

  /*
    set options as from command line (multiple supported)
  */

  void setOptions(const char *str) {
    tcc_set_options(MTccState,str);
  }

//------------------------------
public: // preprocessor
//------------------------------

  /*
    add include path
  */

//  int (*tcc_add_include_path_t)(TCCState *s, const char *pathname);

  /*
    add in system include path
  */

//  int (*tcc_add_sysinclude_path_t)(TCCState *s, const char *pathname);

  /*
    define preprocessor symbol 'sym'. Can put optional value
  */

//  void (*tcc_define_symbol_t)(TCCState *s, const char *sym, const char *value);

  /*
    undefine preprocess symbol 'sym'
  */

//  void (*tcc_undefine_symbol_t)(TCCState *s, const char *sym);

//------------------------------
public: // compiling
//------------------------------

  /*
    add a file (C file, dll, object, library, ld script). Return -1 if error.
  */

//  int (*tcc_add_file_t)(TCCState *s, const char *filename);

  /*
    compile a string containing a C source. Return -1 if error.
  */

  int compileString(const char *buf) {
    return tcc_compile_string(MTccState,buf);
  }

//------------------------------
public: // linking
//------------------------------

  /*
    set output type. MUST BE CALLED before any compilation
  */

//  int (*tcc_set_output_type_t)(TCCState *s, int output_type);

  /*
    equivalent to -Lpath option
  */

//  int (*tcc_add_library_path_t)(TCCState *s, const char *pathname);

  /*
    the library name is the same as the argument of the '-l' option
  */

//  int (*tcc_add_library_t)(TCCState *s, const char *libraryname);

  /*
    add a symbol to the compiled program
  */

//  int (*tcc_add_symbol_t)(TCCState *s, const char *name, const void *val);

  /*
    output an executable, library or object file.
    DO NOT call tcc_relocate() before.
  */

//  int (*tcc_output_file_t)(TCCState *s, const char *filename);

  /*
    link and run main() function and return its value.
    DO NOT call tcc_relocate() before.
  */

  int run(int argc, char **argv) {
    return tcc_run(MTccState,argc,argv);
  }

  /*
    do all relocations (needed before using tcc_get_symbol())
    possible values for 'ptr':
    - TCC_RELOCATE_AUTO : Allocate and manage memory internally
    - NULL              : return required memory size for the step below
    - memory address    : copy code to memory passed by the caller
    returns -1 if error.
  */

//  int (*tcc_relocate_t)(TCCState *s1, void *ptr);

  /*
    return symbol value or NULL if not found
  */

//  void* (*tcc_get_symbol_t)(TCCState *s, const char *name);

};


//----------------------------------------------------------------------
#endif
