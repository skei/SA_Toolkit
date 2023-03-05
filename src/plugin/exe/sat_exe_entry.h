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

class SAT_ExeWindow
: public SAT_ImplementedWindow {

private:

  const clap_plugin_t*      MPlugin = nullptr;
  const clap_plugin_gui_t*  MGui    = nullptr;

public:

  SAT_ExeWindow(uint32_t AWidth, uint32_t AHeight, intptr_t AParent, const clap_plugin_t* APlugin, const clap_plugin_gui_t* AGui)
  : SAT_ImplementedWindow(AWidth,AHeight,AParent) {
    MPlugin = APlugin;
    MGui = AGui;
  }

  virtual ~SAT_ExeWindow() {
  }

public:

  void on_window_open() final {
    //SAT_PRINT;
  }

  void on_window_close() final {
    //SAT_PRINT;
  }

  void on_window_resize(int32_t AWidth, int32_t AHeight) final {
    //SAT_PRINT;
    MGui->set_size(MPlugin,AWidth,AHeight);
  }

  //void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) final {
  //  SAT_PRINT;
  //}

  void on_window_client_message(uint32_t AData) final {
    //SAT_Print("%i\n",AData);
  }

};

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

/*
  Showing the GUI works as follow:
    1.  clap_plugin_gui->is_api_supported(), check what can work
    2.  clap_plugin_gui->create(), allocates gui resources
    3.  if the plugin window is floating
    4.     -> clap_plugin_gui->set_transient()
    5.     -> clap_plugin_gui->suggest_title()
    6.  else
    7.     -> clap_plugin_gui->set_scale()
    8.     -> clap_plugin_gui->can_resize()
    9.     -> if resizable and has known size from previous session, clap_plugin_gui->set_size()
    10.    -> else clap_plugin_gui->get_size(), gets initial size
    11.    -> clap_plugin_gui->set_parent()
    12. clap_plugin_gui->show()
    13. clap_plugin_gui->hide()/show() ...
    14. clap_plugin_gui->destroy() when done with the gui
*/

void open_editor(const clap_plugin_t* plugin) {
  const clap_plugin_gui_t* gui = (const clap_plugin_gui_t*)plugin->get_extension(plugin,CLAP_EXT_GUI);
  if (gui) {
    uint32_t width,height;
    gui->create(plugin,CLAP_WINDOW_API_X11,false);
    gui->set_scale(plugin,1.0);
    gui->get_size(plugin,&width,&height);
    SAT_ExeWindow* window = new SAT_ExeWindow(width,height,0,plugin,gui);
    xcb_window_t x11window = window->getX11Window();
    const clap_window_t clapwindow = { .api = CLAP_WINDOW_API_X11, .x11 = x11window };
    gui->set_parent(plugin,&clapwindow);
    gui->show(plugin);
    gui->set_size(plugin,width,height);
    window->show();

    window->eventLoop();

    gui->hide(plugin);
    gui->destroy(plugin);
    window->hide();
    delete window;
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
  open_editor(plugin);
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

  uint32_t    plugin_index      = 0;
  //const char* plugin_path       = "";
  //double      sample_rate       = 44100;
  //uint32_t    min_frames_count  = 128;
  //uint32_t    max_frames_count  = 128;

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
      //uint32_t count = entry->init(plugin_path);
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
