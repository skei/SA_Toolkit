#ifndef sat_clap_plugin_included
#define sat_clap_plugin_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/clap/sat_clap.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_ClapPlugin {

  friend class SAT_Vst3Plugin;
  friend class SAT_Vst3Factory;

//------------------------------
private:
//------------------------------

  const clap_host_t*  MClapHost = nullptr;

//------------------------------
public:
//------------------------------

  SAT_ClapPlugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
    MPlugin.desc = ADescriptor;
    MClapHost = AHost;
  }

  //----------

  // note: we're calling 'delete this' in SAT_Plugin,destroy()

  virtual ~SAT_ClapPlugin() {
  }

//------------------------------
public:
//------------------------------

  const clap_plugin_descriptor_t* getClapDescriptor() {
    return MPlugin.desc;
  }

  //----------

  const clap_plugin_t* getClapPlugin() {
    return &MPlugin;
  }

  //----------

  const clap_host_t* getClapHost() {
    return MClapHost;
  }

//------------------------------
protected:
//------------------------------

  virtual bool                init() { return true; }
  virtual void                destroy() {}
  virtual bool                activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) { return true; }
  virtual void                deactivate() {}
  virtual bool                start_processing() { return true; }
  virtual void                stop_processing() {}
  virtual void                reset() {}
  virtual clap_process_status process(const clap_process_t *process) { return CLAP_PROCESS_CONTINUE; }
  virtual const void*         get_extension(const char *id) { return nullptr; }
  virtual void                on_main_thread() {}

  // extensions

  virtual uint32_t            audio_ports_count(bool is_input) { return 0; }
  virtual bool                audio_ports_get(uint32_t index, bool is_input, clap_audio_port_info_t *info) { return false; }
  virtual uint32_t            audio_ports_config_count() { return 0; }
  virtual bool                audio_ports_config_get(uint32_t index, clap_audio_ports_config_t *config) { return true; }
  virtual bool                audio_ports_config_select(clap_id config_id) { return true;}
  virtual bool                gui_is_api_supported(const char *api, bool is_floating) { return false; }
  virtual bool                gui_get_preferred_api(const char **api, bool *is_floating) { return false; }
  virtual bool                gui_create(const char *api, bool is_floating) { return false; }
  virtual void                gui_destroy() {}
  virtual bool                gui_set_scale(double scale) { return false; }
  virtual bool                gui_get_size(uint32_t *width, uint32_t *height) { return false; }
  virtual bool                gui_can_resize() { return false; }
  virtual bool                gui_get_resize_hints(clap_gui_resize_hints_t *hints) { return false; }
  virtual bool                gui_adjust_size(uint32_t *width, uint32_t *height) { return false; }
  virtual bool                gui_set_size(uint32_t width, uint32_t height) { return false; }
  virtual bool                gui_set_parent(const clap_window_t *window) { return false; }
  virtual bool                gui_set_transient(const clap_window_t *window) { return false; }
  virtual void                gui_suggest_title(const char *title) {}
  virtual bool                gui_show() { return false; }
  virtual bool                gui_hide() { return false; }
  virtual uint32_t            latency_get() { return 0; }
  virtual uint32_t            note_name_count() { return 0; }
  virtual bool                note_name_get(uint32_t index, clap_note_name_t *note_name) { return false; }
  virtual uint32_t            note_ports_count(bool is_input) { return 0; }
  virtual bool                note_ports_get(uint32_t index, bool is_input, clap_note_port_info_t *info) { return false; }
  virtual uint32_t            params_count() { return 0; }
  virtual bool                params_get_info(uint32_t param_index, clap_param_info_t *param_info) { return false; }
  virtual bool                params_get_value(clap_id param_id, double *out_value) { return false; }
  virtual bool                params_value_to_text(clap_id param_id, double value, char *out_buffer, uint32_t out_buffer_capacity) { return false; }
  virtual bool                params_text_to_value(clap_id param_id, const char *param_value_text, double *out_value) { return false; }
  virtual void                params_flush(const clap_input_events_t *in, const clap_output_events_t *out) {}
  virtual void                posix_fd_support_on_fd(int fd, clap_posix_fd_flags_t flags) {}
  virtual bool                render_has_hard_realtime_requirement() { return false; }
  virtual bool                render_set(clap_plugin_render_mode mode) { return false; }
  virtual bool                state_save(const clap_ostream_t *stream) { return false; }
  virtual bool                state_load(const clap_istream_t *stream) { return false; }
  virtual uint32_t            tail_get() { return 0; }
  virtual void                thread_pool_exec(uint32_t task_index) {}
  virtual void                timer_support_on_timer(clap_id timer_id) {}
  virtual bool                voice_info_get(clap_voice_info_t *info) { return false; }

  // drafts

  

  virtual bool                ambisonic_is_config_supported(const clap_ambisonic_config_t *config) { return false; }
  virtual bool                ambisonic_get_config(bool is_input, uint32_t port_index, clap_ambisonic_config_t *config) { return false; }
  virtual bool                audio_ports_activation_can_activate_while_processing() { return false; }
  virtual bool                audio_ports_activation_set_active(bool is_input, uint32_t port_index, bool is_active, uint32_t sample_size) { return false; }
