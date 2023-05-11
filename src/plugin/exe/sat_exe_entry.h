#ifndef sat_exe_entry_included
#define sat_exe_entry_included
//----------------------------------------------------------------------

//#define SAT_PLUGIN_EXE_EXECUTABLE_LIBRARY

#include "plugin/sat_host_implementation.h"
#include "plugin/clap/sat_clap_entry.h"

//----------------------------------------------------------------------
//
// exe window
//
//----------------------------------------------------------------------

class SAT_ExeWindow
: public SAT_ImplementedWindow {

//------------------------------
private:
//------------------------------

  const clap_plugin_t*      MPlugin = nullptr;
  const clap_plugin_gui_t*  MGui    = nullptr;

//------------------------------
public:
//------------------------------

  SAT_ExeWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent, const clap_plugin_t* APlugin, const clap_plugin_gui_t* AGui)
  : SAT_ImplementedWindow(AWidth,AHeight,AParent) {
    MPlugin = APlugin;
    MGui = AGui;
    setTitle("SA_Toolkit / SAT_ExeWindow");
  }
  
//------------------------------
public:
//------------------------------

  void on_window_resize(int32_t AWidth, int32_t AHeight) final {
    MGui->set_size(MPlugin,AWidth,AHeight);
  }

};

//----------------------------------------------------------------------
//
// exe host
//
//----------------------------------------------------------------------

class SAT_ExeHostImplementation
: public SAT_HostImplementation {

//----------
public:
//----------

  SAT_ExeHostImplementation(int argc, char** argv)
  : SAT_HostImplementation() {
  }

  //----------

  ~SAT_ExeHostImplementation() {
  }

//----------
public:
//----------

};

//----------------------------------------------------------------------
//
// editor
//
//----------------------------------------------------------------------

void handle_editor(const clap_plugin_t* plugin) {
  const clap_plugin_gui_t* gui = (const clap_plugin_gui_t*)plugin->get_extension(plugin,CLAP_EXT_GUI);
  if (gui) {
    uint32_t width,height;
    
    #ifdef SAT_LINUX
      gui->create(plugin,CLAP_WINDOW_API_X11,false);
    #endif
    
    #ifdef SAT_WIN32
      gui->create(plugin,CLAP_WINDOW_API_WIN32,false);
    #endif

    gui->set_scale(plugin,1.0);
    gui->get_size(plugin,&width,&height);
    
    //SAT_Print("creating host window\n");
    SAT_ExeWindow* window = new SAT_ExeWindow(width,height,0,plugin,gui);
    //SAT_Print("host window created\n");
    
    #ifdef SAT_LINUX
      xcb_window_t x11window = window->getX11Window();
      const clap_window_t clapwindow = { .api = CLAP_WINDOW_API_X11, .x11 = x11window };
    #endif
    
    #ifdef SAT_WIN32
      HWND win32window = window->getWin32Window();
      const clap_window_t clapwindow = { .api = CLAP_WINDOW_API_WIN32, .win32 = win32window };
    #endif
    
    gui->set_parent(plugin,&clapwindow);
    gui->show(plugin);
    gui->set_size(plugin,width,height); // is this needed?

    window->show();
    window->eventLoop();
    
    // window->hide(); //why not here?

    gui->hide(plugin);
    gui->destroy(plugin);
    
    window->hide();
    delete window;
  }
}

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

void handle_plugin(const clap_plugin_t* plugin) {
  
  SAT_Plugin* satplugin = (SAT_Plugin*)plugin->plugin_data;
  satplugin->setPluginFormat("EXE");
  
  plugin->init(plugin);
  plugin->activate(plugin,44100,128,128);
  plugin->start_processing(plugin);
  //clap_process_t process = {0};
  //process.audio_inputs        = nullptr;
  //process.audio_inputs_count  = 0;
  //process.audio_outputs       = nullptr;
  //process.audio_outputs_count = 0;
  //process.frames_count        = 128;
  //process.in_events           = nullptr;
  //process.out_events          = nullptr;
  //process.steady_time         = 0;
  //process.transport           = nullptr;
  //plugin->process(plugin,&process); // will probably crash..
  handle_editor(plugin);
  plugin->stop_processing(plugin);
  plugin->deactivate(plugin);
  plugin->destroy(plugin);
  //SAT_Plugin* splug = (SAT_Plugin*)plugin->plugin_data;
  //delete splug;
}

