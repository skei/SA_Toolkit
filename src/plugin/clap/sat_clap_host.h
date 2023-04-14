#ifndef sat_clap_host_included
#define sat_clap_host_included
//----------------------------------------------------------------------

// host from a plugin perspective
// (call into host)

#include "plugin/clap/sat_clap.h"

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
public:
//------------------------------

  virtual void      ambisonic_changed()                                                                                                         { if (ext.ambisonic) ext.ambisonic->changed(MHost); }
  virtual bool      audio_ports_is_rescan_flag_supported(uint32_t flag)                                                                         { if (ext.audio_ports) return ext.audio_ports->is_rescan_flag_supported(MHost,flag); else return false; }
  virtual void      audio_ports_rescan(uint32_t flags)                                                                                          { if (ext.audio_ports) ext.audio_ports->rescan(MHost,flags); }
  virtual void      audio_ports_config_rescan()                                                                                                 { if (ext.audio_ports_config) ext.audio_ports_config->rescan(MHost); }
  virtual void      check_for_update_on_new_version(const clap_check_for_update_info_t *update_info)                                            { if (ext.check_for_update) ext.check_for_update->on_new_version(MHost,update_info); }
  virtual bool      context_menu_populate(const clap_context_menu_target_t *target, const clap_context_menu_builder_t *builder)                 { if (ext.context_menu) return ext.context_menu->populate(MHost,target,builder); else return false; }
  virtual bool      context_menu_perform(const clap_context_menu_target_t *target, clap_id action_id)                                           { if (ext.context_menu) return ext.context_menu->perform(MHost,target,action_id); else return false; }
  virtual bool      context_menu_can_popup()                                                                                                    { if (ext.context_menu) return ext.context_menu->can_popup(MHost); else return false; }
  virtual bool      context_menu_popup(const clap_context_menu_target_t *target, int32_t screen_index, int32_t x, int32_t y)                    { if (ext.context_menu) return ext.context_menu->popup(MHost,target,screen_index,x,y); else return false; }
  virtual void      cv_changed()                                                                                                                { if (ext.cv) ext.cv->changed(MHost); }
  virtual bool      event_registry_query(const char *space_name, uint16_t *space_id)                                                            { if (ext.event_registry) return ext.event_registry->query(MHost,space_name,space_id); else return false; }
  virtual void      gui_resize_hints_changed()                                                                                                  { if (ext.gui) ext.gui->resize_hints_changed(MHost); }
  virtual bool      gui_request_resize(uint32_t width, uint32_t height)                                                                         { if (ext.gui) return ext.gui->request_resize(MHost,width,height); else return false; }
  virtual bool      gui_request_show()                                                                                                          { if (ext.gui) return ext.gui->request_show(MHost); else return false; }
  virtual bool      gui_request_hide()                                                                                                          { if (ext.gui) return ext.gui->request_hide(MHost); else return false; }
  virtual void      gui_closed(bool was_destroyed)                                                                                              { if (ext.gui) ext.gui->closed(MHost,was_destroyed); }
  virtual void      latency_changed()                                                                                                           { if (ext.latency) ext.latency->changed(MHost); }
  virtual void      log_log(clap_log_severity severity, const char *msg)                                                                        { if (ext.log) ext.log->log(MHost,severity,msg); }
  virtual void      midi_mappings_changed()                                                                                                     { if (ext.midi_mappings) ext.midi_mappings->changed(MHost); }
  virtual void      note_name_changed()                                                                                                         { if (ext.note_name) ext.note_name->changed(MHost); }
  virtual uint32_t  note_ports_supported_dialects()                                                                                             { if (ext.note_ports) return ext.note_ports->supported_dialects(MHost); else return 0; }
  virtual void      note_ports_rescan(uint32_t flags)                                                                                           { if (ext.note_ports) ext.note_ports->rescan(MHost,flags); }
  virtual void      params_rescan(clap_param_rescan_flags flags)                                                                                { if (ext.params) ext.params->rescan(MHost,flags); }
  virtual void      params_clear(clap_id param_id, clap_param_clear_flags flags)                                                                { if (ext.params) ext.params->clear(MHost,param_id,flags); }
  virtual void      params_request_flush()                                                                                                      { if (ext.params) ext.params->request_flush(MHost); }
  virtual bool      posix_fd_support_register_fd(int fd, clap_posix_fd_flags_t flags)                                                           { if (ext.posix_fd_support) return ext.posix_fd_support->register_fd(MHost,fd,flags); else return false; }
  virtual bool      posix_fd_support_modify_fd(int fd, clap_posix_fd_flags_t flags)                                                             { if (ext.posix_fd_support) return ext.posix_fd_support->modify_fd(MHost,fd,flags); else return false; }
  virtual bool      posix_fd_support_unregister_fd(int fd)                                                                                      { if (ext.posix_fd_support) return ext.posix_fd_support->unregister_fd(MHost,fd); else return false; }
  virtual void      preset_load_on_error(uint32_t location_kind, const char *location, const char *load_key,int32_t os_error, const char *msg)  { if (ext.preset_load) ext.preset_load->on_error(MHost,location_kind,location,load_key,os_error,msg); }
  virtual void      preset_load_loaded(uint32_t location_kind, const char *location, const char *load_key)                                      { if (ext.preset_load) ext.preset_load->loaded(MHost,location_kind,location,load_key); }
  virtual void      remote_controls_changed()                                                                                                   { if (ext.remote_controls) ext.remote_controls->changed(MHost); }
  virtual void      remote_controls_suggest_page(clap_id page_id)                                                                               { if (ext.remote_controls) ext.remote_controls->suggest_page(MHost,page_id); }
  virtual bool      resource_directory_request_directory(bool is_shared)                                                                        { if (ext.resource_directory) return ext.resource_directory->request_directory(MHost,is_shared); else return false; }
  virtual void      resource_directory_release_directory(bool is_shared)                                                                        { if (ext.resource_directory) ext.resource_directory->release_directory(MHost,is_shared); }
  virtual void      state_mark_dirty()                                                                                                          { if (ext.state) ext.state->mark_dirty(MHost); }
  virtual void      surround_changed()                                                                                                          { if (ext.surround) ext.surround->changed(MHost); }
  virtual void      tail_changed()                                                                                                              { if (ext.tail) ext.tail->changed(MHost); }
  virtual bool      thread_check_is_main_thread()                                                                                               { if (ext.thread_check) return ext.thread_check->is_main_thread(MHost); else return false; }
  virtual bool      thread_check_is_audio_thread()                                                                                              { if (ext.thread_check) return ext.thread_check->is_audio_thread(MHost); else return false; }
  virtual bool      thread_pool_request_exec(uint32_t num_tasks)                                                                                { if (ext.thread_pool) return ext.thread_pool->request_exec(MHost,num_tasks); else return false; }
  virtual bool      timer_support_register_timer(uint32_t period_ms, clap_id *timer_id)                                                         { if (ext.timer_support) return ext.timer_support->register_timer(MHost,period_ms,timer_id); else return false; }
  virtual bool      timer_support_unregister_timer(clap_id timer_id)                                                                            { if (ext.timer_support) return ext.timer_support->unregister_timer(MHost,timer_id); else return false; }
  virtual bool      track_info_get(clap_track_info_t *info)                                                                                     { if (ext.track_info) return ext.track_info->get(MHost,info); else return false; }
  virtual void      transport_control_request_start()                                                                                           { if (ext.transport_control) ext.transport_control->request_start(MHost); }
  virtual void      transport_control_request_stop()                                                                                            { if (ext.transport_control) ext.transport_control->request_stop(MHost); }
  virtual void      transport_control_request_continue()                                                                                        { if (ext.transport_control) ext.transport_control->request_continue(MHost); }
  virtual void      transport_control_request_pause()                                                                                           { if (ext.transport_control) ext.transport_control->request_pause(MHost); }
  virtual void      transport_control_request_toggle_play()                                                                                     { if (ext.transport_control) ext.transport_control->request_toggle_play(MHost); }
  virtual void      transport_control_request_jump(clap_beattime position)                                                                      { if (ext.transport_control) ext.transport_control->request_jump(MHost,position); }
  virtual void      transport_control_request_loop_region(clap_beattime start, clap_beattime duration)                                          { if (ext.transport_control) ext.transport_control->request_loop_region(MHost,start,duration); }
  virtual void      transport_control_request_toggle_loop()                                                                                     { if (ext.transport_control) ext.transport_control->request_toggle_loop(MHost); }
  virtual void      transport_control_request_enable_loop(bool is_enabled)                                                                      { if (ext.transport_control) ext.transport_control->request_enable_loop(MHost,is_enabled); }
  virtual void      transport_control_request_record(bool is_recording)                                                                         { if (ext.transport_control) ext.transport_control->request_record(MHost,is_recording); }
  virtual void      transport_control_request_toggle_record()                                                                                   { if (ext.transport_control) ext.transport_control->request_toggle_record(MHost); }
  virtual void      triggers_rescan(clap_trigger_rescan_flags flags)                                                                            { if (ext.triggers) ext.triggers->rescan(MHost,flags); }
  virtual void      triggers_clear(clap_id trigger_id, clap_trigger_clear_flags flags)                                                          { if (ext.triggers) ext.triggers->clear(MHost,trigger_id,flags); }
  virtual double    tuning_get_relative(clap_id tuning_id, int32_t channel, int32_t key, uint32_t sample_offset)                                { if (ext.tuning) return ext.tuning->get_relative(MHost,tuning_id,channel,key,sample_offset); else return 0.0; }
  virtual bool      tuning_should_play(clap_id tuning_id, int32_t channel, int32_t key)                                                         { if (ext.tuning) return ext.tuning->should_play(MHost,tuning_id,channel,key); else return false; }
  virtual uint32_t  tuning_get_tuning_count()                                                                                                   { if (ext.tuning) return ext.tuning->get_tuning_count(MHost); else return 0; }
  virtual bool      tuning_get_info(uint32_t tuning_index, clap_tuning_info_t *info)                                                            { if (ext.tuning) return ext.tuning->get_info(MHost,tuning_index,info); else return false; }
  virtual void      voice_info_changed()                                                                                                        { if (ext.voice_info) ext.voice_info->changed(MHost); }