//virtual void                check_for_update_check(bool include_preview) {}
  virtual bool                configurable_audio_ports_can_apply_configuration(const struct clap_audio_port_configuration_request *requests, uint32_t request_count) { return false; }
  virtual bool                configurable_audio_ports_apply_configuration(const struct clap_audio_port_configuration_request *requests, uint32_t request_count) { return false; }
  virtual bool                context_menu_populate(const clap_context_menu_target_t  *target, const clap_context_menu_builder_t *builder) { return false; }
  virtual bool                context_menu_perform(const clap_context_menu_target_t *target, clap_id action_id) { return false; }
//virtual bool                cv_get_channel_type(bool is_input, uint32_t port_index, uint32_t channel_index, uint32_t *channel_type) { return false; }
  virtual bool                extensible_audio_ports_add_port(bool is_input, uint32_t channel_count, const char *port_type, const void *port_details) { return false; }
  virtual bool                extensible_audio_ports_remove_port(bool is_input, uint32_t index) { return false; }
//virtual uint32_t            midi_mappings_count() { return 0; }
//virtual bool                midi_mappings_get(uint32_t index, clap_midi_mapping_t *mapping) { return false; }
  virtual void                param_indication_set_mapping(clap_id param_id, bool has_mapping, const clap_color_t *color, const char *label, const char *description) {}
  virtual void                param_indication_set_automation(clap_id param_id, uint32_t automation_state, const clap_color_t *color) {}
  virtual bool                preset_load_from_location(uint32_t location_kind, const char *location, const char *load_key) { return false; }
  virtual uint32_t            remote_controls_count() { return 0; }
  virtual bool                remote_controls_get(uint32_t page_index, clap_remote_controls_page_t *page) { return false; }
  virtual void                resource_directory_set_directory(const char *path, bool is_shared) {}
  virtual void                resource_directory_collect(bool all) {}
  virtual uint32_t            resource_directory_get_files_count() { return 0; }
  virtual int32_t             resource_directory_get_file_path(uint32_t index, char *path, uint32_t path_size) { return 0; }
  virtual bool                state_context_save(const clap_ostream_t *stream, uint32_t context_type) { return false; }
  virtual bool                state_context_load(const clap_istream_t *stream, uint32_t context_type) { return false; }
  virtual bool                surround_is_channel_mask_supported(uint64_t channel_mask) { return false; }
  virtual uint32_t            surround_get_channel_map(bool is_input, uint32_t port_index, uint8_t *channel_map, uint32_t channel_map_capacity) { return 0; }
  virtual void                track_info_changed() {}
  virtual uint32_t            triggers_count() { return 0; }
  virtual bool                triggers_get_info(uint32_t index, clap_trigger_info_t *trigger_info) { return false; }
  virtual void                tuning_changed() {}

//------------------------------
private: // plugin
//------------------------------

  static
  bool init_callback(const struct clap_plugin *plugin) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->init();
  }

  static
  void destroy_callback(const struct clap_plugin *plugin) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    plug->destroy();
    //delete this;
  }

  static
  bool activate_callback(const struct clap_plugin *plugin, double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->activate(sample_rate,min_frames_count,max_frames_count);
  }

  static
  void deactivate_callback(const struct clap_plugin *plugin) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    plug->deactivate();
  }

  static
  bool start_processing_callback(const struct clap_plugin *plugin) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->start_processing();
  }

  static
  void stop_processing_callback(const struct clap_plugin *plugin) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    plug->stop_processing();
  }

  static
  void reset_callback(const struct clap_plugin *plugin) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    plug->reset();
  }

  static
  clap_process_status process_callback(const struct clap_plugin *plugin, const clap_process_t *process) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->process(process);
  }

  static
  const void* get_extension_callback(const struct clap_plugin *plugin, const char *id) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->get_extension(id);
  }

  static
  void on_main_thread_callback(const struct clap_plugin *plugin) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    plug->on_main_thread();
  }

