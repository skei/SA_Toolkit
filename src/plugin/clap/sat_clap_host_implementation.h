#ifndef sat_clap_host_implementation_included
#define sat_clap_host_implementation_included
//----------------------------------------------------------------------

// aka SAT_HostImplementation

#include "sat.h"
#include "plugin/clap/sat_clap.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_ClapHostImplementation {

//------------------------------
private:
//------------------------------

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

  const clap_host_t* getClapHost() {
    return &MHost;
  }

  //------------------------------

  virtual const void* get_extension(const char *extension_id) { return nullptr; }
  virtual void        request_restart() {}
  virtual void        request_process() {}
  virtual void        request_callback() {}

//------------------------------
public: // extensions
//------------------------------

  virtual bool        audio_ports_is_rescan_flag_supported(uint32_t flag) { return false; }
  virtual void        audio_ports_rescan(uint32_t flags) {}
  virtual void        audio_ports_config_rescan() {}
  virtual bool        event_registry_query(const char *space_name, uint16_t *space_id) { return false; }
  virtual void        gui_resize_hints_changed() {}
  virtual bool        gui_request_resize(uint32_t width, uint32_t height) { return false; }
  virtual bool        gui_request_show() { return false; }
  virtual bool        gui_request_hide() { return false; }
  virtual void        gui_closed(bool was_destroyed) {}
  virtual void        latency_changed() {}
  virtual void        log_log(clap_log_severity severity, const char *msg) {}
  virtual void        note_name_changed() {}
  virtual uint32_t    note_ports_supported_dialects() { return 0; }
  virtual void        note_ports_rescan(uint32_t flags) {}
  virtual void        params_rescan(clap_param_rescan_flags flags) {}
  virtual void        params_clear(clap_id param_id, clap_param_clear_flags flags) {}
  virtual void        params_request_flush() {}
  virtual bool        posix_fd_support_register_fd(int fd, clap_posix_fd_flags_t flags) { return false; }
  virtual bool        posix_fd_support_modify_fd(int fd, clap_posix_fd_flags_t flags) { return false; }
  virtual bool        posix_fd_support_unregister_fd(int fd) { return false; }
  virtual void        state_mark_dirty() {}
  virtual void        tail_changed() {}
  virtual bool        thread_check_is_main_thread() { return false; }
  virtual bool        thread_check_is_audio_thread() { return false; }
  virtual bool        thread_pool_request_exec(uint32_t num_tasks) { return false; }
  virtual bool        timer_support_register_timer(uint32_t period_ms, clap_id *timer_id) { return false; }
  virtual bool        timer_support_unregister_timer(clap_id timer_id) { return false; }
  virtual void        voice_info_changed() {}

//------------------------------
public: // drafts
//------------------------------

  virtual void        ambisonic_changed() {}
  virtual void        check_for_update_on_new_version(const clap_check_for_update_info_t *update_info) {}
  virtual bool        context_menu_populate(const clap_context_menu_target_t *target, const clap_context_menu_builder_t *builder) { return false; }
  virtual bool        context_menu_perform(const clap_context_menu_target_t *target, clap_id action_id) { return false; }
  virtual bool        context_menu_can_popup() { return false; }
  virtual bool        context_menu_popup(const clap_context_menu_target_t *target, int32_t screen_index, int32_t x, int32_t y) { return false; }
  virtual void        cv_changed() {}
  virtual void        midi_mappings_changed() {}
  virtual void        preset_load_on_error(uint32_t location_kind, const char *location, const char *load_key,int32_t os_error, const char *msg) {}
  virtual void        preset_load_loaded(uint32_t location_kind, const char *location, const char *load_key) {}
  virtual void        remote_controls_changed() {}
  virtual void        remote_controls_suggest_page(clap_id page_id) {}
  virtual bool        resource_directory_request_directory(bool is_shared) { return false; }
  virtual void        resource_directory_release_directory(bool is_shared) {}
  virtual void        surround_changed() {}
  virtual bool        track_info_get(clap_track_info_t *info) { return false; }
  virtual void        transport_control_request_start() {}
  virtual void        transport_control_request_stop() {}
  virtual void        transport_control_request_continue() {}
  virtual void        transport_control_request_pause() {}
  virtual void        transport_control_request_toggle_play() {}
  virtual void        transport_control_request_jump(clap_beattime position) {}
  virtual void        transport_control_request_loop_region(clap_beattime start, clap_beattime duration) {}
  virtual void        transport_control_request_toggle_loop() {}
  virtual void        transport_control_request_enable_loop(bool is_enabled) {}
  virtual void        transport_control_request_record(bool is_recording) {}
  virtual void        transport_control_request_toggle_record() {}
  virtual void        triggers_rescan(clap_trigger_rescan_flags flags) {}
  virtual void        triggers_clear(clap_id trigger_id, clap_trigger_clear_flags flags) {}
  virtual double      tuning_get_relative(clap_id tuning_id, int32_t channel, int32_t key, uint32_t sample_offset) { return 0.0; }
  virtual bool        tuning_should_play(clap_id tuning_id, int32_t channel, int32_t key) { return false; }
  virtual uint32_t    tuning_get_tuning_count() { return 0; }
  virtual bool        tuning_get_info(uint32_t tuning_index, clap_tuning_info_t *info) { return false; }
  