//------------------------------
private:
//------------------------------

  void initExtensions() {
    ext.ambisonic           = (clap_host_ambisonic_t*)MHost->get_extension(MHost,CLAP_EXT_AMBISONIC);
    ext.audio_ports_config  = (clap_host_audio_ports_config_t*)MHost->get_extension(MHost,CLAP_EXT_AUDIO_PORTS_CONFIG);
    ext.audio_ports         = (clap_host_audio_ports_t*)MHost->get_extension(MHost,CLAP_EXT_AUDIO_PORTS);
    ext.check_for_update    = (clap_host_check_for_update_t*)MHost->get_extension(MHost,CLAP_EXT_CHECK_FOR_UPDATE);
    ext.context_menu        = (clap_host_context_menu_t*)MHost->get_extension(MHost,CLAP_EXT_CONTEXT_MENU);
    ext.cv                  = (clap_host_cv_t*)MHost->get_extension(MHost,CLAP_EXT_CV);
    ext.event_registry      = (clap_host_event_registry_t*)MHost->get_extension(MHost,CLAP_EXT_EVENT_REGISTRY);
    ext.gui                 = (clap_host_gui_t*)MHost->get_extension(MHost,CLAP_EXT_GUI);
    ext.latency             = (clap_host_latency_t*)MHost->get_extension(MHost,CLAP_EXT_LATENCY);
    ext.log                 = (clap_host_log_t*)MHost->get_extension(MHost,CLAP_EXT_LOG);
    ext.midi_mappings       = (clap_host_midi_mappings_t*)MHost->get_extension(MHost,CLAP_EXT_MIDI_MAPPINGS);
    ext.note_name           = (clap_host_note_name_t*)MHost->get_extension(MHost,CLAP_EXT_NOTE_NAME);
    ext.note_ports          = (clap_host_note_ports_t*)MHost->get_extension(MHost,CLAP_EXT_NOTE_PORTS);
    ext.params              = (clap_host_params_t*)MHost->get_extension(MHost,CLAP_EXT_PARAMS);
    ext.posix_fd_support    = (clap_host_posix_fd_support_t*)MHost->get_extension(MHost,CLAP_EXT_POSIX_FD_SUPPORT);
    ext.preset_load         = (clap_host_preset_load_t*)MHost->get_extension(MHost,CLAP_EXT_PRESET_LOAD);
    ext.remote_controls     = (clap_host_remote_controls_t*)MHost->get_extension(MHost,CLAP_EXT_REMOTE_CONTROLS);
    ext.resource_directory  = (clap_host_resource_directory_t*)MHost->get_extension(MHost,CLAP_EXT_RESOURCE_DIRECTORY);
    ext.state               = (clap_host_state_t*)MHost->get_extension(MHost,CLAP_EXT_STATE);
    ext.surround            = (clap_host_surround_t*)MHost->get_extension(MHost,CLAP_EXT_SURROUND);
    ext.tail                = (clap_host_tail_t*)MHost->get_extension(MHost,CLAP_EXT_TAIL);
    ext.thread_check        = (clap_host_thread_check_t*)MHost->get_extension(MHost,CLAP_EXT_THREAD_CHECK);
    ext.thread_pool         = (clap_host_thread_pool_t*)MHost->get_extension(MHost,CLAP_EXT_THREAD_POOL);
    ext.timer_support       = (clap_host_timer_support_t*)MHost->get_extension(MHost,CLAP_EXT_TIMER_SUPPORT);
    ext.track_info          = (clap_host_track_info_t*)MHost->get_extension(MHost,CLAP_EXT_TRACK_INFO);
    ext.transport_control   = (clap_host_transport_control_t*)MHost->get_extension(MHost,CLAP_EXT_TRANSPORT_CONTROL);
    ext.triggers            = (clap_host_triggers_t*)MHost->get_extension(MHost,CLAP_EXT_TRIGGERS);
    ext.tuning              = (clap_host_tuning_t*)MHost->get_extension(MHost,CLAP_EXT_TUNING);
    ext.voice_info          = (clap_host_voice_info_t*)MHost->get_extension(MHost,CLAP_EXT_VOICE_INFO);
  }
  
};

//----------------------------------------------------------------------
#endif


