#ifndef sat_exe_entry_included
#define sat_exe_entry_included
//----------------------------------------------------------------------

//#define SAT_PLUGIN_EXECUTABLE_LIBRARY

#include "base/utils/sat_arguments.h"
#include "plugin/sat_host_implementation.h"
#include "plugin/clap/sat_clap_entry.h"

#include "plugin/exe/sat_exe_host_implementation.h"
#include "plugin/exe/sat_exe_window.h"

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
    
    //printf("creating host window\n");
    SAT_ExeWindow* window = new SAT_ExeWindow(width,height,0,plugin,gui);
    //printf("host window created\n");
    
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
  
  int32_t plugin_index = 0;
  
  SAT_Arguments args;
  args.init(argc,argv);
  
  if (args.hasOption("-?") || args.hasOption("-h")) {
    printf("PLUGIN.EXE <optioins>\n");
    printf("  -l          list plugins\n");
    printf("  -i <index>  select plugin index (default 0)\n");
    return 0;
  }

  // list plugins
  
  if (args.hasOption("-l")) {
    // list plugins
    const clap_plugin_entry_t* entry = &clap_entry;
    if (!entry) { printf("! ERROR: clap_entry is null\n"); }
    else {
      const clap_plugin_factory_t* factory = (const clap_plugin_factory_t*)entry->get_factory(CLAP_PLUGIN_FACTORY_ID);
      if (!factory) { printf("! ERROR: Couldn't get factory from entry\n"); }
      else {
        uint32_t count = factory->get_plugin_count(factory);
        printf("Num plugins: %i\n",count);
        for (uint32_t i=0; i<count; i++) {
          const clap_plugin_descriptor_t* descriptor = factory->get_plugin_descriptor(factory,i);
          if (!descriptor) { printf("! ERROR: Couldn't get descriptor %i from factory\n",i); }
          else {
            printf("%i. %s\n",i,descriptor->name);
          }
        }
      }
    }
    return 0;
  }
  
  // select plugins

  if (args.hasOption("-i")) {
    plugin_index = args.getArgInt("-i");
    printf("index %i\n",plugin_index);
  }
  
  // instantiate

  SAT_ExeHostImplementation* hostimpl = new SAT_ExeHostImplementation(argc,argv);
  const clap_host_t* host = hostimpl->getHost();
  if (!host) { printf("! ERROR: Couldn't create exe host implementation\n"); }
  else {
    const clap_plugin_entry_t* entry = &clap_entry;
    if (!entry) { printf("! ERROR: clap_entry is null\n"); }
    else {
      
      //uint32_t count = entry->init(plugin_path);
      
      const clap_plugin_factory_t* factory = (const clap_plugin_factory_t*)entry->get_factory(CLAP_PLUGIN_FACTORY_ID);
      if (!factory) { printf("! ERROR: Couldn't get factory from entry\n"); }
      else {
        uint32_t count = factory->get_plugin_count(factory);
        if (count == 0) { printf("! ERROR: Plugin count is 0\n"); }
        else {
          const clap_plugin_descriptor_t* descriptor = factory->get_plugin_descriptor(factory,plugin_index);
          if (!descriptor) { printf("! ERROR: Couldn't get descriptor from factory\n"); }
          else {
            const char* plugin_id = descriptor->id;
            if (!plugin_id) { printf("! ERROR: plugin_id is null\n"); }
            else {
              const clap_plugin_t* plugin = factory->create_plugin(factory,host,plugin_id);
              if (!plugin) { printf("! ERROR: Couldn't create plugin from factory\n"); }
              else { 
                handle_plugin(plugin);
              } // plugin
            } // plugin_id
          } // descriptor
        } // count
      } // factory
      entry->deinit(); // entry->init() called?
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

  #include "plugin/exe/sat_exelib.h"

//----------------------------------------------------------------------
#endif
