#ifndef sat_hosted_clap_plugin_included
#define sat_hosted_clap_plugin_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/lib/sat_clap.h"
#include "plugin/host/sat_host_window.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

// struct SAT_ClapPluginExtensions {
//   const clap_plugin_ambisonic_t*                ambisonic                 = nullptr;
//   const clap_plugin_audio_ports_activation_t*   audio_ports_activation    = nullptr;
//   const clap_plugin_audio_ports_config_t*       audio_ports_config        = nullptr;
//   const clap_plugin_audio_ports_t*              audio_ports               = nullptr;
//   const clap_plugin_configurable_audio_ports_t* configurable_audio_ports  = nullptr;
//   const clap_plugin_context_menu_t*             context_menu              = nullptr;
//   const clap_plugin_gui_t*                      gui                       = nullptr;
//   const clap_plugin_latency_t*                  latency                   = nullptr;
//   const clap_plugin_note_name_t*                note_name                 = nullptr;
//   const clap_plugin_note_ports_t*               note_ports                = nullptr;
//   const clap_plugin_param_indication_t*         param_indication          = nullptr;
//   const clap_plugin_params_t*                   params                    = nullptr;
//   const clap_plugin_posix_fd_support_t*         posix_fd_support          = nullptr;
//   const clap_plugin_preset_load_t*              preset_load               = nullptr;
//   const clap_plugin_remote_controls_t*          remote_controls           = nullptr;
//   const clap_plugin_render_t*                   render                    = nullptr;
//   const clap_plugin_state_t*                    state                     = nullptr;
//   const clap_plugin_state_context_t*            state_context             = nullptr;
//   const clap_plugin_surround_t*                 surround                  = nullptr;
//   const clap_plugin_tail_t*                     tail                      = nullptr;
//   const clap_plugin_thread_pool_t*              thread_pool               = nullptr;
//   const clap_plugin_track_info_t*               track_info                = nullptr;
//   const clap_plugin_timer_support_t*            timer_support             = nullptr;
//   const clap_plugin_voice_info_t*               voice_info                = nullptr;
//   // draft
//   const clap_plugin_extensible_audio_ports_t*   extensible_audio_ports    = nullptr;
//   const clap_plugin_resource_directory_t*       resource_directory        = nullptr;
//   const clap_plugin_triggers_t*                 triggers                  = nullptr;
//   const clap_plugin_tuning_t*                   tuning                    = nullptr;
// };

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_HostedClapPlugin {

//------------------------------
private:
//------------------------------

  const clap_plugin_entry_t*      MClapEntry      = nullptr;
  const clap_plugin_factory_t*    MClapFactory    = nullptr;
  const clap_host_t*              MClapHost       = nullptr;
  const clap_plugin_descriptor_t* MClapDescriptor = nullptr;
  const clap_plugin_t*            MClapPlugin     = nullptr;
  const clap_plugin_gui_t*        MClapGui        = nullptr;

  SAT_HostWindow*                 MHostWindow     = nullptr;

  // SAT_ClapPluginExtensions        MExtensions     = {};

//------------------------------
public:
//------------------------------

  SAT_HostedClapPlugin() {
  }

  //----------

  virtual ~SAT_HostedClapPlugin() {
  }

//------------------------------
public:
//------------------------------

  void initExtensions() {
    // ext.ambisonic           = (clap_host_ambisonic_t*)MHost->get_extension(MHost,CLAP_EXT_AMBISONIC);
    // ext.audio_ports_config  = (clap_plugin_audio_ports_config_t*)MHost->get_extension(MHost,CLAP_EXT_AUDIO_PORTS_CONFIG);
    // ext.audio_ports         = (clap_host_audio_ports_t*)MHost->get_extension(MHost,CLAP_EXT_AUDIO_PORTS);
    // ext.context_menu        = (clap_host_context_menu_t*)MHost->get_extension(MHost,CLAP_EXT_CONTEXT_MENU);
    // ext.event_registry      = (clap_host_event_registry_t*)MHost->get_extension(MHost,CLAP_EXT_EVENT_REGISTRY);
    // ext.gui                 = (clap_host_gui_t*)MHost->get_extension(MHost,CLAP_EXT_GUI);
    // ext.latency             = (clap_host_latency_t*)MHost->get_extension(MHost,CLAP_EXT_LATENCY);
    // ext.log                 = (clap_host_log_t*)MHost->get_extension(MHost,CLAP_EXT_LOG);
    // ext.note_name           = (clap_host_note_name_t*)MHost->get_extension(MHost,CLAP_EXT_NOTE_NAME);
    // ext.note_ports          = (clap_host_note_ports_t*)MHost->get_extension(MHost,CLAP_EXT_NOTE_PORTS);
    // ext.params              = (clap_host_params_t*)MHost->get_extension(MHost,CLAP_EXT_PARAMS);
    // ext.posix_fd_support    = (clap_host_posix_fd_support_t*)MHost->get_extension(MHost,CLAP_EXT_POSIX_FD_SUPPORT);
    // ext.preset_load         = (clap_host_preset_load_t*)MHost->get_extension(MHost,CLAP_EXT_PRESET_LOAD);
    // ext.remote_controls     = (clap_host_remote_controls_t*)MHost->get_extension(MHost,CLAP_EXT_REMOTE_CONTROLS);
    // ext.state               = (clap_host_state_t*)MHost->get_extension(MHost,CLAP_EXT_STATE);
    // ext.surround            = (clap_host_surround_t*)MHost->get_extension(MHost,CLAP_EXT_SURROUND);
    // ext.tail                = (clap_host_tail_t*)MHost->get_extension(MHost,CLAP_EXT_TAIL);
    // ext.thread_check        = (clap_host_thread_check_t*)MHost->get_extension(MHost,CLAP_EXT_THREAD_CHECK);
    // ext.thread_pool         = (clap_host_thread_pool_t*)MHost->get_extension(MHost,CLAP_EXT_THREAD_POOL);
    // ext.timer_support       = (clap_host_timer_support_t*)MHost->get_extension(MHost,CLAP_EXT_TIMER_SUPPORT);
    // ext.track_info          = (clap_host_track_info_t*)MHost->get_extension(MHost,CLAP_EXT_TRACK_INFO);
    
    // ext.resource_directory  = (clap_host_resource_directory_t*)MHost->get_extension(MHost,CLAP_EXT_RESOURCE_DIRECTORY);
    // ext.transport_control   = (clap_host_transport_control_t*)MHost->get_extension(MHost,CLAP_EXT_TRANSPORT_CONTROL);
    // ext.triggers            = (clap_host_triggers_t*)MHost->get_extension(MHost,CLAP_EXT_TRIGGERS);
    // ext.tuning              = (clap_host_tuning_t*)MHost->get_extension(MHost,CLAP_EXT_TUNING);
  }

  //----------

  virtual bool loadFromMemory(const clap_plugin_entry_t* AClapEntry, const char* APath="", uint32_t AIndex=0) {
    MClapEntry = AClapEntry;
    MClapEntry->init(APath);
    MClapFactory = (clap_plugin_factory_t*)MClapEntry->get_factory(CLAP_PLUGIN_FACTORY_ID);
    MClapDescriptor = MClapFactory->get_plugin_descriptor(MClapFactory,AIndex);
    return false;
  }

  //----------

  virtual bool loadFromFile(const char* APath, uint32_t AIndex) {
    return false;
  }

  //----------

  virtual void unload() {
  }

//------------------------------
public:
//------------------------------

  virtual bool createInstance(const clap_host_t* AHost) {
    MClapPlugin = MClapFactory->create_plugin(MClapFactory,AHost,MClapDescriptor->id);
    MClapPlugin->init(MClapPlugin);
    MClapGui = (clap_plugin_gui_t*)MClapPlugin->get_extension(MClapPlugin,CLAP_EXT_GUI);
    return false;
  }

  //----------

  virtual bool destroyInstance() {
    MClapPlugin->destroy(MClapPlugin);
  }

  //----------

  virtual bool initInstance() {
    return false;
  }

  //----------

  virtual bool openEditor(SAT_HostWindow* AHostWindow) {
    MHostEditor = AHostEditor;

    #ifdef SAT_LINUX
      MClapGui->create(MClapPlugin,CLAP_WINDOW_API_X11,false);
    #endif
    #ifdef SAT_WIN32
      MClapGui->create(plugin,CLAP_WINDOW_API_WIN32,false);
    #endif
    
    MClapGui->set_scale(MClapPlugin,1.0);
    uint32_t width,height;
    MClapGui->get_size(MClapPlugin,&width,&height);

    #ifdef SAT_LINUX
      xcb_window_t x11window = AHostWindow->getXcbWindow();
      const clap_window_t clapwindow = { .api = CLAP_WINDOW_API_X11, .x11 = x11window };
    #endif
    #ifdef SAT_WIN32
      HWND win32window = HostWindow->getWin32Window();
      const clap_window_t clapwindow = { .api = CLAP_WINDOW_API_WIN32, .win32 = win32window };
    #endif

    MClapGui->set_parent(MClapPlugin,&clapwindow);
    MClapGui->show(MClapPlugin);
    MClapGui->set_size(MClapPlugin,width,height); // is this needed?

    return false;
  }

  //----------

  virtual void closeEditor() {
    MHostEditor = nullptr;
    MClapGui->hide(MClapPlugin);
    MClapGui->destroy(MClapPlugin);
    //SAT_PRINT("editor closed\n");
  }

  //----------

  void getEditorSize(uint32_t* width, uint32_t* height) {
    MClapGui->get_size(MClapPlugin,width,height);
  }

  //----------

  void mainloop(SAT_HostWindow* AHostWindow) {
    MHostWindow->eventLoop();
  }


};

//----------------------------------------------------------------------
#endif

