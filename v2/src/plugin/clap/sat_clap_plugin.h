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

//------------------------------
public:
//------------------------------

  SAT_ClapPlugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
  }

  virtual ~SAT_ClapPlugin() {
  }

  const clap_plugin_t* getClapPlugin() {
    return &MPlugin;
  }

//------------------------------
protected:
//------------------------------

  virtual bool init() { return true; }
  virtual void destroy() {}
  virtual bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) { return true; }
  virtual void deactivate() {}
  virtual bool start_processing() { return true; }
  virtual void stop_processing() {}
  virtual void reset() {}
  virtual clap_process_status process(const clap_process_t *process) { return CLAP_PROCESS_CONTINUE; }
  virtual const void* get_extension(const char *id) { return nullptr; }
  virtual void on_main_thread() {}

//------------------------------
private: // plugin
//------------------------------

  static
  bool clap_plugin_init_callback(const struct clap_plugin *plugin) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->init();
  }

  static
  void clap_plugin_destroy_callback(const struct clap_plugin *plugin) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    plug->destroy();
  }

  static
  bool clap_plugin_activate_callback(const struct clap_plugin *plugin, double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->activate(sample_rate,min_frames_count,max_frames_count);
  }

  static
  void clap_plugin_deactivate_callback(const struct clap_plugin *plugin) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    plug->deactivate();
  }

  static
  bool clap_plugin_start_processing_callback(const struct clap_plugin *plugin) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->start_processing();
  }

  static
  void clap_plugin_stop_processing_callback(const struct clap_plugin *plugin) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    plug->stop_processing();
  }

  static
  void clap_plugin_reset_callback(const struct clap_plugin *plugin) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    plug->reset();
  }

  static
  clap_process_status clap_plugin_process_callback(const struct clap_plugin *plugin, const clap_process_t *process) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->process(process);
  }

  static
  const void* clap_plugin_get_extension_callback(const struct clap_plugin *plugin, const char *id) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    return plug->get_extension(id);
  }

  static
  void clap_plugin_on_main_thread_callback(const struct clap_plugin *plugin) {
    SAT_ClapPlugin* plug = (SAT_ClapPlugin*)plugin->plugin_data;
    plug->on_main_thread();
  }

  //------------------------------

  const
  struct clap_plugin MPlugin {
    .desc             = nullptr,
    .plugin_data      = this,
    .init             = clap_plugin_init_callback,
    .destroy          = clap_plugin_destroy_callback,
    .activate         = clap_plugin_activate_callback,
    .deactivate       = clap_plugin_deactivate_callback,
    .start_processing = clap_plugin_start_processing_callback,
    .stop_processing  = clap_plugin_stop_processing_callback,
    .reset            = clap_plugin_reset_callback,
    .process          = clap_plugin_process_callback,
    .get_extension    = clap_plugin_get_extension_callback,
    .on_main_thread   = clap_plugin_on_main_thread_callback
  };