//----------------------------------------------------------------------
private: // host
//----------------------------------------------------------------------

  clap_host_t MHost = {
    .clap_version     = CLAP_VERSION,
    .host_data        = this,
    .name             = "SAT_ClapHostImplementation",
    .vendor           = SAT_VENDOR,
    .url              = SAT_URL,
    .version          = SAT_VERSION,
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
    _host->request_restart();
  }

  static void clap_host_request_process_callback(const struct clap_host *host) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->request_process();
  }

  static void clap_host_request_callback_callback(const struct clap_host *host) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->request_callback();
  }

//----------------------------------------------------------------------
private: // extensions
//----------------------------------------------------------------------

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
    _host->audio_ports_rescan(flags);
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
    _host->audio_ports_config_rescan();
  }

protected:

  clap_host_audio_ports_config_t MAudioPortsConfigExt = {
    .rescan = clap_host_audio_ports_config_rescan_callback
  };

//------------------------------
private: // event registry
//------------------------------

  static
  bool clap_host_event_registry_query_callback(const clap_host_t *host, const char *space_name, uint16_t *space_id) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    return _host->event_registry_query(space_name,space_id);
  }

protected:

  clap_host_event_registry_t MEventRegistryExt = {
    .query  = clap_host_event_registry_query_callback
  };

//------------------------------
private: // gui
//------------------------------

  static
  void clap_host_gui_resize_hints_changed_callback(const clap_host_t *host) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->gui_resize_hints_changed();
  }

  static
  bool clap_host_gui_request_resize_callback(const clap_host_t *host, uint32_t width, uint32_t height) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    return _host->gui_request_resize(width,height);
  }

  static
  bool clap_host_gui_request_show_callback(const clap_host_t *host) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    return _host->gui_request_show();
  }

  static
  bool clap_host_gui_request_hide_callback(const clap_host_t *host) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    return _host->gui_request_hide();
  }

  static
  void clap_host_gui_closed_callback(const clap_host_t *host, bool was_closed) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->gui_closed(was_closed);
  }

protected:

  clap_host_gui_t MGuiExt = {
    .resize_hints_changed = clap_host_gui_resize_hints_changed_callback,
    .request_resize       = clap_host_gui_request_resize_callback,
    .request_show         = clap_host_gui_request_show_callback,
    .request_hide         = clap_host_gui_request_hide_callback,
    .closed               = clap_host_gui_closed_callback
  };

//------------------------------
private: // latency
//------------------------------

  static
  void clap_host_latency_changed_callback(const clap_host_t *host) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->latency_changed();
  }

protected:

  clap_host_latency_t MLatencyExt = {
    .changed  = clap_host_latency_changed_callback
  };

//------------------------------
private: // log
//------------------------------

  static
  void clap_host_log_log_callback(const clap_host_t *host, clap_log_severity severity, const char *msg) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->log_log(severity,msg);
  }

protected:

  clap_host_log_t MLogExt = {
    .log  = clap_host_log_log_callback
  };

//------------------------------
private: // note name
//------------------------------

  static
  void clap_host_note_name_changed_callback(const clap_host_t *host) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->note_name_changed();
  }

protected:

  clap_host_note_name_t MNoteNameExt = {
    .changed  = clap_host_note_name_changed_callback
  };

//------------------------------
private: // note ports
//------------------------------

  static
  uint32_t clap_host_note_ports_supported_dialects_callback(const clap_host_t *host) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    return _host->note_ports_supported_dialects();
  }

  static
  void clap_host_note_ports_rescan_callback(const clap_host_t *host, uint32_t flags) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->note_ports_rescan(flags);
  }

protected:

  clap_host_note_ports_t MNotePortsExt = {
    .supported_dialects  = clap_host_note_ports_supported_dialects_callback,
    .rescan              = clap_host_note_ports_rescan_callback
  };


