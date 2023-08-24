#ifndef sa_interceptor_included
#define sa_interceptor_included
//----------------------------------------------------------------------

#include <string.h>
#include "base/sat.h"
#include "plugin/clap/sat_clap_plugin.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t my_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "skei.audio/sa_interceptor",
  .name         = "sa_interceptor",
  .vendor       = "skei.audio",
  .url          = "",
  .manual_url   = "",
  .support_url  = "",
  .version      = "0.0.1",
  .description  = "",
  .features     = (const char* []) {
                    CLAP_PLUGIN_FEATURE_AUDIO_EFFECT,
                    nullptr
                  }
};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class sa_interceptor_plugin
: public SAT_ClapPlugin {

  //friend class SAT_Plugin;

//------------------------------
public:
//------------------------------

  sa_interceptor_plugin(const clap_plugin_descriptor_t* ADescriptor)
  : SAT_ClapPlugin(ADescriptor) {
  }

  //----------

  virtual ~sa_interceptor_plugin() {
  }

//------------------------------
public:
//------------------------------

  bool init() final {
    return false;
  }

  void destroy() final {
  }

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    return false;
  }

  void deactivate() final {
  }

  bool start_processing() final {
    return false;
  }

  void stop_processing() final {
  }

  void reset() final {
  }

  clap_process_status process(const clap_process_t* process) final {
    return CLAP_PROCESS_SLEEP;
  }

  const void*         get_extension(const char *id) final {
    return nullptr;
  }

  void on_main_thread() final {
  }
  
//------------------------------
public:
//------------------------------

  bool ambisonic_get_info(bool is_input,  uint32_t port_index, clap_ambisonic_info_t *info) final {
    return false;
  }

  uint32_t audio_ports_count(bool is_input) final {
    return 0;
  }

  bool audio_ports_get(uint32_t index, bool is_input, clap_audio_port_info_t *info) final {
    return false;
  }

  bool audio_ports_activation_can_activate_while_processing() final {
    return false;
  }

  bool audio_ports_activation_set_active(bool is_input, uint32_t port_index, bool is_active) final {
    return false;
  }

  uint32_t audio_ports_config_count() final {
    return 0;
  }

  bool audio_ports_config_get(uint32_t index, clap_audio_ports_config_t *config) final {
    return false;
  }

  bool audio_ports_config_select(clap_id config_id) final {
    return false;
  }

  void check_for_updates_check(bool include_preview) final {
  }

  bool configurable_audio_ports_is_port_configurable(bool is_input, uint32_t port_index) final {
    return false;
  }

  bool configurable_audio_ports_request_configuration(const struct clap_audio_port_configuration_request *requests,uint32_t request_count) final {
    return false;
  }

  bool context_menu_populate(const clap_context_menu_target_t *target, const clap_context_menu_builder_t *builder) final {
    return false;
  }

  bool context_menu_perform(const clap_context_menu_target_t *target, clap_id action_id) final {
    return false;
  }

  bool cv_get_channel_type(bool is_input, uint32_t port_index, uint32_t channel_index, uint32_t *channel_type) final {
    return false;
  }

  bool extensible_audio_ports_add_port(bool is_input, uint32_t channel_count, const char *port_type, const void *port_details) final {
    return false;
  }

  bool extensible_audio_ports_remove_port(bool is_input, uint32_t index) final {
    return false;
  }

  bool gui_is_api_supported(const char *api, bool is_floating) final {
    return false;
  }

  bool gui_get_preferred_api(const char **api, bool *is_floating) final {
    return false;
  }

  bool gui_create(const char *api, bool is_floating) final {
    return false;
  }

  void gui_destroy() final {
  }

  bool gui_set_scale(double scale) final {
    return false;
  }

  bool gui_get_size(uint32_t *width, uint32_t *height) final {
    return false;
  }

  bool gui_can_resize() final {
    return false;
  }

  bool gui_get_resize_hints(clap_gui_resize_hints_t *hints) final {
    return false;
  }

  bool gui_adjust_size(uint32_t *width, uint32_t *height) final {
    return false;
  }

  bool gui_set_size(uint32_t width, uint32_t height) final {
    return false;
  }

  bool gui_set_parent(const clap_window_t *window) final {
    return false;
  }

  bool gui_set_transient(const clap_window_t *window) final {
    return false;
  }

  void gui_suggest_title(const char *title) final {
  }

  bool gui_show() final {
    return false;
  }

  bool gui_hide() final {
    return false;
  }

  uint32_t latency_get() final {
    return 0;
  }

  uint32_t midi_mappings_count() final {
    return 0;
  }

  bool midi_mappings_get(uint32_t index, clap_midi_mapping_t *mapping) final {
    return false;
  }

  uint32_t note_name_count() final {
    return 0;
  }

  bool note_name_get(uint32_t index, clap_note_name_t *note_name) final {
    return false;
  }

  uint32_t note_ports_count(bool is_input) final {
    return 0;
  }

  bool note_ports_get(uint32_t index, bool is_input, clap_note_port_info_t* info) final {
    return false;
  }

  void param_indication_set_mapping(clap_id param_id, bool has_mapping, const clap_color_t *color, const char *label, const char *description) final {
  }

  void param_indication_set_automation(clap_id param_id, uint32_t automation_state, const clap_color_t *color) final {
  }

  uint32_t params_count() final {
    return 0;
  }

  bool params_get_info(uint32_t param_index, clap_param_info_t *param_info) final {
    return false;
  }

  bool params_get_value(clap_id param_id, double *value) final {
    return false;
  }

  bool params_value_to_text(clap_id param_id, double value, char *display, uint32_t size) final {
    return false;
  }

  bool params_text_to_value(clap_id param_id, const char *display, double *value) final {
    return false;
  }

  void params_flush(const clap_input_events_t *in, const clap_output_events_t *out) final {
  }

  void posix_fd_support_on_fd(int fd, clap_posix_fd_flags_t flags) final {
  }

  bool preset_load_from_location(uint32_t location_kind, const char *location, const char *load_key) final {
    return false;
  }

  uint32_t remote_controls_count() final {
    return 0;
  }

  bool remote_controls_get(uint32_t page_index, clap_remote_controls_page_t *page) final {
    return false;
  }

  bool render_has_hard_realtime_requirement() final {
    return false;
  }

  bool render_set(clap_plugin_render_mode mode) final {
    return false;
  }

  void resource_directory_set_directory(const char *path, bool is_shared) final {
  }

  void resource_directory_collect(bool all) final {
  }

  uint32_t resource_directory_get_files_count() final {
    return 0;
  }

  int32_t resource_directory_get_file_path(uint32_t index, char *path, uint32_t path_size) final {
    return -1;
  }

  bool state_save(const clap_ostream_t *stream) final {
    return false;
  }

  bool state_load(const clap_istream_t *stream) final {
    return false;
  }

  bool state_context_save(const clap_ostream_t *stream, uint32_t context_type) final {
    return false;
  }

  bool state_context_load(const clap_istream_t *stream, uint32_t context_type) final {
    return false;
  }

  uint32_t surround_get_channel_map(bool is_input, uint32_t port_index, uint8_t *channel_map, uint32_t channel_map_capacity) final {
    return 0;
  }

  void surround_changed() final {
  }

  uint32_t tail_get() final {
    return 0;
  }

  void thread_pool_exec(uint32_t task_index) final {
  }

  void timer_support_on_timer(clap_id timer_id) final {
  }

  void track_info_changed() final {
  }

  uint32_t triggers_count() final {
    return 0;
  }

  bool triggers_get_info(uint32_t index, clap_trigger_info_t *trigger_info) final {
    return false;
  }

  void tuning_changed() final {
  }

  bool voice_info_get(clap_voice_info_t *info) final {
    return false;
  }

};