//------------------------------
private: // extensions
//------------------------------

  // typedef struct clap_plugin_audio_ports_config {
  //   uint32_t count(const clap_plugin_t *plugin);
  //   bool get(const clap_plugin_t *plugin, uint32_t index, clap_audio_ports_config_t *config);
  //   bool select(const clap_plugin_t *plugin, clap_id config_id);
  // } clap_plugin_audio_ports_config_t;

  // typedef struct clap_plugin_audio_ports {
  //   uint32_t count(const clap_plugin_t *plugin, bool is_input);
  //   bool get(const clap_plugin_t *plugin, uint32_t index, bool is_input, clap_audio_port_info_t *info);
  // } clap_plugin_audio_ports_t;

  // typedef struct clap_plugin_gui {
  //   bool is_api_supported(const clap_plugin_t *plugin, const char *api, bool is_floating);
  //   bool get_preferred_api(const clap_plugin_t *plugin, const char **api, bool *is_floating);
  //   bool create(const clap_plugin_t *plugin, const char *api, bool is_floating);
  //   void destroy(const clap_plugin_t *plugin);
  //   bool set_scale(const clap_plugin_t *plugin, double scale);
  //   bool get_size(const clap_plugin_t *plugin, uint32_t *width, uint32_t *height);
  //   bool can_resize(const clap_plugin_t *plugin);
  //   bool get_resize_hints(const clap_plugin_t *plugin, clap_gui_resize_hints_t *hints);
  //   bool adjust_size(const clap_plugin_t *plugin, uint32_t *width, uint32_t *height);
  //   bool set_size(const clap_plugin_t *plugin, uint32_t width, uint32_t height);
  //   bool set_parent(const clap_plugin_t *plugin, const clap_window_t *window);
  //   bool set_transient(const clap_plugin_t *plugin, const clap_window_t *window);
  //   void suggest_title(const clap_plugin_t *plugin, const char *title);
  //   bool show(const clap_plugin_t *plugin);
  //   bool hide(const clap_plugin_t *plugin);
  // } clap_plugin_gui_t;

  // typedef struct clap_plugin_latency {
  //   uint32_t get(const clap_plugin_t *plugin);
  // } clap_plugin_latency_t;

  // typedef struct clap_plugin_note_name {
  //   uint32_t count(const clap_plugin_t *plugin);
  //   bool get(const clap_plugin_t *plugin, uint32_t index, clap_note_name_t *note_name);
  // } clap_plugin_note_name_t;

  // typedef struct clap_plugin_note_ports {
  //   uint32_t count(const clap_plugin_t *plugin, bool is_input);
  //   bool get(const clap_plugin_t *plugin, uint32_t index, bool is_input, clap_note_port_info_t *info);
  // } clap_plugin_note_ports_t;

  // typedef struct clap_plugin_params {
  //   uint32_t count(const clap_plugin_t *plugin);
  //   bool get_info(const clap_plugin_t *plugin, uint32_t param_index, clap_param_info_t *param_info);
  //   bool get_value(const clap_plugin_t *plugin, clap_id param_id, double *out_value);
  //   bool value_to_text(const clap_plugin_t *plugin, clap_id param_id, double value, char *out_buffer, uint32_t out_buffer_capacity);
  //   bool text_to_value(const clap_plugin_t *plugin, clap_id param_id, const char *param_value_text, double *out_value);
  //   void flush(const clap_plugin_t *plugin, const clap_input_events_t *in, const clap_output_events_t *out);
  // } clap_plugin_params_t;

  // typedef struct clap_plugin_posix_fd_support {
  //   void on_fd(const clap_plugin_t *plugin, int fd, clap_posix_fd_flags_t flags);
  // } clap_plugin_posix_fd_support_t;

  // typedef struct clap_plugin_render {
  //   bool has_hard_realtime_requirement(const clap_plugin_t *plugin);
  //   bool set(const clap_plugin_t *plugin, clap_plugin_render_mode mode);
  // } clap_plugin_render_t;

  // typedef struct clap_plugin_state {
  //   bool save(const clap_plugin_t *plugin, const clap_ostream_t *stream);
  //   bool load(const clap_plugin_t *plugin, const clap_istream_t *stream);
  // } clap_plugin_state_t;

  // typedef struct clap_plugin_tail {
  //   uint32_t get(const clap_plugin_t *plugin);
  // } clap_plugin_tail_t;

  // typedef struct clap_plugin_thread_pool {
  //   void exec(const clap_plugin_t *plugin, uint32_t task_index);
  // } clap_plugin_thread_pool_t;

  // typedef struct clap_plugin_timer_support {
  //   void on_timer(const clap_plugin_t *plugin, clap_id timer_id);
  // } clap_plugin_timer_support_t;

  // typedef struct clap_plugin_voice_info {
  //   bool get(const clap_plugin_t *plugin, clap_voice_info_t *info);
  // } clap_plugin_voice_info_t;  

