#ifndef sa_interceptor_host_included
#define sa_interceptor_host_included
//----------------------------------------------------------------------

#include "base/sat.h"
//#include "base/system/sat_library.h"
#include "plugin/clap/sat_clap.h"
//#include "plugin/clap/sat_clap_plugin.h"
#include "plugin/clap/sat_clap_host_implementation.h"



//----------------------------------------------------------------------
//
// host
//
//----------------------------------------------------------------------

class sa_interceptor_host
: public SAT_ClapHostImplementation {

//------------------------------
private:
//------------------------------

  const clap_host_t*      MInterceptedHost  = nullptr;
  SAT_ClapHostExtensions  ext               = {};
//------------------------------
public:
//------------------------------

  sa_interceptor_host(const clap_host_t* AHost)
  : SAT_ClapHostImplementation() {
    SAT_Print("HOST\n");
    MInterceptedHost = AHost;
    initExtensions();
  };

  //----------

  virtual ~sa_interceptor_host() {
    SAT_Print("~HOST\n");
    //return MInterceptedPlugin->
  }
  
//------------------------------
public:
//------------------------------

  void initExtensions() {
    SAT_Print("HOST\n");
    ext.ambisonic               = (clap_host_ambisonic_t*)MInterceptedHost->get_extension(MInterceptedHost,CLAP_EXT_AMBISONIC);
    ext.audio_ports             = (clap_host_audio_ports_t*)MInterceptedHost->get_extension(MInterceptedHost,CLAP_EXT_AUDIO_PORTS);
    ext.audio_ports_config      = (clap_host_audio_ports_config_t*)MInterceptedHost->get_extension(MInterceptedHost,CLAP_EXT_AUDIO_PORTS_CONFIG);
    ext.check_for_update        = (clap_host_check_for_update_t*)MInterceptedHost->get_extension(MInterceptedHost,CLAP_EXT_CHECK_FOR_UPDATE);
    ext.context_menu            = (clap_host_context_menu_t*)MInterceptedHost->get_extension(MInterceptedHost,CLAP_EXT_CONTEXT_MENU);
    ext.cv                      = (clap_host_cv_t*)MInterceptedHost->get_extension(MInterceptedHost,CLAP_EXT_CV);
    ext.event_registry          = (clap_host_event_registry_t*)MInterceptedHost->get_extension(MInterceptedHost,CLAP_EXT_EVENT_REGISTRY);
    ext.gui                     = (clap_host_gui_t*)MInterceptedHost->get_extension(MInterceptedHost,CLAP_EXT_GUI);
    ext.latency                 = (clap_host_latency_t*)MInterceptedHost->get_extension(MInterceptedHost,CLAP_EXT_LATENCY);
    ext.log                     = (clap_host_log_t*)MInterceptedHost->get_extension(MInterceptedHost,CLAP_EXT_LOG);
    ext.midi_mappings           = (clap_host_midi_mappings_t*)MInterceptedHost->get_extension(MInterceptedHost,CLAP_EXT_MIDI_MAPPINGS);
    ext.note_name               = (clap_host_note_name_t*)MInterceptedHost->get_extension(MInterceptedHost,CLAP_EXT_NOTE_NAME);
    ext.note_ports              = (clap_host_note_ports_t*)MInterceptedHost->get_extension(MInterceptedHost,CLAP_EXT_NOTE_PORTS);
    ext.params                  = (clap_host_params_t*)MInterceptedHost->get_extension(MInterceptedHost,CLAP_EXT_PARAMS);
    ext.posix_fd_support        = (clap_host_posix_fd_support_t*)MInterceptedHost->get_extension(MInterceptedHost,CLAP_EXT_POSIX_FD_SUPPORT);
    ext.preset_load             = (clap_host_preset_load_t*)MInterceptedHost->get_extension(MInterceptedHost,CLAP_EXT_PRESET_LOAD);
    ext.remote_controls         = (clap_host_remote_controls_t*)MInterceptedHost->get_extension(MInterceptedHost,CLAP_EXT_REMOTE_CONTROLS);
    ext.resource_directory      = (clap_host_resource_directory_t*)MInterceptedHost->get_extension(MInterceptedHost,CLAP_EXT_RESOURCE_DIRECTORY);
    ext.state                   = (clap_host_state_t*)MInterceptedHost->get_extension(MInterceptedHost,CLAP_EXT_STATE);
    ext.surround                = (clap_host_surround_t*)MInterceptedHost->get_extension(MInterceptedHost,CLAP_EXT_SURROUND);
    ext.tail                    = (clap_host_tail_t*)MInterceptedHost->get_extension(MInterceptedHost,CLAP_EXT_TAIL);
    ext.thread_check            = (clap_host_thread_check_t*)MInterceptedHost->get_extension(MInterceptedHost,CLAP_EXT_THREAD_CHECK);
    ext.thread_pool             = (clap_host_thread_pool_t*)MInterceptedHost->get_extension(MInterceptedHost,CLAP_EXT_THREAD_POOL);
    ext.timer_support           = (clap_host_timer_support_t*)MInterceptedHost->get_extension(MInterceptedHost,CLAP_EXT_TIMER_SUPPORT);
    ext.track_info              = (clap_host_track_info_t*)MInterceptedHost->get_extension(MInterceptedHost,CLAP_EXT_TRACK_INFO);
    ext.transport_control       = (clap_host_transport_control_t*)MInterceptedHost->get_extension(MInterceptedHost,CLAP_EXT_TRANSPORT_CONTROL);
    ext.triggers                = (clap_host_triggers_t*)MInterceptedHost->get_extension(MInterceptedHost,CLAP_EXT_TRIGGERS);
    ext.tuning                  = (clap_host_tuning_t*)MInterceptedHost->get_extension(MInterceptedHost,CLAP_EXT_TUNING);
    ext.voice_info              = (clap_host_voice_info_t*)MInterceptedHost->get_extension(MInterceptedHost,CLAP_EXT_VOICE_INFO);
  }

//------------------------------
public:
//------------------------------

  const void* get_extension(const char *extension_id) final {
    SAT_Print("extension_id: %s ",extension_id);
    const void* extension = MInterceptedHost->get_extension(MInterceptedHost,extension_id);
    SAT_DPrint("%s\n",extension ? "true" : "false");
    return extension;
  }

  void request_restart() final {
    SAT_Print("\n");
    MInterceptedHost->request_restart(MInterceptedHost);
  }

  void request_process() final {
    SAT_Print("\n");
    MInterceptedHost->request_process(MInterceptedHost);
  }

  void request_callback() final {
    SAT_Print("\n");
    MInterceptedHost->request_callback(MInterceptedHost);
  }  
  
//------------------------------
public:
//------------------------------

  void ambisonic_changed() final {
    SAT_Print("\n");
    ext.ambisonic->changed(MInterceptedHost);
  }

  bool audio_ports_is_rescan_flag_supported(uint32_t flag) final {
    SAT_Print("\n");
    return ext.audio_ports->is_rescan_flag_supported(MInterceptedHost,flag);
  }

  void audio_ports_rescan(uint32_t flags) final {
    SAT_Print("\n");
    return ext.audio_ports->rescan(MInterceptedHost,flags);
  }

  void audio_ports_config_rescan() final {
    SAT_Print("\n");
    return ext.audio_ports_config->rescan(MInterceptedHost);
  }

  void check_for_update_on_new_version(const clap_check_for_update_info_t *update_info) final {
    SAT_Print("\n");
    ext.check_for_update->on_new_version(MInterceptedHost,update_info);
  }

  bool context_menu_populate(const clap_context_menu_target_t *target, const clap_context_menu_builder_t *builder) final {
    SAT_Print("\n");
    return ext.context_menu->populate(MInterceptedHost,target,builder);
  }

  bool context_menu_perform(const clap_context_menu_target_t *target, clap_id action_id) final {
    SAT_Print("\n");
    return ext.context_menu->perform(MInterceptedHost,target,action_id);
  }

  bool context_menu_can_popup() final {
    SAT_Print("\n");
    return ext.context_menu->can_popup(MInterceptedHost);
  }

  bool context_menu_popup(const clap_context_menu_target_t *target, int32_t screen_index, int32_t x, int32_t y) final {
    SAT_Print("\n");
    return ext.context_menu->popup(MInterceptedHost,target,screen_index,x,y);
  }

  void cv_changed() final {
    SAT_Print("\n");
    ext.cv->changed(MInterceptedHost);
  }

  bool event_registry_query(const char *space_name, uint16_t *space_id) final {
    SAT_Print("\n");
    return ext.event_registry->query(MInterceptedHost,space_name,space_id);
  }

  void gui_resize_hints_changed() final {
    SAT_Print("\n");
    ext.gui->resize_hints_changed(MInterceptedHost);
  }

  bool gui_request_resize(uint32_t width, uint32_t height) final {
    SAT_Print("\n");
    return ext.gui->request_resize(MInterceptedHost,width,height);
  }

  bool gui_request_show() final {
    SAT_Print("\n");
    return ext.gui->request_show(MInterceptedHost);
  }

  bool gui_request_hide() final {
    SAT_Print("\n");
    return ext.gui->request_hide(MInterceptedHost);
  }

  void gui_closed(bool was_destroyed) final {
    SAT_Print("\n");
    ext.gui->closed(MInterceptedHost,was_destroyed);
  }

  void latency_changed() final {
    SAT_Print("\n");
    ext.latency->changed(MInterceptedHost);
  }

  void log_log(clap_log_severity severity, const char *msg) final {
    SAT_Print("\n");
    ext.log->log(MInterceptedHost,severity,msg);
  }

  void midi_mappings_changed() final {
    SAT_Print("\n");
    ext.midi_mappings->changed(MInterceptedHost);
  }

  void note_name_changed() final {
    SAT_Print("\n");
    ext.note_name->changed(MInterceptedHost);
  }

  uint32_t note_ports_supported_dialects() final {
    SAT_Print("\n");
    return ext.note_ports->supported_dialects(MInterceptedHost);
  }

  void note_ports_rescan(uint32_t flags) final {
    SAT_Print("\n");
    ext.note_ports->rescan(MInterceptedHost,flags);
  }

  void params_rescan(clap_param_rescan_flags flags) final {
    SAT_Print("\n");
    ext.params->rescan(MInterceptedHost,flags);
  }

  void params_clear(clap_id param_id, clap_param_clear_flags flags) final {
    SAT_Print("\n");
    ext.params->clear(MInterceptedHost,param_id,flags);
  }

  void params_request_flush() final {
    SAT_Print("\n");
    ext.params->request_flush(MInterceptedHost);
  }

  bool posix_fd_support_register_fd(int fd, clap_posix_fd_flags_t flags) final {
    SAT_Print("\n");
    return ext.posix_fd_support->register_fd(MInterceptedHost,fd,flags);
  }

  bool posix_fd_support_modify_fd(int fd, clap_posix_fd_flags_t flags) final {
    SAT_Print("\n");
    return ext.posix_fd_support->modify_fd(MInterceptedHost,fd,flags);
  }

  bool posix_fd_support_unregister_fd(int fd) final {
    SAT_Print("\n");
    return ext.posix_fd_support->unregister_fd(MInterceptedHost,fd);
  }

  void preset_load_on_error(uint32_t location_kind, const char *location, const char *load_key,int32_t os_error, const char *msg) final {
    SAT_Print("\n");
    ext.preset_load->on_error(MInterceptedHost,location_kind,location,load_key,os_error,msg);
  }

  void preset_load_loaded(uint32_t location_kind, const char *location, const char *load_key) final {
    SAT_Print("\n");
    ext.preset_load->loaded(MInterceptedHost,location_kind,location,load_key);
  }

  void remote_controls_changed() final {
    SAT_Print("\n");
    ext.remote_controls->changed(MInterceptedHost);
  }

  void remote_controls_suggest_page(clap_id page_id) final {
    SAT_Print("\n");
    ext.remote_controls->suggest_page(MInterceptedHost,page_id);
  }

  bool resource_directory_request_directory(bool is_shared) final {
    SAT_Print("\n");
    return ext.resource_directory->request_directory(MInterceptedHost,is_shared);
  }

  void resource_directory_release_directory(bool is_shared) final {
    SAT_Print("\n");
    return ext.resource_directory->release_directory(MInterceptedHost,is_shared);
  }

  void state_mark_dirty() final {
    SAT_Print("\n");
    ext.state->mark_dirty(MInterceptedHost);
  }

  void surround_changed() final {
    SAT_Print("\n");
    ext.surround->changed(MInterceptedHost);
  }

  void tail_changed() final {
    SAT_Print("\n");
    ext.tail->changed(MInterceptedHost);
  }

  bool thread_check_is_main_thread() final {
    SAT_Print("\n");
    return ext.thread_check->is_main_thread(MInterceptedHost);
  }

  bool thread_check_is_audio_thread() final {
    SAT_Print("\n");
    return ext.thread_check->is_audio_thread(MInterceptedHost);
  }

  bool thread_pool_request_exec(uint32_t num_tasks) final {
    SAT_Print("\n");
    return ext.thread_pool->request_exec(MInterceptedHost,num_tasks);
  }

  bool timer_support_register_timer(uint32_t period_ms, clap_id *timer_id) final {
    SAT_Print("\n");
    return ext.timer_support->register_timer(MInterceptedHost,period_ms,timer_id);
  }

  bool timer_support_unregister_timer(clap_id timer_id) final {
    SAT_Print("\n");
    return ext.timer_support->unregister_timer(MInterceptedHost,timer_id);
  }

  bool track_info_get(clap_track_info_t *info) final {
    SAT_Print("\n");
    return ext.track_info->get(MInterceptedHost,info);
    return false; 
  }

  void transport_control_request_start() final {
    SAT_Print("\n");
    ext.transport_control->request_start(MInterceptedHost);
  }

  void transport_control_request_stop() final {
    SAT_Print("\n");
    ext.transport_control->request_stop(MInterceptedHost);
  }

  void transport_control_request_continue() final {
    SAT_Print("\n");
    ext.transport_control->request_continue(MInterceptedHost);
  }

  void transport_control_request_pause() final {
    SAT_Print("\n");
    ext.transport_control->request_pause(MInterceptedHost);
  }

  void transport_control_request_toggle_play() final {
    SAT_Print("\n");
    ext.transport_control->request_toggle_play(MInterceptedHost);
  }

  void transport_control_request_jump(clap_beattime position) final {
    SAT_Print("\n");
    ext.transport_control->request_jump(MInterceptedHost,position);
  }

  void transport_control_request_loop_region(clap_beattime start, clap_beattime duration) final {
    SAT_Print("\n");
    ext.transport_control->request_loop_region(MInterceptedHost,start,duration);
  }

  void transport_control_request_toggle_loop() final {
    SAT_Print("\n");
    ext.transport_control->request_toggle_loop(MInterceptedHost);
  }

  void transport_control_request_enable_loop(bool is_enabled) final {
    SAT_Print("\n");
    ext.transport_control->request_enable_loop(MInterceptedHost,is_enabled);
  }

  void transport_control_request_record(bool is_recording) final {
    SAT_Print("\n");
    ext.transport_control->request_record(MInterceptedHost,is_recording);
  }

  void transport_control_request_toggle_record() final {
    SAT_Print("\n");
    ext.transport_control->request_toggle_record(MInterceptedHost);
  }

  void triggers_rescan(clap_trigger_rescan_flags flags) final {
    SAT_Print("\n");
    ext.triggers->rescan(MInterceptedHost,flags);
  }

  void triggers_clear(clap_id trigger_id, clap_trigger_clear_flags flags) final {
    SAT_Print("\n");
    ext.triggers->clear(MInterceptedHost,trigger_id,flags);
  }

  double tuning_get_relative(clap_id tuning_id, int32_t channel, int32_t key, uint32_t sample_offset) final {
    SAT_Print("\n");
    return ext.tuning->get_relative(MInterceptedHost,tuning_id,channel,key,sample_offset);
  }

  bool tuning_should_play(clap_id tuning_id, int32_t channel, int32_t key) final {
    SAT_Print("\n");
    return ext.tuning->should_play(MInterceptedHost,tuning_id,channel,key);
  }

  uint32_t tuning_get_tuning_count() final {
    SAT_Print("\n");
    return ext.tuning->get_tuning_count(MInterceptedHost);
  }

  bool tuning_get_info(uint32_t tuning_index, clap_tuning_info_t *info) final {
    SAT_Print("\n");
    return ext.tuning->get_info(MInterceptedHost,tuning_index,info);
  }

  void voice_info_changed() final {  
    SAT_Print("\n");
    ext.voice_info->changed(MInterceptedHost);
  }

};


//----------------------------------------------------------------------
#endif

