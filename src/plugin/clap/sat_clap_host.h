#ifndef sat_clap_host_included
#define sat_clap_host_included
//----------------------------------------------------------------------
// host from a plugin perspective

#include "plugin/clap/sat_clap.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

struct SAT_ClapHostExtensions {
  const clap_host_ambisonic_t*                ambisonic               = nullptr;
  const clap_host_audio_ports_t*              audio_ports             = nullptr;
  const clap_host_audio_ports_config_t*       audio_ports_config      = nullptr;
  const clap_host_context_menu_t*             context_menu            = nullptr;
  const clap_host_event_registry_t*           event_registry          = nullptr;
  const clap_host_gui_t*                      gui                     = nullptr;
  const clap_host_latency_t*                  latency                 = nullptr;
  const clap_host_log_t*                      log                     = nullptr;
  const clap_host_note_name_t*                note_name               = nullptr;
  const clap_host_note_ports_t*               note_ports              = nullptr;
  const clap_host_params_t*                   params                  = nullptr;
  const clap_host_posix_fd_support_t*         posix_fd_support        = nullptr;
  const clap_host_preset_load_t*              preset_load             = nullptr;
  const clap_host_remote_controls_t*          remote_controls         = nullptr;
  const clap_host_state_t*                    state                   = nullptr;
  const clap_host_surround_t*                 surround                = nullptr;
  const clap_host_tail_t*                     tail                    = nullptr;
  const clap_host_thread_check_t*             thread_check            = nullptr;
  const clap_host_thread_pool_t*              thread_pool             = nullptr;
  const clap_host_timer_support_t*            timer_support           = nullptr;
  const clap_host_track_info_t*               track_info              = nullptr;
  const clap_host_voice_info_t*               voice_info              = nullptr;
  // draft
  const clap_host_mini_curve_display_t*       mini_curve_display      = nullptr;
  const clap_host_resource_directory_t*       resource_directory      = nullptr;
  const clap_host_scratch_memory_t*           scratch_memory          = nullptr;
  const clap_host_transport_control_t*        transport_control       = nullptr;
  const clap_host_triggers_t*                 triggers                = nullptr;
  const clap_host_tuning_t*                   tuning                  = nullptr;
  const clap_host_undo_t*                     undo                    = nullptr;
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_ClapHost {

//------------------------------
private:
//------------------------------

  const clap_host_t* MHost = nullptr;

//------------------------------
public:
//------------------------------

  SAT_ClapHostExtensions ext = {};

//------------------------------
public:
//------------------------------

  SAT_ClapHost(const clap_host_t* AHost) {
    //SAT_Assert(AHost);
    MHost = AHost;
    initExtensions();
  };

  //----------

  virtual ~SAT_ClapHost() {
  }

//------------------------------
public:
//------------------------------

  virtual const clap_host_t*  getHost()     { return MHost; }
  virtual const char*         getName()     { return MHost->name; }
  virtual const char*         getVendor()   { return MHost->vendor; }
  virtual const char*         getUrl()      { return MHost->url; }
  virtual const char*         getVersion()  { return MHost->version; }

//------------------------------
public:
//------------------------------

  // Query an extension.
  // The returned pointer is owned by the host.
  // It is forbidden to call it before plugin->init().
  // You can call it within plugin->init() call, and after.
  // [thread-safe]

  virtual const void* get_extension(const char *extension_id) {
    return MHost->get_extension(MHost,extension_id);
  }

  //----------

  // Request the host to deactivate and then reactivate the plugin.
  // The operation may be delayed by the host.
  // [thread-safe]

  virtual void request_restart() {
    MHost->request_restart(MHost);
  }

  //----------

  // Request the host to activate and start processing the plugin.
  // This is useful if you have external IO and need to wake up the plugin from "sleep".
  // [thread-safe]

  virtual void request_process() {
    MHost->request_process(MHost);
  }

  //----------

  // Request the host to schedule a call to plugin->on_main_thread(plugin) on the main thread.
  // [thread-safe]

  virtual void request_callback() {
    MHost->request_callback(MHost);
  }

//------------------------------
private:
//------------------------------

//------------------------------
private:
//------------------------------

  void initExtensions() {
    //SAT_PRINT;
    ext.ambisonic           = (clap_host_ambisonic_t*)MHost->get_extension(MHost,CLAP_EXT_AMBISONIC);
    ext.audio_ports         = (clap_host_audio_ports_t*)MHost->get_extension(MHost,CLAP_EXT_AUDIO_PORTS);
    ext.audio_ports_config  = (clap_host_audio_ports_config_t*)MHost->get_extension(MHost,CLAP_EXT_AUDIO_PORTS_CONFIG);
    ext.context_menu        = (clap_host_context_menu_t*)MHost->get_extension(MHost,CLAP_EXT_CONTEXT_MENU);
    ext.event_registry      = (clap_host_event_registry_t*)MHost->get_extension(MHost,CLAP_EXT_EVENT_REGISTRY);
    ext.gui                 = (clap_host_gui_t*)MHost->get_extension(MHost,CLAP_EXT_GUI);
    ext.latency             = (clap_host_latency_t*)MHost->get_extension(MHost,CLAP_EXT_LATENCY);
    ext.log                 = (clap_host_log_t*)MHost->get_extension(MHost,CLAP_EXT_LOG);
    ext.note_name           = (clap_host_note_name_t*)MHost->get_extension(MHost,CLAP_EXT_NOTE_NAME);
    ext.note_ports          = (clap_host_note_ports_t*)MHost->get_extension(MHost,CLAP_EXT_NOTE_PORTS);
    ext.params              = (clap_host_params_t*)MHost->get_extension(MHost,CLAP_EXT_PARAMS);
    ext.posix_fd_support    = (clap_host_posix_fd_support_t*)MHost->get_extension(MHost,CLAP_EXT_POSIX_FD_SUPPORT);
    ext.preset_load         = (clap_host_preset_load_t*)MHost->get_extension(MHost,CLAP_EXT_PRESET_LOAD);
    ext.remote_controls     = (clap_host_remote_controls_t*)MHost->get_extension(MHost,CLAP_EXT_REMOTE_CONTROLS);
    ext.state               = (clap_host_state_t*)MHost->get_extension(MHost,CLAP_EXT_STATE);
    ext.surround            = (clap_host_surround_t*)MHost->get_extension(MHost,CLAP_EXT_SURROUND);
    ext.tail                = (clap_host_tail_t*)MHost->get_extension(MHost,CLAP_EXT_TAIL);
    ext.thread_check        = (clap_host_thread_check_t*)MHost->get_extension(MHost,CLAP_EXT_THREAD_CHECK);
    ext.thread_pool         = (clap_host_thread_pool_t*)MHost->get_extension(MHost,CLAP_EXT_THREAD_POOL);
    ext.timer_support       = (clap_host_timer_support_t*)MHost->get_extension(MHost,CLAP_EXT_TIMER_SUPPORT);
    ext.track_info          = (clap_host_track_info_t*)MHost->get_extension(MHost,CLAP_EXT_TRACK_INFO);
    ext.voice_info          = (clap_host_voice_info_t*)MHost->get_extension(MHost,CLAP_EXT_VOICE_INFO);
    // draft

    ext.mini_curve_display  = (clap_host_mini_curve_display_t*)MHost->get_extension(MHost,CLAP_EXT_MINI_CURVE_DISPLAY);
    ext.resource_directory  = (clap_host_resource_directory_t*)MHost->get_extension(MHost,CLAP_EXT_RESOURCE_DIRECTORY);
    ext.scratch_memory      = (clap_host_scratch_memory_t*)MHost->get_extension(MHost,CLAP_EXT_SCRATCH_MEMORY);
    ext.transport_control   = (clap_host_transport_control_t*)MHost->get_extension(MHost,CLAP_EXT_TRANSPORT_CONTROL);
    ext.triggers            = (clap_host_triggers_t*)MHost->get_extension(MHost,CLAP_EXT_TRIGGERS);
    ext.tuning              = (clap_host_tuning_t*)MHost->get_extension(MHost,CLAP_EXT_TUNING);
    ext.undo                = (clap_host_undo_t*)MHost->get_extension(MHost,CLAP_EXT_UNDO);

  }

};

//----------------------------------------------------------------------
#endif