//------------------------------
private: // draft
//------------------------------

  // typedef struct clap_plugin_ambisonic {
  //   bool(CLAP_ABI *is_config_supported)(const clap_plugin_t *plugin, const clap_ambisonic_config_t *config);
  //   bool(CLAP_ABI *get_config)(const clap_plugin_t *plugin, bool is_input, uint32_t port_index, clap_ambisonic_config_t *config);
  // } clap_plugin_ambisonic_t;

  // typedef struct clap_plugin_audio_ports_activation {
  //   bool(CLAP_ABI *can_activate_while_processing)(const clap_plugin_t *plugin);
  //   bool(CLAP_ABI *set_active)(const clap_plugin_t *plugin, bool is_input, uint32_t port_index, bool is_active, uint32_t sample_size);
  // } clap_plugin_audio_ports_activation_t;

  // typedef struct clap_plugin_check_for_update {
  //   void(CLAP_ABI *check)(const clap_plugin_t *plugin, bool include_preview);
  // } clap_plugin_check_for_update_t;

  // typedef struct clap_plugin_configurable_audio_ports {
  //   bool(CLAP_ABI *can_apply_configuration)( const clap_plugin_t *plugin, const struct clap_audio_port_configuration_request *requests, uint32_t request_count);
  //   bool(CLAP_ABI *apply_configuration)(const clap_plugin_t *plugin, const struct clap_audio_port_configuration_request *requests, uint32_t request_count);
  // } clap_plugin_configurable_audio_ports_t;

  // typedef struct clap_plugin_context_menu {
  //   bool(CLAP_ABI *populate)(const clap_plugin_t *plugin, const clap_context_menu_target_t  *target, const clap_context_menu_builder_t *builder);
  //   bool(CLAP_ABI *perform)(const clap_plugin_t *plugin, const clap_context_menu_target_t *target, clap_id action_id);
  // } clap_plugin_context_menu_t;

  // typedef struct clap_plugin_cv {
  //   bool(CLAP_ABI *get_channel_type)(const clap_plugin_t *plugin, bool is_input, uint32_t port_index, uint32_t channel_index, uint32_t *channel_type);
  // } clap_plugin_cv_t;

  // typedef struct clap_plugin_extensible_audio_ports {
  //   bool(CLAP_ABI *add_port)(const clap_plugin_t *plugin, bool is_input, uint32_t channel_count, const char *port_type, const void *port_details);
  //   bool(CLAP_ABI *remove_port)(const clap_plugin_t *plugin, bool is_input, uint32_t index);
  // } clap_plugin_extensible_audio_ports_t;

  // typedef struct clap_plugin_midi_mappings {
  //   uint32_t(CLAP_ABI *count)(const clap_plugin_t *plugin);
  //   bool(CLAP_ABI *get)(const clap_plugin_t *plugin, uint32_t index, clap_midi_mapping_t *mapping);
  // } clap_plugin_midi_mappings_t;

  // typedef struct clap_plugin_param_indication {
  //   void(CLAP_ABI *set_mapping)(const clap_plugin_t *plugin, clap_id param_id, bool has_mapping, const clap_color_t *color, const char *label, const char *description);
  //   void(CLAP_ABI *set_automation)(const clap_plugin_t *plugin, clap_id param_id, uint32_t automation_state, const clap_color_t *color);
  // } clap_plugin_param_indication_t;

  // typedef struct clap_plugin_preset_load {
  //   bool(CLAP_ABI *from_location)(const clap_plugin_t *plugin, uint32_t location_kind, const char *location, const char *load_key);
  // } clap_plugin_preset_load_t;

  // typedef struct clap_plugin_remote_controls {
  //   uint32_t(CLAP_ABI *count)(const clap_plugin_t *plugin);
  //   bool(CLAP_ABI *get)(const clap_plugin_t *plugin, uint32_t page_index, clap_remote_controls_page_t *page);
  // } clap_plugin_remote_controls_t;

  // typedef struct clap_plugin_resource_directory {
  //   void(CLAP_ABI *set_directory)(const clap_plugin_t *plugin, const char *path, bool is_shared);
  //   void(CLAP_ABI *collect)(const clap_plugin_t *plugin, bool all);
  //   uint32_t(CLAP_ABI *get_files_count)(const clap_plugin_t *plugin);
  //   int32_t(CLAP_ABI *get_file_path)(const clap_plugin_t *plugin, uint32_t index, char *path, uint32_t path_size);
  // } clap_plugin_resource_directory_t;

  // typedef struct clap_plugin_state_context {
  //   bool(CLAP_ABI *save)(const clap_plugin_t *plugin, const clap_ostream_t *stream, uint32_t context_type);
  //   bool(CLAP_ABI *load)(const clap_plugin_t *plugin, const clap_istream_t *stream, uint32_t context_type);
  // } clap_plugin_state_context_t;

  // typedef struct clap_plugin_surround {
  //   bool(CLAP_ABI *is_channel_mask_supported)(const clap_plugin_t *plugin, uint64_t channel_mask);
  //   uint32_t(CLAP_ABI *get_channel_map)(const clap_plugin_t *plugin, bool is_input, uint32_t port_index, uint8_t *channel_map, uint32_t channel_map_capacity);
  // } clap_plugin_surround_t;

  // typedef struct clap_plugin_track_info {
  //   void(CLAP_ABI *changed)(const clap_plugin_t *plugin);
  // } clap_plugin_track_info_t;

  // typedef struct clap_plugin_triggers {
  //   uint32_t(CLAP_ABI *count)(const clap_plugin_t *plugin);
  //   bool(CLAP_ABI *get_info)(const clap_plugin_t *plugin, uint32_t index, clap_trigger_info_t *trigger_info);
  // } clap_plugin_triggers_t;

  // typedef struct clap_plugin_tuning {
  //   void(CLAP_ABI *changed)(const clap_plugin_t *plugin);
  // } clap_plugin_tuning_t;

};

//----------------------------------------------------------------------
#endif