//------------------------------
private: // params
//------------------------------

  static
  void clap_host_params_rescan_callback(const clap_host_t *host, clap_param_rescan_flags flags) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->params_rescan(flags);
  }

  static
  void clap_host_params_clear_callback(const clap_host_t *host, clap_id param_id, clap_param_clear_flags flags) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->params_clear(param_id,flags);
  }

  static
  void clap_host_params_request_flush_callback(const clap_host_t *host) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->params_request_flush();
  }

protected:

  clap_host_params_t MParamsExt = {
    .rescan         = clap_host_params_rescan_callback,
    .clear          = clap_host_params_clear_callback,
    .request_flush  = clap_host_params_request_flush_callback
  };

//------------------------------
private: // posix fd support
//------------------------------

  static
  bool clap_host_posix_fd_support_register_fd_callback(const clap_host_t *host, int fd, clap_posix_fd_flags_t flags) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    return _host->posix_fd_support_register_fd(fd,flags);
  }

  static
  bool clap_host_posix_fd_support_modify_fd_callback(const clap_host_t *host, int fd, clap_posix_fd_flags_t flags) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    return _host->posix_fd_support_modify_fd(fd,flags);
  }

  static
  bool clap_host_posix_fd_support_unregister_fd_callback(const clap_host_t *host, int fd) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    return _host->posix_fd_support_unregister_fd(fd);
  }

protected:

  clap_host_posix_fd_support_t MPosixFdSupportExt = {
    .register_fd    = clap_host_posix_fd_support_register_fd_callback,
    .modify_fd      = clap_host_posix_fd_support_modify_fd_callback,
    .unregister_fd  = clap_host_posix_fd_support_unregister_fd_callback
  };

//------------------------------
private: // state
//------------------------------

  static
  void clap_host_state_mark_dirty_callback(const clap_host_t *host) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->state_mark_dirty();
  }

protected:

  clap_host_state_t MStateExt = {
    .mark_dirty  = clap_host_state_mark_dirty_callback
  };

//------------------------------
private: // tail
//------------------------------

  static
  void clap_host_tail_changed_callback(const clap_host_t *host) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->tail_changed();
  }

protected:

  clap_host_tail_t MTailExt = {
    .changed  = clap_host_tail_changed_callback
  };

//------------------------------
private: // thread check
//------------------------------

  static
  bool clap_host_thread_check_is_main_thread_callback(const clap_host_t *host) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    return _host->thread_check_is_main_thread();
  }

  static
  bool clap_host_thread_check_is_audio_thread_callback(const clap_host_t *host) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    return _host->thread_check_is_audio_thread();
  }

protected:

  clap_host_thread_check_t MThreadCheckExt = {
    .is_main_thread   = clap_host_thread_check_is_main_thread_callback,
    .is_audio_thread  = clap_host_thread_check_is_audio_thread_callback
  };

//------------------------------
private: //thread pool
//------------------------------

  static
  bool clap_host_thread_pool_request_exec_callback(const clap_host_t *host, uint32_t num_tasks) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    return _host->thread_pool_request_exec(num_tasks);
  }

protected:

  clap_host_thread_pool_t MThreadPoolExt = {
    .request_exec  = clap_host_thread_pool_request_exec_callback
  };

//------------------------------
private: // timer support
//------------------------------

  static
  bool clap_host_timer_support_register_timer_callback(const clap_host_t *host, uint32_t period_ms, clap_id *timer_id) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    return _host->timer_support_register_timer(period_ms,timer_id);
  }

  static
  bool clap_host_timer_support_unregister_timer_callback(const clap_host_t *host, clap_id timer_id) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    return _host->timer_support_unregister_timer(timer_id);
  }

protected:

  clap_host_timer_support_t MTimerSupportExt = {
    .register_timer   = clap_host_timer_support_register_timer_callback,
    .unregister_timer = clap_host_timer_support_unregister_timer_callback
  };

//------------------------------
private: // voice info
//------------------------------

  static
  void clap_host_voice_info_changed_callback(const clap_host_t *host) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->voice_info_changed();
  }

protected:

  clap_host_voice_info_t MVoiceInfoExt = {
    .changed  = clap_host_voice_info_changed_callback
  };


//----------------------------------------------------------------------
private: // drafts
//----------------------------------------------------------------------

//------------------------------
private: // ambisonic
//------------------------------

  static
  void clap_host_ambisonic_changed_callback(const clap_host_t *host) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->ambisonic_changed();
  }

protected:

  clap_host_ambisonic_t MAmbisonicExt = {
    .changed = clap_host_ambisonic_changed_callback
  };

