#ifndef sat_tcc_included
#define sat_tcc_included
//----------------------------------------------------------------------

/*
  Tiny C Compiler
  https://bellard.org/tcc/
*/

//----------------------------------------------------------------------

#include <dlfcn.h>
#include "base/sat.h"

// see
//#include "extern/tcc/tinycc/libtcc.h"
// but we load the .so manually..

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

struct SAT_TCC_State;
typedef struct SAT_TCC_State SAT_TCC_State;

typedef void (*SAT_TCC_ErrorFunc)(void *opaque, const char *msg);
typedef void (*SAT_TCC_SymbolCallback)(void *ctx, const char *name, const void *val);

//

typedef SAT_TCC_State*    (*sat_tcc_new_t)(void);
typedef void              (*sat_tcc_delete_t)(SAT_TCC_State *s);
typedef void              (*sat_tcc_set_lib_path_t)(SAT_TCC_State *s, const char *path);
typedef void              (*sat_tcc_set_error_func_t)(SAT_TCC_State *s, void *error_opaque, SAT_TCC_ErrorFunc);
typedef SAT_TCC_ErrorFunc (*sat_tcc_get_error_func_t)(SAT_TCC_State *s);
typedef void*             (*sat_tcc_get_error_opaque_t)(SAT_TCC_State *s);
typedef void              (*sat_tcc_set_options_t)(SAT_TCC_State *s, const char *str);
typedef int               (*sat_tcc_add_include_path_t)(SAT_TCC_State *s, const char *pathname);
typedef int               (*sat_tcc_add_sysinclude_path_t)(SAT_TCC_State *s, const char *pathname);
typedef void              (*sat_tcc_define_symbol_t)(SAT_TCC_State *s, const char *sym, const char *value);
typedef void              (*sat_tcc_undefine_symbol_t)(SAT_TCC_State *s, const char *sym);
typedef int               (*sat_tcc_add_file_t)(SAT_TCC_State *s, const char *filename);
typedef int               (*sat_tcc_compile_string_t)(SAT_TCC_State *s, const char *buf);
typedef int               (*sat_tcc_set_output_type_t)(SAT_TCC_State *s, int output_type);
typedef int               (*sat_tcc_add_library_path_t)(SAT_TCC_State *s, const char *pathname);
typedef int               (*sat_tcc_add_library_t)(SAT_TCC_State *s, const char *libraryname);
typedef int               (*sat_tcc_add_symbol_t)(SAT_TCC_State *s, const char *name, const void *val);
typedef int               (*sat_tcc_output_file_t)(SAT_TCC_State *s, const char *filename);
typedef int               (*sat_tcc_run_t)(SAT_TCC_State *s, int argc, char **argv);
typedef int               (*sat_tcc_relocate_t)(SAT_TCC_State *s1, void *ptr);
typedef void*             (*sat_tcc_get_symbol_t)(SAT_TCC_State *s, const char *name);
typedef void              (*sat_tcc_list_symbols_t)(SAT_TCC_State *s, void *ctx, SAT_TCC_SymbolCallback);  

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Tcc {

//------------------------------
private:
//------------------------------

  void*           MLibHandle    = nullptr;
  SAT_TCC_State*  MTccState     = nullptr;

//------------------------------
public:
//------------------------------

  SAT_Tcc() {
  }

  //----------

  ~SAT_Tcc() {
    //unload_lib();
  }

//------------------------------
public:
//------------------------------

  sat_tcc_new_t                 m_new                 = nullptr;
  sat_tcc_delete_t              m_delete              = nullptr;
  sat_tcc_set_lib_path_t        m_set_lib_path        = nullptr;
  sat_tcc_set_error_func_t      m_set_error_func      = nullptr;
  sat_tcc_get_error_func_t      m_get_error_func      = nullptr;
  sat_tcc_get_error_opaque_t    m_get_error_opaque    = nullptr;
  sat_tcc_set_options_t         m_set_options         = nullptr;
  sat_tcc_add_include_path_t    m_add_include_path    = nullptr;
  sat_tcc_add_sysinclude_path_t m_add_sysinclude_path = nullptr;
  sat_tcc_define_symbol_t       m_define_symbol       = nullptr;
  sat_tcc_undefine_symbol_t     m_undefine_symbol     = nullptr;
  sat_tcc_add_file_t            m_add_file            = nullptr;
  sat_tcc_compile_string_t      m_compile_string      = nullptr;
  sat_tcc_set_output_type_t     m_set_output_type     = nullptr;
  sat_tcc_add_library_path_t    m_add_library_path    = nullptr;
  sat_tcc_add_library_t         m_add_library         = nullptr;
  sat_tcc_add_symbol_t          m_add_symbol          = nullptr;
  sat_tcc_output_file_t         m_output_file         = nullptr;
  sat_tcc_run_t                 m_run                 = nullptr;
  sat_tcc_relocate_t            m_relocate            = nullptr;
  sat_tcc_get_symbol_t          m_get_symbol          = nullptr;
  sat_tcc_list_symbols_t        m_list_symbols        = nullptr;

//------------------------------
public:
//------------------------------

  bool load_lib(const char* path) {
    MLibHandle = dlopen(path,RTLD_LAZY | RTLD_LOCAL);
    if (!MLibHandle) {
      SAT_Print("Error! Can't load '%s'\n",path);
      return false;
    }
    
    //todo: error checking

    m_new                 = (sat_tcc_new_t)dlsym(MLibHandle,"tcc_new");
    m_delete              = (sat_tcc_delete_t)dlsym(MLibHandle,"tcc_delete");
    m_set_lib_path        = (sat_tcc_set_lib_path_t)dlsym(MLibHandle,"tcc_set_lib_path");
    m_set_error_func      = (sat_tcc_set_error_func_t)dlsym(MLibHandle,"tcc_set_error_func");
    m_get_error_func      = (sat_tcc_get_error_func_t)dlsym(MLibHandle,"tcc_get_error_func");
    m_get_error_opaque    = (sat_tcc_get_error_opaque_t)dlsym(MLibHandle,"tcc_get_error_opaque");
    m_set_options         = (sat_tcc_set_options_t)dlsym(MLibHandle,"tcc_set_options");
    m_add_include_path    = (sat_tcc_add_include_path_t)dlsym(MLibHandle,"tcc_add_include_path");
    m_add_sysinclude_path = (sat_tcc_add_sysinclude_path_t)dlsym(MLibHandle,"tcc_add_sysinclude_path");
    m_define_symbol       = (sat_tcc_define_symbol_t)dlsym(MLibHandle,"tcc_define_symbol");
    m_undefine_symbol     = (sat_tcc_undefine_symbol_t)dlsym(MLibHandle,"tcc_undefine_symbol");
    m_add_file            = (sat_tcc_add_file_t)dlsym(MLibHandle,"tcc_add_file");
    m_compile_string      = (sat_tcc_compile_string_t)dlsym(MLibHandle,"tcc_compile_string");
    m_set_output_type     = (sat_tcc_set_output_type_t)dlsym(MLibHandle,"tcc_set_output_type");
    m_add_library_path    = (sat_tcc_add_library_path_t)dlsym(MLibHandle,"tcc_add_library_path");
    m_add_library         = (sat_tcc_add_library_t)dlsym(MLibHandle,"tcc_add_library");
    m_add_symbol          = (sat_tcc_add_symbol_t)dlsym(MLibHandle,"tcc_add_symbol");
    m_output_file         = (sat_tcc_output_file_t)dlsym(MLibHandle,"tcc_output_file");
    m_run                 = (sat_tcc_run_t)dlsym(MLibHandle,"tcc_run");
    m_relocate            = (sat_tcc_relocate_t)dlsym(MLibHandle,"tcc_relocate");
    m_get_symbol          = (sat_tcc_get_symbol_t)dlsym(MLibHandle,"tcc_get_symbol");
    m_list_symbols        = (sat_tcc_list_symbols_t)dlsym(MLibHandle,"tcc_list_symbols");
    
    /*
      create a new TCC compilation context
    */

    MTccState = m_new();
    SAT_Assert(MTccState);
    return (MTccState);
    
  }

  //----------

  /*
    free a TCC compilation context
  */

  void unload_lib() {
    if (MTccState) m_delete(MTccState);
    if (MLibHandle) dlclose(MLibHandle);
  }

//------------------------------
public:
//------------------------------

  /*
    set CONFIG_TCCDIR at runtime
  */

  void setLibPath(const char *path) {
    m_set_lib_path(MTccState,path);
  }

  //----------

  /*
    set error/warning display callback
  */

  void setErrorFunc(void *error_opaque, void (*error_func)(void *opaque, const char *msg)) {
    m_set_error_func(MTccState,error_opaque,error_func);
  }
  
  //----------

  /*
    return error/warning callback
  */

  SAT_TCC_ErrorFunc getErrorFunc() {
    return m_get_error_func(MTccState);
  }

  //----------

  /*
    return error/warning callback opaque pointer
  */

  void* getErrorOpaque() {
    return m_get_error_opaque(MTccState);
  }

  //----------

  /*
    set options as from command line (multiple supported)
  */

  void setOptions(const char *str) {
    m_set_options(MTccState,str);
  }

//------------------------------
public: // preprocessor
//------------------------------

  /*
    add include path
  */

  int addIncludePath(const char *pathname) {
    return m_add_include_path(MTccState,pathname);
  }

  //----------

  /*
    add in system include path
  */

  int addSysIncludePath(const char *pathname) {
    return m_add_sysinclude_path(MTccState,pathname);
  }

  //----------

  /*
    define preprocessor symbol 'sym'. Can put optional value
  */

  void defineSymbol(const char *sym, const char *value) {
    m_define_symbol(MTccState,sym,value);
  }

  //----------

  /*
    undefine preprocess symbol 'sym'
  */

  void undefineSymbol(const char *sym) {
    m_undefine_symbol(MTccState,sym);
  }

//------------------------------
public: // compiling
//------------------------------

  /*
    add a file (C file, dll, object, library, ld script). Return -1 if error.
  */

  int addFile(const char *filename) {
    return m_add_file(MTccState,filename);
  }
  
  //----------

  /*
    compile a string containing a C source. Return -1 if error.
  */

  int compileString(const char *buf) {
    return m_compile_string(MTccState,buf);
  }

//------------------------------
public: // linking
//------------------------------

  /*
    set output type. MUST BE CALLED before any compilation
  */

  int setOutputType(int output_type) {
    return m_set_output_type(MTccState,output_type);
  }

  //----------

  /*
    equivalent to -Lpath option
  */

  int addLibraryPath(const char *pathname) {
    return m_add_library_path(MTccState,pathname);
  }

  //----------

  /*
    the library name is the same as the argument of the '-l' option
  */

  int addLibrary(const char *libraryname) {
    return m_add_library(MTccState,libraryname);
  }

  //----------

  /*
    add a symbol to the compiled program
  */

  int addSymbol(const char *name, const void *val) {
    return m_add_symbol(MTccState,name,val);
  }

  //----------

  /*
    output an executable, library or object file.
    DO NOT call tcc_relocate() before.
  */

  int outputFile(const char *filename) {
    return m_output_file(MTccState,filename);
  }

  //----------

  /*
    link and run main() function and return its value.
    DO NOT call tcc_relocate() before.
  */

  int run(int argc, char **argv) {
    return m_run(MTccState,argc,argv);
  }

  //----------

  /*
    do all relocations (needed before using tcc_get_symbol())
    possible values for 'ptr':
    - TCC_RELOCATE_AUTO : Allocate and manage memory internally
    - NULL              : return required memory size for the step below
    - memory address    : copy code to memory passed by the caller
    returns -1 if error.
  */

  int relocate(void *ptr) {
    return m_relocate(MTccState,ptr);
  }

  //----------

  /*
    return symbol value or NULL if not found
  */

  void* getSymbol(const char *name) {
    return m_get_symbol(MTccState,name);
  }
  
  //----------

  /*
    return symbol value or NULL if not found
  */
  
  void listSymbols(void* ctx, SAT_TCC_SymbolCallback cb) {
    m_list_symbols(MTccState,ctx,cb);
  }

};


//----------------------------------------------------------------------
#endif