//----------------------------------------------------------------------
//
// main
//
//----------------------------------------------------------------------

int main(int argc, char** argv) {
  uint32_t plugin_index = 0;
  SAT_ExeHostImplementation* hostimpl = new SAT_ExeHostImplementation(argc,argv);
  const clap_host_t* host = hostimpl->getHost();
  if (!host) { SAT_Print("! ERROR: Couldn't create exe host implementation\n"); }
  else {
    const clap_plugin_entry_t* entry = &clap_entry;
    if (!entry) { SAT_Print("! ERROR: clap_entry is null\n"); }
    else {
      //uint32_t count = entry->init(plugin_path);
      const clap_plugin_factory_t* factory = (const clap_plugin_factory_t*)entry->get_factory(CLAP_PLUGIN_FACTORY_ID);
      if (!factory) { SAT_Print("! ERROR: Couldn't get factory from entry\n"); }
      else {
        uint32_t count = factory->get_plugin_count(factory);
        if (count == 0) { SAT_Print("! ERROR: Plugin count is 0\n"); }
        else {
          const clap_plugin_descriptor_t* descriptor = factory->get_plugin_descriptor(factory,plugin_index);
          if (!descriptor) { SAT_Print("! ERROR: Couldn't get descriptor from factory\n"); }
          else {
            const char* plugin_id = descriptor->id;
            if (!plugin_id) { SAT_Print("! ERROR: plugin_id is null\n"); }
            else {
              const clap_plugin_t* plugin = factory->create_plugin(factory,host,plugin_id);
              if (!plugin) { SAT_Print("! ERROR: Couldn't create plugin from factory\n"); }
              else { 
                handle_plugin(plugin);
              } // plugin
            } // plugin_id
          } // descriptor
        } // count
      } // factory
      entry->deinit();
    } // entry
    delete hostimpl;
  }
  return 0;
}

//----------------------------------------------------------------------
//
// executable library
//
//----------------------------------------------------------------------

  //-rdynamic
  //-Wl,-Bsymbolic
  //-Wl,-e,exe_entry_point

#ifndef SAT_WIN32
#ifdef SAT_PLUGIN_EXE_EXECUTABLE_LIBRARY

  #include <sys/types.h>

  const char interp_section[] __attribute__((section(".interp"))) = "/lib64/ld-linux-x86-64.so.2";

  extern "C" {
  
    //int my_test_global = 123;
    //
    //int test_func() {
    //  printf("> test_func()\n");
    //  return 42;
    //}
    //
    // not called?
    //int test_func_result = test_func();
    //
    // not called?
    //__attribute__((constructor))
    //void ctor_test_func() {
    //  SAT_Print("> ctor_test_func()\n");
    //  test_func_result = 41;
    //}

    int main_result;

    int* main_trampoline(int argc, char** argv, char** env) {
      //SAT_Print("> main_trampoline()\n");
      //SAT_Print("(my_test_global = %i)\n",my_test_global);
      //SAT_Print("(test_func_result = %i)\n",test_func_result);
      //SAT_Print("calling real main()..\n");
      
      main_result = main(argc,argv);//,env);
      
      //SAT_Print("..and we're back..\n");
      return &main_result;
    }
    
  //} // extern c

    //------------------------------
    // __libc_start_main
    //------------------------------
  
    //extern int __libc_start_main(
    //  //int  *(main)(int, char**, char**),
    //  int  (*main)(int, char**, char**),
    //  int    argc,
    //  char** ubp_av,
    //  void (*init)(void),
    //  void (*fini)(void),
    //  void (*rtld_fini)(void),
    //  void (*stack_end)
    //);

    extern int __libc_start_main(int *(main) (int, char * *, char * *), int argc, char * * ubp_av, void (*init) (void), void (*fini) (void), void (*rtld_fini) (void), void (* stack_end));

    //------------------------------
    // entry_point
    //------------------------------

    __attribute__((force_align_arg_pointer))
    void exe_entry_point() {
      SAT_Print("* entry_point()\n");
      int argc = 0;
      const char* argv[] = { "", 0 };
      SAT_Print("> argc %i argv %s\n",argc,argv[0]);
      SAT_Print("calling __libc_start_main..\n");
    
      //    SAT_REGISTRY.initialize();

      //__libc_start_main(main_trampoline,argc,(char**)argv,my_init,my_fini,my_rtld_fini,0);//&my_stack[500000]);
      //__libc_start_main(main,argc,(char**)argv,nullptr,nullptr,nullptr,0);//&my_stack[500000]);
      __libc_start_main(main_trampoline,argc,(char**)argv,nullptr,nullptr,nullptr,0);//&my_stack[500000]);
    
      SAT_Print("..back from __libc_start_main\n");
      SAT_Print("calling _exit..\n");
      _exit(EXIT_SUCCESS);
    }

  } // extern c