//------------------------------
private: // check for update
//------------------------------

  static
  void clap_host_check_for_update_on_new_version_callback(const clap_host_t *host, const clap_check_for_update_info_t *update_info) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->check_for_update_on_new_version(update_info);
  }

protected:

  clap_host_check_for_update_t MCheckForUpdateExt = {
    .on_new_version = clap_host_check_for_update_on_new_version_callback
  };

//------------------------------
private: // context menu
//------------------------------

  static
  bool clap_host_context_menu_populate_callback(const clap_host_t *host, const clap_context_menu_target_t *target, const clap_context_menu_builder_t *builder) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    return _host->context_menu_populate(target,builder);
  }

  static
  bool clap_host_context_menu_perform_callback(const clap_host_t *host, const clap_context_menu_target_t *target, clap_id action_id) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    return _host->context_menu_perform(target,action_id);
  }

  static
  bool clap_host_context_menu_can_popup_callback(const clap_host_t *host) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    return _host->context_menu_can_popup();
  }

  static
  bool clap_host_context_menu_popup_callback(const clap_host_t *host, const clap_context_menu_target_t *target, int32_t screen_index, int32_t x, int32_t y) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    return _host->context_menu_popup(target,screen_index,x,y);
  }

protected:

  clap_host_context_menu_t MContextMenuExt = {
    .populate   = clap_host_context_menu_populate_callback,
    .perform    = clap_host_context_menu_perform_callback,
    .can_popup  = clap_host_context_menu_can_popup_callback,
    .popup      = clap_host_context_menu_popup_callback
  };

//------------------------------
private: // cv
//------------------------------

  static
  void clap_host_cv_changed_callback(const clap_host_t *host) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->cv_changed();
  }

protected:

  clap_host_cv_t MCVExt = {
    .changed    = clap_host_cv_changed_callback
  };

//------------------------------
private: // midi mappings
//------------------------------

  static
  void clap_host_midi_mappings_changed_callback(const clap_host_t *host) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->midi_mappings_changed();
  }

protected:

  clap_host_midi_mappings_t MMidiMappingsExt = {
    .changed  = clap_host_midi_mappings_changed_callback
  };

//------------------------------
private: // preset load
//------------------------------

  static
  void clap_host_preset_load_on_error_callback(const clap_host_t *host, uint32_t location_kind, const char *location, const char *load_key,int32_t os_error, const char *msg) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->preset_load_on_error(location_kind,location,load_key,os_error,msg);
  }

  static
  void clap_host_preset_load_loaded_callback(const clap_host_t *host, uint32_t location_kind, const char *location, const char *load_key) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->preset_load_loaded(location_kind,location,load_key);
  }

protected:

  clap_host_preset_load_t MPresetLoadExt = {
    .on_error = clap_host_preset_load_on_error_callback,
    .loaded   = clap_host_preset_load_loaded_callback
  };

//------------------------------
private: // remote controls
//------------------------------

  static
  void clap_host_remote_controls_changed_callback(const clap_host_t *host) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->remote_controls_changed();
  }

  static
  void clap_host_remote_controls_suggest_page_callback(const clap_host_t *host, clap_id page_id) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->remote_controls_suggest_page(page_id);
  }

protected:

  clap_host_remote_controls_t MRemoteControlsExt = {
    .changed      = clap_host_remote_controls_changed_callback,
    .suggest_page = clap_host_remote_controls_suggest_page_callback
  };

//------------------------------
private: // resource directory
//------------------------------

  static
  bool clap_host_resource_directory_request_directory_callback(const clap_host_t *host, bool is_shared) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    return _host->resource_directory_request_directory(is_shared);
  }

  static
  void clap_host_resource_directory_release_directory_callback(const clap_host_t *host, bool is_shared) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->resource_directory_release_directory(is_shared);
  }

protected:

  clap_host_resource_directory_t MResourceDirectoryExt = {
    .request_directory  = clap_host_resource_directory_request_directory_callback,
    .release_directory  = clap_host_resource_directory_release_directory_callback
  };

//------------------------------
private: // surround
//------------------------------

  static
  void clap_host_surround_changed_callback(const clap_host_t *host) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->surround_changed();
  }

protected:

  clap_host_surround_t MSurroundExt = {
    .changed = clap_host_surround_changed_callback//,
  };

//------------------------------
private: // track info
//------------------------------

  static
  bool clap_host_track_info_get_callback(const clap_host_t *host, clap_track_info_t *info) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    return _host->track_info_get(info);
  }

