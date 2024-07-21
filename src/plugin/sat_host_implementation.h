#ifndef sat_host_implementation_included
#define sat_host_implementation_included
//----------------------------------------------------------------------
// the host ptr you give to hosted plugins

#include "sat.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/clap/sat_clap_host_implementation.h"
//#include "plugin/host/sat_host_window.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_HostImplementation 
: public SAT_ClapHostImplementation {

//------------------------------
public:
//------------------------------

  SAT_HostImplementation()
  : SAT_ClapHostImplementation() {

    MClapHost.name = "SAT_HostImplementation";

  };

  //----------

  virtual ~SAT_HostImplementation() {
  }

//------------------------------
public:
//------------------------------

  bool loadPlugin(const char* APath) {
    return false;
    // dlopen, dlsym, entry_point.. (see host in exe_plugin)..
  }

  //----------

  void unloadPlugin() {
  }

//------------------------------
public: // plugin
//------------------------------

  const void* get_extension(const char *extension_id) override {
    //SAT_Print("id %s\n",extension_id);
    if (strcmp(extension_id,CLAP_EXT_AMBISONIC)           == 0) return &MAmbisonicExt;
    if (strcmp(extension_id,CLAP_EXT_AUDIO_PORTS)         == 0) return &MAudioPortsExt;
    if (strcmp(extension_id,CLAP_EXT_AUDIO_PORTS_CONFIG)  == 0) return &MAudioPortsConfigExt;
    if (strcmp(extension_id,CLAP_EXT_CONTEXT_MENU)        == 0) return &MContextMenuExt;
    if (strcmp(extension_id,CLAP_EXT_EVENT_REGISTRY)      == 0) return &MEventRegistryExt;
    if (strcmp(extension_id,CLAP_EXT_GUI)                 == 0) return &MGuiExt;
    if (strcmp(extension_id,CLAP_EXT_LATENCY)             == 0) return &MLatencyExt;
    if (strcmp(extension_id,CLAP_EXT_LOG)                 == 0) return &MLogExt;
    if (strcmp(extension_id,CLAP_EXT_NOTE_NAME)           == 0) return &MNoteNameExt;
    if (strcmp(extension_id,CLAP_EXT_NOTE_PORTS)          == 0) return &MNotePortsExt;
    if (strcmp(extension_id,CLAP_EXT_PARAMS)              == 0) return &MParamsExt;
    if (strcmp(extension_id,CLAP_EXT_POSIX_FD_SUPPORT)    == 0) return &MPosixFdSupportExt;
    if (strcmp(extension_id,CLAP_EXT_PRESET_LOAD)         == 0) return &MPresetLoadExt;
    if (strcmp(extension_id,CLAP_EXT_REMOTE_CONTROLS)     == 0) return &MRemoteControlsExt;
    if (strcmp(extension_id,CLAP_EXT_STATE)               == 0) return &MStateExt;
    if (strcmp(extension_id,CLAP_EXT_SURROUND)            == 0) return &MSurroundExt;
    if (strcmp(extension_id,CLAP_EXT_TAIL)                == 0) return &MTailExt;
    if (strcmp(extension_id,CLAP_EXT_THREAD_CHECK)        == 0) return &MThreadCheckExt;
    if (strcmp(extension_id,CLAP_EXT_THREAD_POOL)         == 0) return &MThreadPoolExt;
    if (strcmp(extension_id,CLAP_EXT_TIMER_SUPPORT)       == 0) return &MTimerSupportExt;
    if (strcmp(extension_id,CLAP_EXT_TRACK_INFO)          == 0) return &MTrackInfoExt;
    if (strcmp(extension_id,CLAP_EXT_VOICE_INFO)          == 0) return &MVoiceInfoExt;
    // drafts
    if (strcmp(extension_id,CLAP_EXT_RESOURCE_DIRECTORY)  == 0) return &MResourceDirectoryExt;
    if (strcmp(extension_id,CLAP_EXT_TRANSPORT_CONTROL)   == 0) return &MTransportControlExt;
    if (strcmp(extension_id,CLAP_EXT_TRIGGERS)            == 0) return &MTriggersExt;
    if (strcmp(extension_id,CLAP_EXT_TUNING)              == 0) return &MTuningExt;

    return nullptr;
  }

  void request_restart() override {
  }

  void request_process() override {
  }

  void request_callback() override {
  }

//------------------------------
public: // extensions
//------------------------------

  void ambisonic_changed() override {
  }
  
  bool audio_ports_is_rescan_flag_supported(uint32_t flag) override {
    return false;
  }

  void audio_ports_rescan(uint32_t flags) override {
  }

  void audio_ports_config_rescan() override {
  }
  
  bool context_menu_populate(const clap_context_menu_target_t *target, const clap_context_menu_builder_t *builder) override {
    return false;
  }
  
  bool context_menu_perform(const clap_context_menu_target_t *target, clap_id action_id) override {
    return false;
  }
  
  bool context_menu_can_popup() override {
    return false;
  }
  
  bool context_menu_popup(const clap_context_menu_target_t *target, int32_t screen_index, int32_t x, int32_t y) override {
    return false;
  }
  
  bool event_registry_query(const char *space_name, uint16_t *space_id) override {
    return false;
  }
  
  void gui_resize_hints_changed() override {
  }
  
  bool gui_request_resize(uint32_t width, uint32_t height) override {
    return false;
  }
  
  bool gui_request_show() override {
    return false;
  }
  
  bool gui_request_hide() override {
    return false;
  }
  
  void gui_closed(bool was_destroyed) override {
  }
  
  void latency_changed() override {
  }
  
  void log_log(clap_log_severity severity, const char *msg) override {
  }
  
  void note_name_changed() override {
  }
  
  uint32_t note_ports_supported_dialects() override {
    return 0;
  }

  void note_ports_rescan(uint32_t flags) override {
  }
  
  void params_rescan(clap_param_rescan_flags flags) override {
  }
  
  void params_clear(clap_id param_id, clap_param_clear_flags flags) override {
  }
  
  void params_request_flush() override {
  }
  
  bool posix_fd_support_register_fd(int fd, clap_posix_fd_flags_t flags) override {
    return false;
  }
  
  bool posix_fd_support_modify_fd(int fd, clap_posix_fd_flags_t flags) override {
    return false;
  }
  
  bool posix_fd_support_unregister_fd(int fd) override {
    return false;
  }
  
  void preset_load_on_error(uint32_t location_kind, const char *location, const char *load_key,int32_t os_error, const char *msg) override {
  }
  
  void preset_load_loaded(uint32_t location_kind, const char *location, const char *load_key) override {
  }
  
  void remote_controls_changed() override {
  }
  
  void remote_controls_suggest_page(clap_id page_id) override {
  }
  
  void state_mark_dirty() override {
  }
  
  void surround_changed() override {
  }
  
  void tail_changed() override {
  }
  
  bool thread_check_is_main_thread() override {
    return false;
  }
  
  bool thread_check_is_audio_thread() override {
    return false;
  }
  
  bool thread_pool_request_exec(uint32_t num_tasks) override {
    return false;
  }
  
  bool timer_support_register_timer(uint32_t period_ms, clap_id *timer_id) override {
    return false;
  }
  
  bool timer_support_unregister_timer(clap_id timer_id) override {
    return false;
  }
  
  bool track_info_get(clap_track_info_t *info) override {
    return false;
  }
  
  void voice_info_changed() override {
  }
  

//------------------------------
public: // drafts
//------------------------------

  bool resource_directory_request_directory(bool is_shared) override {
    return false;
  }
  
  void resource_directory_release_directory(bool is_shared) override {
  }
  
  void transport_control_request_start() override {
  }
  
  void transport_control_request_stop() override {
  }
  
  void transport_control_request_continue() override {
  }
  
  void transport_control_request_pause() override {
  }
  
  void transport_control_request_toggle_play() override {
  }
  
  void transport_control_request_jump(clap_beattime position) override {
  }
  
  void transport_control_request_loop_region(clap_beattime start, clap_beattime duration) override {
  }
  
  void transport_control_request_toggle_loop() override {
  }
  
  void transport_control_request_enable_loop(bool is_enabled) override {
  }
  
  void transport_control_request_record(bool is_recording) override {
  }
  
  void transport_control_request_toggle_record() override {
  }
  
  void triggers_rescan(clap_trigger_rescan_flags flags) override {
  }
  
  void triggers_clear(clap_id trigger_id, clap_trigger_clear_flags flags) override {
  }
  
  double tuning_get_relative(clap_id tuning_id, int32_t channel, int32_t key, uint32_t sample_offset) override {
    return 0.0;
  }

  bool tuning_should_play(clap_id tuning_id, int32_t channel, int32_t key) override {
    return false;
  }
  
  uint32_t tuning_get_tuning_count() override {
    return 0;
  }

  bool tuning_get_info(uint32_t tuning_index, clap_tuning_info_t *info) override {
    return false;
  }
};

//----------------------------------------------------------------------
#endif