#endif // SAT_PLUGIN_EXE_EXECUTABLE_LIBRARY
#endif // SAT_WIN32

//----------------------------------------------------------------------
#endif







































#if 0




#include "mip.h"

//#define MIP_EXECUTABLE_SHARED_LIBRARY
//-Wl,-e,entry_point


#ifdef MIP_PLUGIN
#ifdef MIP_EXECUTABLE_SHARED_LIBRARY

int main(int argc, char** argv, char** env);

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

extern "C" {

  int my_test_global = 123;

  int test_func() {
    printf("> test_func()\n");
    return 42;
  }

  // not called?
  int test_func_result = test_func();

  // not called?
  __attribute__((constructor))
  void ctor_test_func() {
    MIP_Print("> ctor_test_func()\n");
    test_func_result = 41;
  }

  int main_result;

  int* main_trampoline(int argc, char** argv, char** env) {
    MIP_Print("> main_trampoline()\n");
    MIP_Print("(my_test_global = %i)\n",my_test_global);
    MIP_Print("(test_func_result = %i)\n",test_func_result);
    MIP_Print("calling real main()..\n");
    main_result = main(argc,argv,env);
    MIP_Print("..and we're back..\n");
    return &main_result;
  }

}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*
  https://stackoverflow.com/questions/15265295/understanding-the-libc-init-array

  These symbols are related to the C / C++ constructor and destructor startup
  and tear down code that is called before / after main(). Sections named
  .init, .ctors, .preinit_array, and .init_array are to do with initialization
  of C/C++ objects, and sections .fini, .fini_array, and .dtors are for tear
  down. The start and end symbols define the beginning and end of code sections
  related to such operations and might be referenced from other parts of the
  runtime support code.

  The .preinit_array and .init_array sections contain arrays of pointers to
  functions that will be called on initialization. The .fini_array is an array
  of functions that will be called on destruction. Presumably the start and end
  labels are used to walk these lists.

  A good example of code that uses these symbols is to be found here
  http://static.grumpycoder.net/pixel/uC-sdk-doc/initfini_8c_source.html
  for initfini.c. You can see that on startup, __libc_init_array() is called
  and this first calls all the function pointers in section .preinit_array by
  referring to the start and end labels. Then it calls the _init() function in
  the .init section. Lastly it calls all the function pointers in section
  .init_array. After main() is complete the teardown call to
  __libc_fini_array() causes all the functions in .fini_array to be called,
  before finally calling _fini().

  Note that there seems to be a cut-and-paste bug in this code when it
  calculates the count of functions to call at teardown. Presumably they were
  dealing with a real time micro controller OS and never encountered this
  section.
*/

//----------

/*
  see also:
  https://refspecs.linuxbase.org/LSB_3.1.0/LSB-generic/LSB-generic/baselib---libc-start-main-.html
  --enable-initfini-array ??
*/

//----------

/*
  #ifdef SIXTY_FOUR_BIT
    const char my_interp[] __attribute__((section(".interp"))) = "/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2";
  #else
    const char my_interp[] __attribute__((section(".interp"))) = "/lib/ld-linux.so.2";
  #endif
*/

const char interp_section[] __attribute__((section(".interp"))) = "/lib64/ld-linux-x86-64.so.2";

#include <sys/types.h>