//----------------------------------------------------------------------
//
// factory
//
//----------------------------------------------------------------------

uint32_t my_factory_get_plugin_count_callback(const struct clap_plugin_factory *factory) {
  SAT_Print("\n");
  return 1;
}

//----------

const clap_plugin_descriptor_t* my_factory_get_plugin_descriptor_callback(const struct clap_plugin_factory *factory, uint32_t index) {
  SAT_Print("index: %i\n",index);
  if (index == 0) {
    return &my_descriptor;
  }
  return nullptr;
}

//----------

const clap_plugin_t* my_factory_create_plugin_callback(const struct clap_plugin_factory *factory, const clap_host_t *host, const char *plugin_id) {
  SAT_Print("plugin_id: %s\n",plugin_id);
  if (strcmp(plugin_id,"skei.audio/sa_interceptor") == 0) {
    sa_interceptor_plugin * plugin = new sa_interceptor_plugin(&my_descriptor);
    return plugin->getPlugin();
  }
  return nullptr;
}

//------------------------------

const clap_plugin_factory_t my_factory {
  .get_plugin_count      = my_factory_get_plugin_count_callback,
  .get_plugin_descriptor = my_factory_get_plugin_descriptor_callback,
  .create_plugin         = my_factory_create_plugin_callback
};

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

bool clap_entry_init_callback(const char *plugin_path) {
  SAT_Print("path: %s\n",plugin_path);
  return true;
}

//----------

void clap_entry_deinit_callback(void) {
  SAT_Print("\n");
}

//----------

const void* clap_entry_get_factory_callback(const char* factory_id) {
  SAT_Print("factory_id: %s\n",factory_id);
  if (strcmp(factory_id,CLAP_PLUGIN_FACTORY_ID) == 0) {
    return &my_factory;
  }
  return nullptr;
}

//------------------------------

CLAP_EXPORT const clap_plugin_entry_t clap_entry {
  .clap_version = CLAP_VERSION,
  .init         = clap_entry_init_callback,
  .deinit       = clap_entry_deinit_callback,
  .get_factory  = clap_entry_get_factory_callback
};

//----------------------------------------------------------------------
#endif
