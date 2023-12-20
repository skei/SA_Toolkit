#ifndef interceptor_included
#define interceptor_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/clap/sat_clap_plugin.h"
#include "plugin/clap/sat_clap_host_implementation.h"

//----------------------------------------------------------------------
//
// host
//
//----------------------------------------------------------------------

class SA_InterceptorHost
: public SAT_ClapHostImplementation {

//------------------------------
private:
//------------------------------

//------------------------------
public:
//------------------------------

  SA_InterceptorHost()
  : SAT_ClapHostImplementation() {
  }

  //----------

  virtual ~SA_InterceptorHost() {
  }

//------------------------------
public: // extensions
//------------------------------

  const void* get_extension(const char *extension_id) final { return nullptr; }
  void        request_restart() final {}
  void        request_process() final {}
  void        request_callback() final {}

//------------------------------
public: // extensions
//------------------------------

  bool        audio_ports_is_rescan_flag_supported(uint32_t flag) final { return false; }
  void        audio_ports_rescan(uint32_t flags) final {}
  void        audio_ports_config_rescan() final {}
  bool        event_registry_query(const char *space_name, uint16_t *space_id) final { return false; }
  void        gui_resize_hints_changed() final {}
  bool        gui_request_resize(uint32_t width, uint32_t height) final { return false; }
  bool        gui_request_show() final { return false; }
  bool        gui_request_hide() final { return false; }
  void        gui_closed(bool was_destroyed) final {}
  void        latency_changed() final {}
  void        log_log(clap_log_severity severity, const char *msg) final {}
  void        note_name_changed() final {}
  uint32_t    note_ports_supported_dialects() final { return 0; }
  void        note_ports_rescan(uint32_t flags) final {}
  void        params_rescan(clap_param_rescan_flags flags) final {}
  void        params_clear(clap_id param_id, clap_param_clear_flags flags) final {}
  void        params_request_flush() final {}
  bool        posix_fd_support_register_fd(int fd, clap_posix_fd_flags_t flags) final { return false; }
  bool        posix_fd_support_modify_fd(int fd, clap_posix_fd_flags_t flags) final { return false; }
  bool        posix_fd_support_unregister_fd(int fd) final { return false; }
  void        state_mark_dirty() final {}
  void        tail_changed() final {}
  bool        thread_check_is_main_thread() final { return false; }
  bool        thread_check_is_audio_thread() final { return false; }
  bool        thread_pool_request_exec(uint32_t num_tasks) final { return false; }
  bool        timer_support_register_timer(uint32_t period_ms, clap_id *timer_id) final { return false; }
  bool        timer_support_unregister_timer(clap_id timer_id) final { return false; }
  void        voice_info_changed() final {}

//------------------------------
public: // drafts
//------------------------------

  void        ambisonic_changed() final {}
  void        check_for_update_on_new_version(const clap_check_for_update_info_t *update_info) final {}
  bool        context_menu_populate(const clap_context_menu_target_t *target, const clap_context_menu_builder_t *builder) final { return false; }
  bool        context_menu_perform(const clap_context_menu_target_t *target, clap_id action_id) final { return false; }
  bool        context_menu_can_popup() final { return false; }
  bool        context_menu_popup(const clap_context_menu_target_t *target, int32_t screen_index, int32_t x, int32_t y) final { return false; }
  void        cv_changed() final {}
  void        midi_mappings_changed() final {}
  void        preset_load_on_error(uint32_t location_kind, const char *location, const char *load_key,int32_t os_error, const char *msg) final {}
  void        preset_load_loaded(uint32_t location_kind, const char *location, const char *load_key) final {}
  void        remote_controls_changed() final {}
  void        remote_controls_suggest_page(clap_id page_id) final {}
  bool        resource_directory_request_directory(bool is_shared) final { return false; }
  void        resource_directory_release_directory(bool is_shared) final {}
  void        surround_changed() final {}
  bool        track_info_get(clap_track_info_t *info) final { return false; }
  void        transport_control_request_start() final {}
  void        transport_control_request_stop() final {}
  void        transport_control_request_continue() final {}
  void        transport_control_request_pause() final {}
  void        transport_control_request_toggle_play() final {}
  void        transport_control_request_jump(clap_beattime position) final {}
  void        transport_control_request_loop_region(clap_beattime start, clap_beattime duration) final {}
  void        transport_control_request_toggle_loop() final {}
  void        transport_control_request_enable_loop(bool is_enabled) final {}
  void        transport_control_request_record(bool is_recording) final {}
  void        transport_control_request_toggle_record() final {}
  void        triggers_rescan(clap_trigger_rescan_flags flags) final {}
  void        triggers_clear(clap_id trigger_id, clap_trigger_clear_flags flags) final {}
  double      tuning_get_relative(clap_id tuning_id, int32_t channel, int32_t key, uint32_t sample_offset) final { return 0.0; }
  bool        tuning_should_play(clap_id tuning_id, int32_t channel, int32_t key) final { return false; }
  uint32_t    tuning_get_tuning_count() final { return 0; }
  bool        tuning_get_info(uint32_t tuning_index, clap_tuning_info_t *info) final { return false; }
};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class SA_InterceptorPlugin
: public SAT_ClapPlugin {

//------------------------------
private:
//------------------------------

  SA_InterceptorHost  MHost = {};

//------------------------------
public:
//------------------------------

  SA_InterceptorPlugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_ClapPlugin(ADescriptor,AHost) {
  }

  //----------

  // note: we're calling 'delete this' in SAT_Plugin,destroy()

  virtual ~SA_InterceptorPlugin() {
  }

//------------------------------
protected:
//------------------------------

  virtual bool                init() final { return true; }
  virtual void                destroy() final {}
  virtual bool                activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final { return true; }
  virtual void                deactivate() final {}
  virtual bool                start_processing() final { return true; }
  virtual void                stop_processing() final {}
  virtual void                reset() final {}
  virtual clap_process_status process(const clap_process_t *process) final { return CLAP_PROCESS_CONTINUE; }
  virtual const void*         get_extension(const char *id) final { return nullptr; }
  virtual void                on_main_thread() final {}

  // extensions

  virtual uint32_t            audio_ports_count(bool is_input) final { return 0; }
  virtual bool                audio_ports_get(uint32_t index, bool is_input, clap_audio_port_info_t *info) final { return false; }
  virtual uint32_t            audio_ports_config_count() final { return 0; }
  virtual bool                audio_ports_config_get(uint32_t index, clap_audio_ports_config_t *config) final { return true; }
  virtual bool                audio_ports_config_select(clap_id config_id) final { return true;}
  virtual bool                gui_is_api_supported(const char *api, bool is_floating) final { return false; }
  virtual bool                gui_get_preferred_api(const char **api, bool *is_floating) final { return false; }
  virtual bool                gui_create(const char *api, bool is_floating) final { return false; }
  virtual void                gui_destroy() final {}
  virtual bool                gui_set_scale(double scale) final { return false; }
  virtual bool                gui_get_size(uint32_t *width, uint32_t *height) final { return false; }
  virtual bool                gui_can_resize() final { return false; }
  virtual bool                gui_get_resize_hints(clap_gui_resize_hints_t *hints) final { return false; }
  virtual bool                gui_adjust_size(uint32_t *width, uint32_t *height) final { return false; }
  virtual bool                gui_set_size(uint32_t width, uint32_t height) final { return false; }
  virtual bool                gui_set_parent(const clap_window_t *window) final { return false; }
  virtual bool                gui_set_transient(const clap_window_t *window) final { return false; }
  virtual void                gui_suggest_title(const char *title) final {}
  virtual bool                gui_show() final { return false; }
  virtual bool                gui_hide() final { return false; }
  virtual uint32_t            latency_get() final { return 0; }
  virtual uint32_t            note_name_count() final { return 0; }
  virtual bool                note_name_get(uint32_t index, clap_note_name_t *note_name) final { return false; }
  virtual uint32_t            note_ports_count(bool is_input) final { return 0; }
  virtual bool                note_ports_get(uint32_t index, bool is_input, clap_note_port_info_t *info) final { return false; }
  virtual uint32_t            params_count() final { return 0; }
  virtual bool                params_get_info(uint32_t param_index, clap_param_info_t *param_info) final { return false; }
  virtual bool                params_get_value(clap_id param_id, double *out_value) final { return false; }
  virtual bool                params_value_to_text(clap_id param_id, double value, char *out_buffer, uint32_t out_buffer_capacity) final { return false; }
  virtual bool                params_text_to_value(clap_id param_id, const char *param_value_text, double *out_value) final { return false; }
  virtual void                params_flush(const clap_input_events_t *in, const clap_output_events_t *out) final {}
  virtual void                posix_fd_support_on_fd(int fd, clap_posix_fd_flags_t flags) final {}
  virtual bool                render_has_hard_realtime_requirement() final { return false; }
  virtual bool                render_set(clap_plugin_render_mode mode) final { return false; }
  virtual bool                state_save(const clap_ostream_t *stream) final { return false; }
  virtual bool                state_load(const clap_istream_t *stream) final { return false; }
  virtual uint32_t            tail_get() final { return 0; }
  virtual void                thread_pool_exec(uint32_t task_index) final {}
  virtual void                timer_support_on_timer(clap_id timer_id) final {}
  virtual bool                voice_info_get(clap_voice_info_t *info) final { return false; }

  // drafts

  virtual bool                ambisonic_is_config_supported(const clap_ambisonic_config_t *config) final { return false; }
  virtual bool                ambisonic_get_config(bool is_input, uint32_t port_index, clap_ambisonic_config_t *config) final { return false; }
  virtual bool                audio_ports_activation_can_activate_while_processing() final { return false; }
  virtual bool                audio_ports_activation_set_active(bool is_input, uint32_t port_index, bool is_active, uint32_t sample_size) final { return false; }
  virtual void                check_for_update_check(bool include_preview) final {}
  virtual bool                configurable_audio_ports_can_apply_configuration(const struct clap_audio_port_configuration_request *requests, uint32_t request_count) final { return false; }
  virtual bool                configurable_audio_ports_apply_configuration(const struct clap_audio_port_configuration_request *requests, uint32_t request_count) final { return false; }
  virtual bool                context_menu_populate(const clap_context_menu_target_t  *target, const clap_context_menu_builder_t *builder) final { return false; }
  virtual bool                context_menu_perform(const clap_context_menu_target_t *target, clap_id action_id) final { return false; }
  virtual bool                cv_get_channel_type(bool is_input, uint32_t port_index, uint32_t channel_index, uint32_t *channel_type) final { return false; }
  virtual bool                extensible_audio_ports_add_port(bool is_input, uint32_t channel_count, const char *port_type, const void *port_details) final { return false; }
  virtual bool                extensible_audio_ports_remove_port(bool is_input, uint32_t index) final { return false; }
  virtual uint32_t            midi_mappings_count() final { return 0; }
  virtual bool                midi_mappings_get(uint32_t index, clap_midi_mapping_t *mapping) final { return false; }
  virtual void                param_indication_set_mapping(clap_id param_id, bool has_mapping, const clap_color_t *color, const char *label, const char *description) final {}
  virtual void                param_indication_set_automation(clap_id param_id, uint32_t automation_state, const clap_color_t *color) final {}
  virtual bool                preset_load_from_location(uint32_t location_kind, const char *location, const char *load_key) final { return false; }
  virtual uint32_t            remote_controls_count() final { return 0; }
  virtual bool                remote_controls_get(uint32_t page_index, clap_remote_controls_page_t *page) final { return false; }
  virtual void                resource_directory_set_directory(const char *path, bool is_shared) final {}
  virtual void                resource_directory_collect(bool all) final {}
  virtual uint32_t            resource_directory_get_files_count() final { return 0; }
  virtual int32_t             resource_directory_get_file_path(uint32_t index, char *path, uint32_t path_size) final { return 0; }
  virtual bool                state_context_save(const clap_ostream_t *stream, uint32_t context_type) final { return false; }
  virtual bool                state_context_load(const clap_istream_t *stream, uint32_t context_type) final { return false; }
  virtual bool                surround_is_channel_mask_supported(uint64_t channel_mask) final { return false; }
  virtual uint32_t            surround_get_channel_map(bool is_input, uint32_t port_index, uint8_t *channel_map, uint32_t channel_map_capacity) final { return 0; }
  virtual void                track_info_changed() final {}
  virtual uint32_t            triggers_count() final { return 0; }
  virtual bool                triggers_get_info(uint32_t index, clap_trigger_info_t *trigger_info) final { return false; }
  virtual void                tuning_changed() final {}

};

//----------------------------------------------------------------------
//
// factory
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------


//----------------------------------------------------------------------
#endif