//------------------------------
protected:
//------------------------------

  //const
  clap_plugin_t MPlugin {
    .desc             = nullptr,
    .plugin_data      = this,
    .init             = init_callback,
    .destroy          = destroy_callback,
    .activate         = activate_callback,
    .deactivate       = deactivate_callback,
    .start_processing = start_processing_callback,
    .stop_processing  = stop_processing_callback,
    .reset            = reset_callback,
    .process          = process_callback,
    .get_extension    = get_extension_callback,
    .on_main_thread   = on_main_thread_callback
  };

//------------------------------
// extensions
//------------------------------

//------------------------------
private: // audio-ports
//------------------------------

  static
  uint32_t audio_ports_count_callback(const clap_plugin_t *plugin, bool is_input) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->audio_ports_count(is_input);
  }

  static
  bool audio_ports_get_callback(const clap_plugin_t *plugin, uint32_t index, bool is_input, clap_audio_port_info_t *info) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->audio_ports_get(index,is_input,info);
  }

//--------------------
protected:
//--------------------

  const clap_plugin_audio_ports_t MExtAudioPorts {
    .count  = audio_ports_count_callback,
    .get    = audio_ports_get_callback
  };

//------------------------------
private: // audio-ports-config
//------------------------------

  static
  uint32_t audio_ports_config_count_callback(const clap_plugin_t *plugin) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->audio_ports_config_count();
  }

  static
  bool audio_ports_config_get_callback(const clap_plugin_t *plugin, uint32_t index, clap_audio_ports_config_t *config) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->audio_ports_config_get(index,config);
  }
 
  static
  bool audio_ports_config_select_callback(const clap_plugin_t *plugin, clap_id config_id) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->audio_ports_config_select(config_id);
  }

//--------------------
protected:
//--------------------

  const clap_plugin_audio_ports_config_t MExtAudioPortsConfig {
    .count  = audio_ports_config_count_callback,
    .get    = audio_ports_config_get_callback,
    .select = audio_ports_config_select_callback
  };

//------------------------------
private: // gui
//------------------------------

  static
  bool gui_is_api_supported_callback(const clap_plugin_t *plugin, const char *api, bool is_floating) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->gui_is_api_supported(api,is_floating);
  }

  static
  bool gui_get_preferred_api_callback(const clap_plugin_t *plugin, const char **api, bool *is_floating) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->gui_get_preferred_api(api,is_floating);
  }

  static
  bool gui_create_callback(const clap_plugin_t *plugin, const char *api, bool is_floating) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->gui_create(api,is_floating);
  }

  static
  void gui_destroy_callback(const clap_plugin_t *plugin) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    plug->gui_destroy();
  }

  static
  bool gui_set_scale_callback(const clap_plugin_t *plugin, double scale) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->gui_set_scale(scale);
  }

  static
  bool gui_get_size_callback(const clap_plugin_t *plugin, uint32_t *width, uint32_t *height) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->gui_get_size(width,height);
  }

  static
  bool gui_can_resize_callback(const clap_plugin_t *plugin) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->gui_can_resize();
  }

  static
  bool gui_get_resize_hints_callback(const clap_plugin_t *plugin, clap_gui_resize_hints_t *hints) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->gui_get_resize_hints(hints);
  }

  static
  bool gui_adjust_size_callback(const clap_plugin_t *plugin, uint32_t *width, uint32_t *height) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->gui_adjust_size(width,height);
  }

  static
  bool gui_set_size_callback(const clap_plugin_t *plugin, uint32_t width, uint32_t height) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->gui_set_size(width,height);
  }

  static
  bool gui_set_parent_callback(const clap_plugin_t *plugin, const clap_window_t *window) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->gui_set_parent(window);
  }

  static
  bool gui_set_transient_callback(const clap_plugin_t *plugin, const clap_window_t *window) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->gui_set_transient(window);
  }

  static
  void gui_suggest_title_callback(const clap_plugin_t *plugin, const char *title) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    plug->gui_suggest_title(title);
  }

  static
  bool gui_show_callback(const clap_plugin_t *plugin) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->gui_show();
  }

  static
  bool gui_hide_callback(const clap_plugin_t *plugin) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->gui_hide();
  }

