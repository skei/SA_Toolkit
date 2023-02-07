#ifndef sat_clap_host_implementation_included
#define sat_clap_host_implementation_included
//----------------------------------------------------------------------

// host from host perspective
// (a host implementation)

#include "base/sat.h"
#include "plugin/clap/sat_clap.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_ClapHostImplementation {

//------------------------------
public:
//------------------------------

  SAT_ClapHostImplementation() {
  }

  //----------

  virtual ~SAT_ClapHostImplementation() {
  }

//------------------------------
public:
//------------------------------

  const clap_host_t* getHost() {
    return &MHost;
  }

//------------------------------
public:
//------------------------------

  virtual const void* get_extension(const char *extension_id) {
    //SAT_Print("id %s\n",extension_id);

//    if (strcmp(extension_CLAP_EXT_AMBISONIC)              == 0) return &MAmbisonicExt;
    if (strcmp(extension_id,CLAP_EXT_AUDIO_PORTS)         == 0) return &MAudioPortsExt;
    if (strcmp(extension_id,CLAP_EXT_AUDIO_PORTS_CONFIG)  == 0) return &MAudioPortsExt;
//    if (strcmp(extension_CLAP_EXT_CHECK_FOR_UPDATE)       == 0) return &MCheckForUpdateExt;
//    if (strcmp(extension_CLAP_EXT_CV)                     == 0) return &MCVExt;
//    if (strcmp(extension_CLAP_EXT_EVENT_REGISTRY)         == 0) return &MEventRegistryExt;
//    if (strcmp(extension_CLAP_EXT_GUI)                    == 0) return &MGuiExt;
//    if (strcmp(extension_CLAP_EXT_LATENCY)                == 0) return &MLatencyExt;
//    if (strcmp(extension_CLAP_EXT_MIDI_MAPPINGS)          == 0) return &MMidiMappingsExt;
//    if (strcmp(extension_CLAP_EXT_NOTE_NAME)              == 0) return &MNoteNameExt;
//    if (strcmp(extension_CLAP_EXT_NOTE_PORTS)             == 0) return &MNotePortsExt;
//    if (strcmp(extension_CLAP_EXT_PARAMS)                 == 0) return &MParamsExt;
//    if (strcmp(extension_CLAP_EXT_POSIX_FD_SUPPORT)       == 0) return &MPosixFdSupportExt;
//    if (strcmp(extension_CLAP_EXT_PRESET_LOAD)            == 0) return &MPresetLoadExt;
//    if (strcmp(extension_CLAP_EXT_REMOTE_CONTROLS)        == 0) return &MRemoteControlsExt;
//    if (strcmp(extension_CLAP_EXT_RESOURCE_CIRECTORY)     == 0) return &MResourceDirectoryExt;
//    if (strcmp(extension_CLAP_EXT_STATE)                  == 0) return &MStateExt;
//    if (strcmp(extension_CLAP_EXT_STATE_CONTEXT)          == 0) return &MStateContextExt;
//    if (strcmp(extension_CLAP_EXT_SURROUND)               == 0) return &MSurroundExt;
//    if (strcmp(extension_CLAP_EXT_TAIL)                   == 0) return &MTailExt;
//    if (strcmp(extension_CLAP_EXT_THREAD_POOL)            == 0) return &MThreadPoolExt;
//    if (strcmp(extension_CLAP_EXT_TIMER_SUPPORT)          == 0) return &MTimerSupportExt;
//    if (strcmp(extension_CLAP_EXT_TRACK_INFO)             == 0) return &MTrackInfoExt;
//    if (strcmp(extension_CLAP_EXT_TRANSPORT_CONTROLS)     == 0) return &MTransportControlsExt;
//    if (strcmp(extension_CLAP_EXT_TRIGGERS)               == 0) return &MTriggersExt;
//    if (strcmp(extension_CLAP_EXT_TUNING)                 == 0) return &MTuningExt;
//    if (strcmp(extension_CLAP_EXT_VOICE_INFO)             == 0) return &MVoiceInfoExt;
    return nullptr;
  }

  //----------

  virtual void request_restart() {
  }

  //----------

  virtual void request_process() {
  }

  //----------

  virtual void request_callback() {
  }