protected:

  clap_host_track_info_t MTrackInfoExt = {
    .get  = clap_host_track_info_get_callback
  };

//------------------------------
private: // transport controls
//------------------------------

  static
  void clap_host_transport_control_request_start_callback(const clap_host_t *host) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->transport_control_request_start();
  }

  static
  void clap_host_transport_control_request_stop_callback(const clap_host_t *host) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->transport_control_request_stop();
  }

  static
  void clap_host_transport_control_request_continue_callback(const clap_host_t *host) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->transport_control_request_continue();
  }

  static
  void clap_host_transport_control_request_pause_callback(const clap_host_t *host) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->transport_control_request_pause();
  }

  static
  void clap_host_transport_control_request_toggle_play_callback(const clap_host_t *host) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->transport_control_request_toggle_play();
  }

  static
  void clap_host_transport_control_request_jump_callback(const clap_host_t *host, clap_beattime position) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->transport_control_request_jump(position);
  }

  static
  void clap_host_transport_control_request_loop_region_callback(const clap_host_t *host, clap_beattime start, clap_beattime duration) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->transport_control_request_loop_region(start,duration);
  }

  static
  void clap_host_transport_control_request_toggle_loop_callback(const clap_host_t *host) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->transport_control_request_toggle_loop();
  }

  static
  void clap_host_transport_control_request_enable_loop_callback(const clap_host_t *host, bool is_enabled) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->transport_control_request_enable_loop(is_enabled);
  }

  static
  void clap_host_transport_control_request_record_callback(const clap_host_t *host, bool is_recording) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->transport_control_request_record(is_recording);
  }

  static
  void clap_host_transport_control_request_toggle_record_callback(const clap_host_t *host) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->transport_control_request_toggle_record();
  }

protected:

  clap_host_transport_control_t MTransportControlExt = {
    .request_start          = clap_host_transport_control_request_start_callback,
    .request_stop           = clap_host_transport_control_request_stop_callback,
    .request_continue       = clap_host_transport_control_request_continue_callback,
    .request_pause          = clap_host_transport_control_request_pause_callback,
    .request_toggle_play    = clap_host_transport_control_request_toggle_play_callback,
    .request_jump           = clap_host_transport_control_request_jump_callback,
    .request_loop_region    = clap_host_transport_control_request_loop_region_callback,
    .request_toggle_loop    = clap_host_transport_control_request_toggle_loop_callback,
    .request_enable_loop    = clap_host_transport_control_request_enable_loop_callback,
    .request_record         = clap_host_transport_control_request_record_callback,
    .request_toggle_record  = clap_host_transport_control_request_toggle_record_callback
  };

//------------------------------
private: // triggers
//------------------------------

  static
  void clap_host_triggers_rescan_callback(const clap_host_t *host, clap_trigger_rescan_flags flags) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->triggers_rescan(flags);
  }

  static
  void clap_host_triggers_clear_callback(const clap_host_t *host, clap_id trigger_id, clap_trigger_clear_flags flags) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    _host->triggers_clear(trigger_id,flags);
  }

protected:

  clap_host_triggers_t MTriggersExt = {
    .rescan = clap_host_triggers_rescan_callback,
    .clear  = clap_host_triggers_clear_callback
  };

//------------------------------
private: // tuning
//------------------------------

  static
  double clap_host_tuning_get_relative_callback(const clap_host_t *host, clap_id tuning_id, int32_t channel, int32_t key, uint32_t sample_offset) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    return _host->tuning_get_relative(tuning_id,channel,key,sample_offset);
  }

  static
  bool clap_host_tuning_should_play_callback(const clap_host_t *host, clap_id tuning_id, int32_t channel, int32_t key) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    return _host->tuning_should_play(tuning_id,channel,key);
  }

  static
  uint32_t clap_host_tuning_get_tuning_count_callback(const clap_host_t *host) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    return _host->tuning_get_tuning_count();
  }

  static
  bool clap_host_tuning_get_info_callback(const clap_host_t *host, uint32_t tuning_index, clap_tuning_info_t *info) {
    SAT_ClapHostImplementation* _host = (SAT_ClapHostImplementation*)host->host_data;
    return _host->tuning_get_info(tuning_index,info);
  }

protected:

  clap_host_tuning_t MTuningExt = {
    .get_relative     = clap_host_tuning_get_relative_callback,
    .should_play      = clap_host_tuning_should_play_callback,
    .get_tuning_count = clap_host_tuning_get_tuning_count_callback,
    .get_info         = clap_host_tuning_get_info_callback
  };

};

//----------------------------------------------------------------------
#endif