//--------------------
protected:
//--------------------

  const clap_plugin_gui_t MExtGui {
    .is_api_supported   = gui_is_api_supported_callback,
    .get_preferred_api  = gui_get_preferred_api_callback,
    .create             = gui_create_callback,
    .destroy            = gui_destroy_callback,
    .set_scale          = gui_set_scale_callback,
    .get_size           = gui_get_size_callback,
    .can_resize         = gui_can_resize_callback,
    .get_resize_hints   = gui_get_resize_hints_callback,
    .adjust_size        = gui_adjust_size_callback,
    .set_size           = gui_set_size_callback,
    .set_parent         = gui_set_parent_callback,
    .set_transient      = gui_set_transient_callback,
    .suggest_title      = gui_suggest_title_callback,
    .show               = gui_show_callback,
    .hide               = gui_hide_callback
  };

//------------------------------
private: // latency
//------------------------------

  static
  uint32_t latency_get_callback(const clap_plugin_t *plugin) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->latency_get();
  }

//--------------------
protected:
//--------------------

  const clap_plugin_latency_t MExtLatency {
    .get = latency_get_callback
  };

//------------------------------
private: // note-name
//------------------------------

  static
  uint32_t note_name_count_callback(const clap_plugin_t *plugin) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->note_name_count();
  }

  static
  bool note_name_get_callback(const clap_plugin_t *plugin, uint32_t index, clap_note_name_t *note_name) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->note_name_get(index,note_name);
  }

//--------------------
protected:
//--------------------

  const clap_plugin_note_name_t MExtNoteName {
    .count  = note_name_count_callback,
    .get    = note_name_get_callback
  };

//------------------------------
private: // note-ports
//------------------------------

  static
  uint32_t note_ports_count_callback(const clap_plugin_t *plugin, bool is_input) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->note_ports_count(is_input);
  }

  static
  bool note_ports_get_callback(const clap_plugin_t *plugin, uint32_t index, bool is_input, clap_note_port_info_t *info) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->note_ports_get(index,is_input,info);
  }

//--------------------
protected:
//--------------------

  const clap_plugin_note_ports_t MExtNotePorts {
    .count  = note_ports_count_callback,
    .get    = note_ports_get_callback
  };

//------------------------------
private: // params
//------------------------------

  static
  uint32_t params_count_callback(const clap_plugin_t *plugin) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->params_count();
  }

  static
  bool params_get_info_callback(const clap_plugin_t *plugin, uint32_t param_index, clap_param_info_t *param_info) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->params_get_info(param_index,param_info);
  }

  static
  bool params_get_value_callback(const clap_plugin_t *plugin, clap_id param_id, double *out_value) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->params_get_value(param_id,out_value);
  }

  static
  bool params_value_to_text_callback(const clap_plugin_t *plugin, clap_id param_id, double value, char *out_buffer, uint32_t out_buffer_capacity) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->params_value_to_text(param_id,value,out_buffer,out_buffer_capacity);
  }

  static
  bool params_text_to_value_callback(const clap_plugin_t *plugin, clap_id param_id, const char *param_value_text, double *out_value) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->params_text_to_value(param_id,param_value_text,out_value);
  }

  static
  void params_flush_callback(const clap_plugin_t *plugin, const clap_input_events_t *in, const clap_output_events_t *out) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    plug->params_flush(in,out);
  }

//--------------------
protected:
//--------------------

  const clap_plugin_params_t MExtParams {
    .count          = params_count_callback,
    .get_info       = params_get_info_callback,
    .get_value      = params_get_value_callback,
    .value_to_text  = params_value_to_text_callback,
    .text_to_value  = params_text_to_value_callback,
    .flush          = params_flush_callback
  };

//------------------------------
private: // posix-fd-support
//------------------------------

  static
  void posix_fd_support_on_fd_callback(const clap_plugin_t *plugin, int fd, clap_posix_fd_flags_t flags) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    plug->posix_fd_support_on_fd(fd,flags);
  }

//--------------------
protected:
//--------------------

  const clap_plugin_posix_fd_support_t MExtPosixFdSupport {
    .on_fd = posix_fd_support_on_fd_callback
  };