extern "C" {

  //------------------------------
  // __libc_init_array
  //------------------------------

  /*
    According to this comment from Nominal Animal here:
    stackoverflow.com/questions/32700494/executing-init-and-fini,
    using the attribute constructor would add the function in the .init_array,
    and it depends of the ABI; if you're on x86, it should work.
  */

  // https://stackoverflow.com/questions/13734745/why-do-i-have-an-undefined-reference-to-init-in-libc-init-array

  /*
  extern void _init (void);
  extern void (*__preinit_array_start[]) (void) __attribute__((weak));
  extern void (*__preinit_array_end[])   (void) __attribute__((weak));
  extern void (*__init_array_start[])    (void) __attribute__((weak));
  extern void (*__init_array_end[])      (void) __attribute__((weak));

  void __libc_init_array(void) {
    MIP_Print("* __libc_init_array()\n");
    MIP_Print("  (__preinit_array_start: %p)\n",__preinit_array_start);
    MIP_Print("  (__preinit_array_end: %p)\n",__preinit_array_end);
    MIP_Print("  (__init_array_start: %p)\n",__init_array_start);
    MIP_Print("  (__init_array_end: %p)\n",__init_array_end);
    size_t count;
    size_t i;
    count = __preinit_array_end - __preinit_array_start;
    MIP_Print("   count: %i\n",(int)count);
    for (i = 0; i < count; i++) __preinit_array_start[i]();
    _init();
    count = __init_array_end - __init_array_start;
    for (i = 0; i < count; i++) __init_array_start[i]();
  }
  */

  //------------------------------
  // __libc_csu_init
  //------------------------------

  /*
  extern void _init (void);
  extern void (*__preinit_array_start[]) (int argc, char **argv, char **envp) __attribute__((weak));
  extern void (*__preinit_array_end[])   (int argc, char **argv, char **envp) __attribute__((weak));
  extern void (*__init_array_start[])    (int argc, char **argv, char **envp) __attribute__((weak));
  extern void (*__init_array_end[])      (int argc, char **argv, char **envp) __attribute__((weak));

  void __libc_csu_init(int argc, char **argv, char **envp) {
    MIP_Print("* __libc_init_array()\n");
    MIP_Print("  (__preinit_array_start: %p)\n",__preinit_array_start);
    MIP_Print("  (__preinit_array_end: %p)\n",__preinit_array_end);
    MIP_Print("  (__init_array_start: %p)\n",__init_array_start);
    MIP_Print("  (__init_array_end: %p)\n",__init_array_end);
    size_t count;
    size_t i;
    count = __preinit_array_end - __preinit_array_start;
    MIP_Print("   preinit count: %i\n",(int)count);
    for (i=0; i<count; i++) (*__preinit_array_start[i])(argc, argv, envp);
    MIP_Print("  calling _init\n");
    _init ();
    count = __init_array_end - __init_array_start;
    MIP_Print("   init count: %i\n",(int)count);
    for (i=0; i<count; i++) (*__init_array_start [i]) (argc, argv, envp);
  }
  */

  //------------------------------
  // __libc_start_main
  //------------------------------

  extern int __libc_start_main(
    int  *(main)(int, char**, char**),
    int    argc,
    char** ubp_av,
    void (*init)(void),
    void (*fini)(void),
    void (*rtld_fini)(void),
    void (*stack_end)
  );

  //------------------------------
  // entry_point
  //------------------------------

  __attribute__((force_align_arg_pointer))
  void entry_point() {
    MIP_Print("* entry_point()\n");
    int argc = 0;
    const char* argv[] = { "", 0 };
    MIP_Print("> argc %i argv %s\n",argc,argv[0]);
    MIP_Print("calling __libc_start_main..\n");
    //__libc_start_main(main_trampoline,argc,(char**)argv,my_init,my_fini,my_rtld_fini,0);//&my_stack[500000]);

    MIP_REGISTRY.initialize();

    __libc_start_main(main_trampoline,argc,(char**)argv,nullptr,nullptr,nullptr,0);//&my_stack[500000]);
    MIP_Print("..back from __libc_start_main\n");
    MIP_Print("calling _exit..\n");
    _exit(EXIT_SUCCESS);
  }


}




#endif // MIP_EXECUTABLE_SHARED_LIBRARY
#endif // MIP_PLUGIN




#endif // 0