//------------------------------
public: // extensions
//------------------------------

  virtual void      ambisonic_changed() {}
  virtual bool      audio_ports_is_rescan_flag_supported(uint32_t flag) { return false; }
  virtual void      audio_ports_rescan(uint32_t flags) {}
  virtual void      audio_ports_config_rescan() {}
  virtual void      check_for_update_on_new_version(const clap_check_for_update_info_t *update_info) {}
  virtual void      cv_changed() {}
  virtual bool      event_registry_query(const char *space_name, uint16_t *space_id) { return false; }
  virtual void      file_reference_changed() {}
  virtual void      file_reference_set_dirty(clap_id resource_id) {}
  virtual void      gui_resize_hints_changed() {}
  virtual bool      gui_request_resize(uint32_t width, uint32_t height) { return false; }
  virtual bool      gui_request_show() { return false; }
  virtual bool      gui_request_hide() { return false; }
  virtual void      gui_closed(bool was_destroyed) {}
  virtual void      latency_changed() {}
  virtual void      log_log(clap_log_severity severity, const char *msg) {}
  virtual void      note_name_changed() {}
  virtual uint32_t  note_ports_supported_dialects() { return 0; }
  virtual void      note_ports_rescan(uint32_t flags) {}
  virtual void      params_rescan(clap_param_rescan_flags flags) {}
  virtual void      params_clear(clap_id param_id, clap_param_clear_flags flags) {}
  virtual void      params_request_flush() {}
  virtual void      midi_mappings_changed() {}
  virtual bool      posix_fd_support_register_fd(int fd, clap_posix_fd_flags_t flags) { return false; }
  virtual bool      posix_fd_support_modify_fd(int fd, clap_posix_fd_flags_t flags) { return false; }
  virtual bool      posix_fd_support_unregister_fd(int fd) { return false; }
  virtual void      quick_controls_changed() {}
  virtual void      quick_controls_suggest_page(clap_id page_id) {}
  virtual void      state_mark_dirty() {}
  virtual void      surround_changed() {}
  virtual void      surround_get_preferred_channel_map(uint8_t *channel_map, uint32_t channel_map_capacity, uint32_t *channel_count) {}
  virtual void      tail_changed() {}
  virtual bool      thread_check_is_main_thread() { return false; }
  virtual bool      thread_check_is_audio_thread() { return false; }
  virtual bool      thread_pool_request_exec(uint32_t num_tasks) { return false; }
  virtual bool      timer_support_register_timer(uint32_t period_ms, clap_id *timer_id) { return false; }
  virtual bool      timer_support_unregister_timer(clap_id timer_id) { return false; }
  virtual bool      track_info_get(clap_track_info_t *info) { return false; }
  virtual void      transport_control_request_start() {}
  virtual void      transport_control_request_stop() {}
  virtual void      transport_control_request_continue() {}
  virtual void      transport_control_request_pause() {}
  virtual void      transport_control_request_toggle_play() {}
  virtual void      transport_control_request_jump(clap_beattime position) {}
  virtual void      transport_control_request_loop_region(clap_beattime start, clap_beattime duration) {}
  virtual void      transport_control_request_toggle_loop() {}
  virtual void      transport_control_request_enable_loop(bool is_enabled) {}
  virtual void      transport_control_request_record(bool is_recording) {}
  virtual void      transport_control_request_toggle_record() {}
  virtual double    tuning_get_relative(clap_id tuning_id, int32_t channel, int32_t key, uint32_t sample_offset) { return 0.0; }
  virtual bool      tuning_should_play(clap_id tuning_id, int32_t channel, int32_t key) { return false; }
  virtual uint32_t  tuning_get_tuning_count() { return 0; }
  virtual bool      tuning_get_info(uint32_t tuning_index, clap_tuning_info_t *info) { return false; }
  virtual void      voice_info_changed() {}