//------------------------------
private: // render
//------------------------------

  static
  bool render_has_hard_realtime_requirement_callback(const clap_plugin_t *plugin) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->render_has_hard_realtime_requirement();
  }

  static
  bool render_set_callback(const clap_plugin_t *plugin, clap_plugin_render_mode mode) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->render_set(mode);
  }

//--------------------
protected:
//--------------------

  const clap_plugin_render_t MExtRender {
    .has_hard_realtime_requirement  = render_has_hard_realtime_requirement_callback,
    .set                            = render_set_callback
  };

//------------------------------
private: // state
//------------------------------

  static
  bool state_save_callback(const clap_plugin_t *plugin, const clap_ostream_t *stream) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->state_save(stream);
  }

  static
  bool state_load_callback(const clap_plugin_t *plugin, const clap_istream_t *stream) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->state_load(stream);
  }

//--------------------
protected:
//--------------------

  const clap_plugin_state_t MExtState {
    .save = state_save_callback,
    .load = state_load_callback,
  };

//------------------------------
private: // tail
//------------------------------

  static
  uint32_t tail_get_callback(const clap_plugin_t *plugin) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->tail_get();
  }
  
//--------------------
protected:
//--------------------

  const clap_plugin_tail_t MExtTail {
    .get = tail_get_callback
  };

//------------------------------
private: // thread-pool
//------------------------------

  static
  void thread_pool_exec_callback(const clap_plugin_t *plugin, uint32_t task_index) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    plug->thread_pool_exec(task_index);
  }
  
//--------------------
protected:
//--------------------

  const clap_plugin_thread_pool_t MExtThreadPool {
    .exec = thread_pool_exec_callback
  };

//------------------------------
private: // timer-support
//------------------------------

  static
  void timer_support_on_timer_callback(const clap_plugin_t *plugin, clap_id timer_id) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    plug->timer_support_on_timer(timer_id);
  }

//--------------------
protected:
//--------------------

  const clap_plugin_timer_support_t MExtTimerSupport {
    .on_timer = timer_support_on_timer_callback
  };

//------------------------------
private: // voice-info
//------------------------------

  static
  bool voice_info_get_callback(const clap_plugin_t *plugin, clap_voice_info_t *info) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->voice_info_get(info);
  }

//--------------------
protected:
//--------------------

  const clap_plugin_voice_info_t MExtVoiceInfo {
    .get = voice_info_get_callback
  };

//------------------------------
// drafts
//------------------------------

//------------------------------
private: // draft: ambisonic
//------------------------------

  static
  bool ambisonic_is_config_supported_callback(const clap_plugin_t *plugin, const clap_ambisonic_config_t *config) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->ambisonic_is_config_supported(config);
  }

  static
  bool ambisonic_get_config_callback(const clap_plugin_t *plugin, bool is_input, uint32_t port_index, clap_ambisonic_config_t *config) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->ambisonic_get_config(is_input,port_index,config);
  }

//--------------------
protected:
//--------------------

  const clap_plugin_ambisonic_t MExtAmbisonic {
    .is_config_supported  = ambisonic_is_config_supported_callback,
    .get_config           = ambisonic_get_config_callback
  };

//------------------------------
private: // draft: audio-ports-activation
//------------------------------

  static
  bool audio_ports_activation_can_activate_while_processing_callback(const clap_plugin_t *plugin) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->audio_ports_activation_can_activate_while_processing();
  }

  static
  bool audio_ports_activation_set_active_callback(const clap_plugin_t *plugin, bool is_input, uint32_t port_index, bool is_active, uint32_t sample_size) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->audio_ports_activation_set_active(is_input,port_index,is_active,sample_size);
  }

//--------------------
protected:
//--------------------

  const clap_plugin_audio_ports_activation_t MExtAudioPortsActivation {
    .can_activate_while_processing  = audio_ports_activation_can_activate_while_processing_callback,
    .set_active                     = audio_ports_activation_set_active_callback
  };

//------------------------------
private: // draft: check-for-update
//------------------------------

  // static
  // void check_for_update_check_callback(const clap_plugin_t *plugin, bool include_preview) {
  //   SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
  //   plug->check_for_update_check(include_preview);
  // }

//--------------------
protected:
//--------------------

  // const clap_plugin_check_for_update_t MExtCheckForUpdate {
  //   .check = check_for_update_check_callback
  // };

