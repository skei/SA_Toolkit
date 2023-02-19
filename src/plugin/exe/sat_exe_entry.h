#ifndef sat_exe_entry_included
#define sat_exe_entry_included
//----------------------------------------------------------------------

#include "plugin/sat_host_implementation.h"
#include "plugin/clap/sat_clap_entry.h"

//----------------------------------------------------------------------
//
//
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

void open_editor(const clap_plugin_t* plugin) {
  const clap_plugin_gui_t* gui = (const clap_plugin_gui_t*)plugin->get_extension(plugin,CLAP_EXT_GUI);
  if (gui) {
    SAT_Print("plugin has gui\n");

    uint32_t width,height;
    gui->create(plugin,CLAP_WINDOW_API_X11,false);
    gui->get_size(plugin,&width,&height);
    gui->set_scale(plugin,1.0);
    //gui->set_size(plugin,width,height);

//    SAT_ImplementedWindow* window = new SAT_ImplementedWindow(width,height,0);
//    xcb_window_t x11window = window->getX11Window();
//    const clap_window_t clapwindow = {
//      .api = CLAP_WINDOW_API_X11,
//      .x11 = x11window
//    };
//    gui->set_parent(plugin,&clapwindow);

    gui->show(plugin);

    //SAT_Sleep(1000);

    gui->hide(plugin);
    gui->destroy(plugin);

//    delete window;

  }
}

//----------------------------------------------------------------------
//
// handle plugin
//
//----------------------------------------------------------------------

void handle_plugin(const clap_plugin_t* plugin) {
  plugin->init(plugin);
  plugin->activate(plugin,44100,128,128);
  plugin->start_processing(plugin);

  clap_process_t process = {0};
    process.audio_inputs        = nullptr;
    process.audio_inputs_count  = 0;
    process.audio_outputs       = nullptr;
    process.audio_outputs_count = 0;
    process.frames_count        = 128;
    process.in_events           = nullptr;
    process.out_events          = nullptr;
    process.steady_time         = 0;
    process.transport           = nullptr;
  //plugin->process(plugin,&process); // will probably crash..

  open_editor(plugin);

  plugin->stop_processing(plugin);
  plugin->deactivate(plugin);
  plugin->destroy(plugin);
}

//----------------------------------------------------------------------
//
// main
//
//----------------------------------------------------------------------

int main(int argc, char** argv) {

  uint32_t    plugin_index      = 0;
  const char* plugin_path       = "";

  double      sample_rate       = 44100;
  uint32_t    min_frames_count  = 128;
  uint32_t    max_frames_count  = 128;

  //SAT_HostImplementation* hostimpl = new SAT_HostImplementation();
  SAT_ExeHostImplementation* hostimpl = new SAT_ExeHostImplementation(argc,argv);

  const clap_host_t* host = hostimpl->getHost();
  if (!host) {
    SAT_Print("! ERROR: Couldn't create exe host implementation\n");
  }
  else {
    const clap_plugin_entry_t* entry = &clap_entry;
    if (!entry) {
      SAT_Print("! ERROR: clap_entry is null\n");
    }
    else {
      uint32_t count = entry->init(plugin_path);
      const clap_plugin_factory_t* factory = (const clap_plugin_factory_t*)entry->get_factory(CLAP_PLUGIN_FACTORY_ID);
      if (!factory) {
        SAT_Print("! ERROR: Couldn't get factory from entry\n");
      }
      else {
        uint32_t count = factory->get_plugin_count(factory);
        if (count == 0) {
          SAT_Print("! ERROR: Plugin count is 0\n");
        }
        else {
          const clap_plugin_descriptor_t* descriptor = factory->get_plugin_descriptor(factory,plugin_index);
          if (!descriptor) {
            SAT_Print("! ERROR: Couldn't get descriptor from factory\n");
          }
          else {
            const char* plugin_id = descriptor->id;
            if (!plugin_id) {
              SAT_Print("! ERROR: plugin_id is null\n");
            }
            else {
              const clap_plugin_t* plugin = factory->create_plugin(factory,host,plugin_id);
              if (!plugin) {
                SAT_Print("! ERROR: Couldn't create plugin from factory\n");
              }
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
#endif