//----------------------------------------------------------------------
private: // host
//----------------------------------------------------------------------

  clap_host_t MHost = {
    .clap_version   = CLAP_VERSION,
    .host_data      = this,
    .name           = "SAT_ClapHostImplementation",
    .vendor         = "skei.audio",
    .url            = "https://github.com/skei/SA_Toolkit",
    .version        = "0",
    .get_extension    = clap_host_get_extension_callback,
    .request_restart  = clap_host_request_restart_callback,
    .request_process  = clap_host_request_process_callback,
    .request_callback = clap_host_request_callback_callback
  };

  //----------

  static const void* clap_host_get_extension_callback(const struct clap_host *host, const char *extension_id) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    return _host->get_extension(extension_id);
  }

  static void clap_host_request_restart_callback(const struct clap_host *host) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    return _host->request_restart();
  }

  static void clap_host_request_process_callback(const struct clap_host *host) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    return _host->request_process();
  }

  static void clap_host_request_callback_callback(const struct clap_host *host) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    return _host->request_callback();
  }

//------------------------------
private: //
//------------------------------

//------------------------------
private: // ambisonic
//------------------------------

  //typedef struct clap_host_ambisonic {
  //   void(CLAP_ABI *changed)(const clap_host_t *host);
  //} clap_host_ambisonic_t;

//------------------------------
private: // audio ports
//------------------------------

  static
  bool clap_host_audio_ports_is_rescan_flag_supported_callback(const clap_host_t *host, uint32_t flag) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    return _host->audio_ports_is_rescan_flag_supported(flag);
  }

  static
  void clap_host_audio_ports_rescan_callback(const clap_host_t *host, uint32_t flags) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    return _host->audio_ports_rescan(flags);
  }

protected:

  clap_host_audio_ports_t MAudioPortsExt = {
    .is_rescan_flag_supported = clap_host_audio_ports_is_rescan_flag_supported_callback,
    .rescan                   = clap_host_audio_ports_rescan_callback
  };

//------------------------------
private: // audio ports config
//------------------------------

  static
  void clap_host_audio_ports_config_rescan_callback(const clap_host_t *host) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    return _host->audio_ports_config_rescan();
  }

protected:

  clap_host_audio_ports_config_t MAudioPortsConfigExt = {
    .rescan = clap_host_audio_ports_config_rescan_callback
  };

//------------------------------
private: // check for update
//------------------------------

  //typedef struct clap_host_check_for_update {
  //   // [main-thread]
  //   void(CLAP_ABI *on_new_version)(const clap_host_t *host, const clap_check_for_update_info_t *update_info);
  //} clap_host_check_for_update_t;

//------------------------------
private: // context menu
//------------------------------

  //typedef struct clap_host_context_menu {
  //   bool(CLAP_ABI *populate)(const clap_host_t *host, const clap_context_menu_target_t *target, const clap_context_menu_builder_t *builder);
  //   bool(CLAP_ABI *perform)(const clap_host_t *host, const clap_context_menu_target_t *target, clap_id action_id);
  //   bool(CLAP_ABI *can_popup)(const clap_host_t *host);
  //   bool(CLAP_ABI *popup)(const clap_host_t *host, const clap_context_menu_target_t *target, int32_t screen_index, int32_t x, int32_t y);
  //} clap_host_context_menu_t;

//------------------------------
private: // cv
//------------------------------

  //typedef struct clap_host_cv {
  //   void(CLAP_ABI *changed)(const clap_host_t *host);
  //} clap_host_cv_t

//------------------------------
private: // event registry
//------------------------------

  //typedef struct clap_host_event_registry {
  //   bool (*query)(const clap_host_t *host, const char *space_name, uint16_t *space_id);
  //} clap_host_event_registry_t;