//------------------------------
private: // draft: configurable-audio-ports
//------------------------------

  static
  bool configurable_audio_ports_can_apply_configuration_callback(const clap_plugin_t *plugin, const struct clap_audio_port_configuration_request *requests, uint32_t request_count) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->configurable_audio_ports_can_apply_configuration(requests,request_count);
  }

  static
  bool configurable_audio_ports_apply_configuration_callback(const clap_plugin_t *plugin, const struct clap_audio_port_configuration_request *requests, uint32_t request_count) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->configurable_audio_ports_apply_configuration(requests,request_count);
  }

//--------------------
protected:
//--------------------

  const clap_plugin_configurable_audio_ports_t MExtConfigurableAudioPorts {
    .can_apply_configuration  = configurable_audio_ports_can_apply_configuration_callback,
    .apply_configuration      = configurable_audio_ports_apply_configuration_callback
  };

//------------------------------
private: // draft: context-menu
//------------------------------

  static
  bool context_menu_populate_callback(const clap_plugin_t *plugin, const clap_context_menu_target_t  *target, const clap_context_menu_builder_t *builder) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->context_menu_populate(target,builder);
  }

  static
  bool context_menu_perform_callback(const clap_plugin_t *plugin, const clap_context_menu_target_t *target, clap_id action_id) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->context_menu_perform(target,action_id);
  }

//--------------------
protected:
//--------------------

  const clap_plugin_context_menu_t MExtContextMenu {
    .populate = context_menu_populate_callback,
    .perform  = context_menu_perform_callback
  };

//------------------------------
private: // draft: cv
//------------------------------

  // static
  // bool cv_get_channel_type_callback(const clap_plugin_t *plugin, bool is_input, uint32_t port_index, uint32_t channel_index, uint32_t *channel_type) {
  //   SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
  //   return plug->cv_get_channel_type(is_input,port_index,channel_index,channel_type);
  // }

//--------------------
protected:
//--------------------

  // const clap_plugin_cv_t MExtCv {
  //   .get_channel_type = cv_get_channel_type_callback
  // };

//------------------------------
private: // draft: extensible-audio-ports
//------------------------------

  static
  bool extensible_audio_ports_add_port_callback(const clap_plugin_t *plugin, bool is_input, uint32_t channel_count, const char *port_type, const void *port_details) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->extensible_audio_ports_add_port(is_input,channel_count,port_type,port_details);
  }

  static
  bool extensible_audio_ports_remove_port_callback(const clap_plugin_t *plugin, bool is_input, uint32_t index) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->extensible_audio_ports_remove_port(is_input,index);
  }

//--------------------
protected:
//--------------------

  const clap_plugin_extensible_audio_ports_t MExtExtensibleAudioPorts {
    .add_port     = extensible_audio_ports_add_port_callback,
    .remove_port  = extensible_audio_ports_remove_port_callback
  };

//------------------------------
private: // draft: midi-mappings
//------------------------------

  // static
  // uint32_t midi_mappings_count_callback(const clap_plugin_t *plugin) {
  //   SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
  //   return plug->midi_mappings_count();
  // }
  //  
  // static
  // bool midi_mappings_get_callback(const clap_plugin_t *plugin, uint32_t index, clap_midi_mapping_t *mapping) {
  //   SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
  //   return plug->midi_mappings_get(index,mapping);
  // }
    
//--------------------
protected:
//--------------------

  // const clap_plugin_midi_mappings_t MExtMidiMappings {
  //   .count  = midi_mappings_count_callback, 
  //   .get    = midi_mappings_get_callback
  // };

//------------------------------
private: // draft: param-indication
//------------------------------

  static
  void param_indication_set_mapping_callback(const clap_plugin_t *plugin, clap_id param_id, bool has_mapping, const clap_color_t *color, const char *label, const char *description) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->param_indication_set_mapping(param_id,has_mapping,color,label,description);
  }

  static
  void param_indication_set_automation_callback(const clap_plugin_t *plugin, clap_id param_id, uint32_t automation_state, const clap_color_t *color) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->param_indication_set_automation(param_id,automation_state,color);
  }

//--------------------
protected:
//--------------------

  const clap_plugin_param_indication_t MExtParamIndication {
    .set_mapping    = param_indication_set_mapping_callback,
    .set_automation = param_indication_set_automation_callback
  };

//------------------------------
private: // draft: preset-load
//------------------------------

  static
  bool preset_load_from_location_callback(const clap_plugin_t *plugin, uint32_t location_kind, const char *location, const char *load_key) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->preset_load_from_location(location_kind,location,load_key);
  }