//------------------------------
private: // gui
//------------------------------

  //typedef struct clap_host_gui {
  //   void (*resize_hints_changed)(const clap_host_t *host);
  //   bool (*request_resize)(const clap_host_t *host, uint32_t width, uint32_t height);
  //   bool (*request_show)(const clap_host_t *host);
  //   bool (*request_hide)(const clap_host_t *host);
  //   void (*closed)(const clap_host_t *host, bool was_destroyed);
  //} clap_host_gui_t;

//------------------------------
private: // latency
//------------------------------

  //typedef struct clap_host_latency {
  //   void (*changed)(const clap_host_t *host);
  //} clap_host_latency_t;

//------------------------------
private: // log
//------------------------------

  //typedef struct clap_host_log {
  //   void (*log)(const clap_host_t *host, clap_log_severity severity, const char *msg);
  //} clap_host_log_t;

//------------------------------
private: // midi mappings
//------------------------------

  //typedef struct clap_host_midi_mappings {
  //   void(CLAP_ABI *changed)(const clap_host_t *host);
  //} clap_host_midi_mappings_t;


//------------------------------
private: // note name
//------------------------------


  //typedef struct clap_host_note_name {
  //   void (*changed)(const clap_host_t *host);
  //} clap_host_note_name_t;


//------------------------------
private: // note ports
//------------------------------


  //typedef struct clap_host_note_ports {
  //   uint32_t (*supported_dialects)(const clap_host_t *host);
  //   void (*rescan)(const clap_host_t *host, uint32_t flags);
  //} clap_host_note_ports_t;


//------------------------------
private: // params
//------------------------------


  //typedef struct clap_host_params {
  //   void (*rescan)(const clap_host_t *host, clap_param_rescan_flags flags);
  //   void (*clear)(const clap_host_t *host, clap_id param_id, clap_param_clear_flags flags);
  //   void (*request_flush)(const clap_host_t *host);
  //} clap_host_params_t;


//------------------------------
private: // posix fd support
//------------------------------


  //typedef struct clap_host_posix_fd_support {
  //   bool (*register_fd)(const clap_host_t *host, int fd, clap_posix_fd_flags_t flags);
  //   bool (*modify_fd)(const clap_host_t *host, int fd, clap_posix_fd_flags_t flags);
  //   bool (*unregister_fd)(const clap_host_t *host, int fd);
  //} clap_host_posix_fd_support_t;


//------------------------------
private: // preset load
//------------------------------


  //typedef struct clap_host_preset_load {
  //  void(CLAP_ABI *on_error)(const clap_host_t *host, const char *uri, int32_t os_error, const char *msg);
  //  void(CLAP_ABI *loaded)(const clap_host_t *host, const char *uri, const char *load_key);
  //} clap_host_preset_load_t;


//------------------------------
private: // remote controls
//------------------------------

  //typedef struct clap_host_remote_controls {
  //   void(CLAP_ABI *changed)(const clap_host_t *host);
  //   void(CLAP_ABI *suggest_page)(const clap_host_t *host, clap_id page_id);
  //} clap_host_remote_controls_t;


//------------------------------
private: // resource directory
//------------------------------


  //typedef struct clap_host_resource_directory {
  //   bool(CLAP_ABI *request_directory)(const clap_host_t *host, bool is_shared);
  //   void(CLAP_ABI *release_directory)(const clap_host_t *host, bool is_shared);
  //} clap_host_resource_directory_t;

//------------------------------
private: // state
//------------------------------

  //typedef struct clap_host_state {
  //   void (*mark_dirty)(const clap_host_t *host);
  //} clap_host_state_t;