//--------------------
protected:
//--------------------

  const clap_plugin_preset_load_t MExtPresetLoad {
    .from_location = preset_load_from_location_callback
  };

//------------------------------
private: // draft: remote-controls
//------------------------------

  static
  uint32_t remote_controls_count_callback(const clap_plugin_t *plugin) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->remote_controls_count();
  }

  static
  bool remote_controls_get_callback(const clap_plugin_t *plugin, uint32_t page_index, clap_remote_controls_page_t *page) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->remote_controls_get(page_index,page);
  }

//--------------------
protected:
//--------------------

  const clap_plugin_remote_controls_t MExtRemoteControls {
    .count  = remote_controls_count_callback,
    .get    = remote_controls_get_callback
  };

//------------------------------
private: // draft: resource-directory
//------------------------------

  static
  void resource_directory_set_directory_callback(const clap_plugin_t *plugin, const char *path, bool is_shared) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    plug->resource_directory_set_directory(path,is_shared);
  }

  static
  void resource_directory_collect_callback(const clap_plugin_t *plugin, bool all) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    plug->resource_directory_collect(all);
  }

  static
  uint32_t resource_directory_get_files_count_callback(const clap_plugin_t *plugin) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->resource_directory_get_files_count();
  }

  static
  int32_t resource_directory_get_file_path_callback(const clap_plugin_t *plugin, uint32_t index, char *path, uint32_t path_size) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->resource_directory_get_file_path(index,path,path_size);
  }

//--------------------
protected:
//--------------------

  const clap_plugin_resource_directory_t MExtResourceDirectory {
    .set_directory    = resource_directory_set_directory_callback,
    .collect          = resource_directory_collect_callback,
    .get_files_count  = resource_directory_get_files_count_callback,
    .get_file_path    = resource_directory_get_file_path_callback
  };

//------------------------------
private: // draft: state-context
//------------------------------

  static
  bool state_context_save_callback(const clap_plugin_t *plugin, const clap_ostream_t *stream, uint32_t context_type) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->state_context_save(stream,context_type);
  }

  static
  bool state_context_load_callback(const clap_plugin_t *plugin, const clap_istream_t *stream, uint32_t context_type) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->state_context_load(stream,context_type);
  }

//--------------------
protected:
//--------------------

  const clap_plugin_state_context_t MExtStateContext {
    .save = state_context_save_callback,
    .load = state_context_load_callback
  };

//------------------------------
private: // draft: surround
//------------------------------

  static
  bool surround_is_channel_mask_supported_callback(const clap_plugin_t *plugin, uint64_t channel_mask) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->surround_is_channel_mask_supported(channel_mask);
  }

  static
  uint32_t surround_get_channel_map_callback(const clap_plugin_t *plugin, bool is_input, uint32_t port_index, uint8_t *channel_map, uint32_t channel_map_capacity) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->surround_get_channel_map(is_input,port_index,channel_map,channel_map_capacity);
  }

//--------------------
protected:
//--------------------

  const clap_plugin_surround_t MExtSurround {
    surround_is_channel_mask_supported_callback,
    surround_get_channel_map_callback
  };

//------------------------------
private: // draft: track-info
//------------------------------

  static
  void track_info_changed_callback(const clap_plugin_t *plugin) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    plug->track_info_changed();
  }

//--------------------
protected:
//--------------------

  const clap_plugin_track_info_t MExtTrackInfo {
    .changed = track_info_changed_callback
  };

//------------------------------
private: // draft: triggers
//------------------------------

  static
  uint32_t triggers_count_callback(const clap_plugin_t *plugin) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->triggers_count();
  }

  static
  bool triggers_get_info_callback(const clap_plugin_t *plugin, uint32_t index, clap_trigger_info_t *trigger_info) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->triggers_get_info(index,trigger_info);
  }

//--------------------
protected:
//--------------------

  const clap_plugin_triggers_t MExtTriggers {
    .count    = triggers_count_callback,
    .get_info = triggers_get_info_callback
  };

//------------------------------
private: // draft: tuning
//------------------------------

  static
  void tuning_changed_callback(const clap_plugin_t *plugin) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    plug->tuning_changed();
  }

//--------------------
protected:
//--------------------

  const clap_plugin_tuning_t MExtTuning {
    .changed = tuning_changed_callback
  };

};

//----------------------------------------------------------------------
#endif