//------------------------------
private: // surround
//------------------------------

  //typedef struct clap_host_surround {
  //   void(CLAP_ABI *changed)(const clap_host_t *host);
  //   void(CLAP_ABI *get_preferred_channel_map)(const clap_host_t *host, uint8_t *channel_map, uint32_t channel_map_capacity, uint32_t *channel_count);
  //} clap_host_surround_t;

//------------------------------
private: // tail
//------------------------------

  //typedef struct clap_host_tail {
  //   void (*changed)(const clap_host_t *host);
  //} clap_host_tail_t;

//------------------------------
private: // thread check
//------------------------------

  //typedef struct clap_host_thread_check {
  //   bool (*is_main_thread)(const clap_host_t *host);
  //   bool (*is_audio_thread)(const clap_host_t *host);
  //} clap_host_thread_check_t;

//------------------------------
private: //thread pool
//------------------------------

  //typedef struct clap_host_thread_pool {
  //   bool (*request_exec)(const clap_host_t *host, uint32_t num_tasks);
  //} clap_host_thread_pool_t;

//------------------------------
private: // timer support
//------------------------------

  //typedef struct clap_host_timer_support {
  //   bool (*register_timer)(const clap_host_t *host, uint32_t period_ms, clap_id *timer_id);
  //   bool (*unregister_timer)(const clap_host_t *host, clap_id timer_id);
  //} clap_host_timer_support_t;

//------------------------------
private: // track info
//------------------------------

  //typedef struct clap_host_track_info {
  //   bool(CLAP_ABI *get)(const clap_host_t *host, clap_track_info_t *info);
  //} clap_host_track_info_t;

//------------------------------
private: // transport controls
//------------------------------

  //typedef struct clap_host_transport_control {
  //   void(CLAP_ABI *request_start)(const clap_host_t *host);
  //   void(CLAP_ABI *request_stop)(const clap_host_t *host);
  //   void(CLAP_ABI *request_continue)(const clap_host_t *host);
  //   void(CLAP_ABI *request_pause)(const clap_host_t *host);
  //   void(CLAP_ABI *request_toggle_play)(const clap_host_t *host);
  //   void(CLAP_ABI *request_jump)(const clap_host_t *host, clap_beattime position);
  //   void(CLAP_ABI *request_loop_region)(const clap_host_t *host, clap_beattime start, clap_beattime duration);
  //   void(CLAP_ABI *request_toggle_loop)(const clap_host_t *host);
  //   void(CLAP_ABI *request_enable_loop)(const clap_host_t *host, bool is_enabled);
  //   void(CLAP_ABI *request_record)(const clap_host_t *host, bool is_recording);
  //   void(CLAP_ABI *request_toggle_record)(const clap_host_t *host);
  //} clap_host_transport_control_t;

//------------------------------
private: // triggers
//------------------------------

  //typedef struct clap_host_triggers {
  //   void(CLAP_ABI *rescan)(const clap_host_t *host, clap_trigger_rescan_flags flags);
  //   void(CLAP_ABI *clear)(const clap_host_t *host, clap_id trigger_id, clap_trigger_clear_flags flags);
  //} clap_host_triggers_t;

//------------------------------
private: // tuning
//------------------------------

  //typedef struct clap_host_tuning {
  //   double(CLAP_ABI *get_relative)(const clap_host_t *host, clap_id tuning_id, int32_t channel, int32_t key, uint32_t sample_offset);
  //   bool(CLAP_ABI *should_play)(const clap_host_t *host, clap_id tuning_id, int32_t channel, int32_t key);
  //   uint32_t(CLAP_ABI *get_tuning_count)(const clap_host_t *host);
  //   bool(CLAP_ABI *get_info)(const clap_host_t  *host, uint32_t tuning_index, clap_tuning_info_t *info);
  //} clap_host_tuning_t;

//------------------------------
private: // voice info
//------------------------------

  //typedef struct clap_host_voice_info {
  //   void (*changed)(const clap_host_t *host);
  //} clap_host_voice_info_t;

};

//----------------------------------------------------------------------
#endif
