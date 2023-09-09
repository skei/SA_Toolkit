#ifndef sa_interceptor_included
#define sa_interceptor_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "base/system/sat_library.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/clap/sat_clap_plugin.h"
#include "plugin/clap/sat_clap_host_implementation.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_interceptor_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/sa_interceptor",
  .name         = "sa_interceptor",
  .vendor       = SAT_VENDOR,
  .url          = SAT_URL,
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
  .description  = "man in the middle",
  .features     = (const char* []) {
                    CLAP_PLUGIN_FEATURE_AUDIO_EFFECT,
                    //CLAP_PLUGIN_FEATURE_INSTRUMENT,
                    nullptr
                  }
};

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

  const clap_host_t*    MInterceptedHost    = nullptr;

//------------------------------
public:
//------------------------------

  sa_interceptor_host(const clap_host_t* AHost)
  : SAT_ClapHostImplementation() {
    SAT_PRINT;
    MInterceptedHost = AHost;
  };

  //----------

  virtual ~sa_interceptor_host() {
    SAT_PRINT;
    //return MInterceptedPlugin->
  }
  
//------------------------------
public:
//------------------------------

  const void* get_extension(const char *extension_id) final {
    SAT_Print("extension_id: %s\n",extension_id);
    const void* ext = MInterceptedHost->get_extension(MInterceptedHost,extension_id);
    return ext;
  }

  void request_restart() final {
    SAT_PRINT;
    MInterceptedHost->request_restart(MInterceptedHost);
  }

  void request_process() final {
    SAT_PRINT;
    MInterceptedHost->request_process(MInterceptedHost);
  }

  void request_callback() final {
    SAT_PRINT;
    MInterceptedHost->request_callback(MInterceptedHost);
  }  
  
//------------------------------
public:
//------------------------------

  void ambisonic_changed() final {
    SAT_PRINT;
    //MInterceptedHost->
  }

  bool audio_ports_is_rescan_flag_supported(uint32_t flag) final {
    SAT_PRINT;
    //return MInterceptedHost->
    return false; 
  }

  void audio_ports_rescan(uint32_t flags) final {
    SAT_PRINT;
    //MInterceptedHost->
  }

  void audio_ports_config_rescan() final {
    SAT_PRINT;
    //MInterceptedHost->
  }

  void check_for_update_on_new_version(const clap_check_for_update_info_t *update_info) final {
    SAT_PRINT;
    //MInterceptedHost->
  }

  bool context_menu_populate(const clap_context_menu_target_t *target, const clap_context_menu_builder_t *builder) final {
    SAT_PRINT;
    //return MInterceptedHost->
    return false; 
  }

  bool context_menu_perform(const clap_context_menu_target_t *target, clap_id action_id) final {
    SAT_PRINT;
    //return MInterceptedHost->
    return false; 
  }

  bool context_menu_can_popup() final {
    SAT_PRINT;
    //return MInterceptedHost->
    return false; 
  }

  bool context_menu_popup(const clap_context_menu_target_t *target, int32_t screen_index, int32_t x, int32_t y) final {
    SAT_PRINT;
    //return MInterceptedHost->
    return false; 
  }

  void cv_changed() final {
    //MInterceptedHost->
    SAT_PRINT;
    //return MInterceptedPlugin->
  }

  bool event_registry_query(const char *space_name, uint16_t *space_id) final {
    SAT_PRINT;
    //return MInterceptedHost->
    return false; 
  }

  void gui_resize_hints_changed() final {
    //MInterceptedHost->
    SAT_PRINT;
  }

  bool gui_request_resize(uint32_t width, uint32_t height) final {
    SAT_PRINT;
    //return MInterceptedHost->
    return false; 
  }

  bool gui_request_show() final {
    SAT_PRINT;
    //return MInterceptedHost->
    return false; 
  }

  bool gui_request_hide() final {
    SAT_PRINT;
    //return MInterceptedHost->
    return false; 
  }

  void gui_closed(bool was_destroyed) final {
    SAT_PRINT;
    //MInterceptedHost->
  }

  void latency_changed() final {
    SAT_PRINT;
    //MInterceptedHost->
  }

  void log_log(clap_log_severity severity, const char *msg) final {
    SAT_PRINT;
    //MInterceptedHost->
  }

  void midi_mappings_changed() final {
    SAT_PRINT;
    //MInterceptedHost->
  }

  void note_name_changed() final {
    SAT_PRINT;
    //MInterceptedHost->
  }

  uint32_t note_ports_supported_dialects() final {
    SAT_PRINT;
    //return MInterceptedHost->
    return 0; 
  }

  void note_ports_rescan(uint32_t flags) final {
    SAT_PRINT;
    //MInterceptedHost->
  }

  void params_rescan(clap_param_rescan_flags flags) final {
    SAT_PRINT;
    //MInterceptedHost->
  }

  void params_clear(clap_id param_id, clap_param_clear_flags flags) final {
    SAT_PRINT;
    //MInterceptedHost->
  }

  void params_request_flush() final {
    SAT_PRINT;
    //MInterceptedHost->
  }

  bool posix_fd_support_register_fd(int fd, clap_posix_fd_flags_t flags) final {
    SAT_PRINT;
    //return MInterceptedHost->
    return false; 
  }

  bool posix_fd_support_modify_fd(int fd, clap_posix_fd_flags_t flags) final {
    SAT_PRINT;
    //return MInterceptedHost->
    return false; 
  }

  bool posix_fd_support_unregister_fd(int fd) final {
    SAT_PRINT;
    //return MInterceptedHost->
    return false; 
  }

  void preset_load_on_error(uint32_t location_kind, const char *location, const char *load_key,int32_t os_error, const char *msg) final {
    SAT_PRINT;
    //MInterceptedHost->
  }

  void preset_load_loaded(uint32_t location_kind, const char *location, const char *load_key) final {
    SAT_PRINT;
    //MInterceptedHost->
  }

  void remote_controls_changed() final {
    SAT_PRINT;
    //MInterceptedHost->
  }

  void remote_controls_suggest_page(clap_id page_id) final {
    SAT_PRINT;
    //MInterceptedHost->
  }

  bool resource_directory_request_directory(bool is_shared) final {
    SAT_PRINT;
    //return MInterceptedHost->
    return false; 
  }

  void resource_directory_release_directory(bool is_shared) final {
    SAT_PRINT;
    //MInterceptedHost->
  }

  void state_mark_dirty() final {
    SAT_PRINT;
    //MInterceptedHost->
  }

  void surround_changed() final {
    SAT_PRINT;
    //MInterceptedHost->
  }

  void surround_get_preferred_channel_map(uint8_t *channel_map, uint32_t channel_map_capacity, uint32_t *channel_count) final {
    SAT_PRINT;
    //MInterceptedHost->
  }

  void tail_changed() final {
    SAT_PRINT;
    //MInterceptedHost->
  }

  bool thread_check_is_main_thread() final {
    SAT_PRINT;
    //return MInterceptedHost->
    return false; 
  }

  bool thread_check_is_audio_thread() final {
    SAT_PRINT;
    //return MInterceptedHost->
    return false; 
  }

  bool thread_pool_request_exec(uint32_t num_tasks) final {
    SAT_PRINT;
    //return MInterceptedHost->
    return false; 
  }

  bool timer_support_register_timer(uint32_t period_ms, clap_id *timer_id) final {
    SAT_PRINT;
    //return MInterceptedHost->
    return false; 
  }

  bool timer_support_unregister_timer(clap_id timer_id) final {
    SAT_PRINT;
    //return MInterceptedHost->
    return false; 
  }

  bool track_info_get(clap_track_info_t *info) final {
    SAT_PRINT;
    //return MInterceptedHost->
    return false; 
  }

  void transport_control_request_start() final {
    SAT_PRINT;
    //MInterceptedHost->
  }

  void transport_control_request_stop() final {
    SAT_PRINT;
    //MInterceptedHost->
  }

  void transport_control_request_continue() final {
    SAT_PRINT;
    //MInterceptedHost->
  }

  void transport_control_request_pause() final {
    SAT_PRINT;
    //MInterceptedHost->
  }

  void transport_control_request_toggle_play() final {
    SAT_PRINT;
    //MInterceptedHost->
  }

  void transport_control_request_jump(clap_beattime position) final {
    SAT_PRINT;
    //MInterceptedHost->
  }

  void transport_control_request_loop_region(clap_beattime start, clap_beattime duration) final {
    SAT_PRINT;
    //MInterceptedHost->
  }

  void transport_control_request_toggle_loop() final {
    SAT_PRINT;
    //MInterceptedHost->
  }

  void transport_control_request_enable_loop(bool is_enabled) final {
    SAT_PRINT;
    //MInterceptedHost->
  }

  void transport_control_request_record(bool is_recording) final {
    SAT_PRINT;
    //MInterceptedHost->
  }

  void transport_control_request_toggle_record() final {
    SAT_PRINT;
    //MInterceptedHost->
  }

  void triggers_rescan(clap_trigger_rescan_flags flags) final {
    SAT_PRINT;
    //MInterceptedHost->
  }

  void triggers_clear(clap_id trigger_id, clap_trigger_clear_flags flags) final {
    SAT_PRINT;
    //MInterceptedHost->
  }

  double tuning_get_relative(clap_id tuning_id, int32_t channel, int32_t key, uint32_t sample_offset) final {
    SAT_PRINT;
    return 0.0; 
  }

  bool tuning_should_play(clap_id tuning_id, int32_t channel, int32_t key) final {
    SAT_PRINT;
    //return MInterceptedHost->
    return false; 
  }

  uint32_t tuning_get_tuning_count() final { return 0; 
    SAT_PRINT;
    //MInterceptedHost->
  }

  bool tuning_get_info(uint32_t tuning_index, clap_tuning_info_t *info) final {
    SAT_PRINT;
    //return MInterceptedHost->
    return false; 
  }

  void voice_info_changed() final {  
    SAT_PRINT;
    //MInterceptedHost->
  }

};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class sa_interceptor_plugin
: public SAT_ClapPlugin {

//------------------------------
private:
//------------------------------

  const clap_plugin_entry_t*      MInterceptedEntry                           = nullptr;
  const clap_plugin_factory_t*    MInterfeptedFactory                         = nullptr;
  const clap_plugin_t*            MInterceptedPlugin                          = nullptr;
  const clap_plugin_descriptor_t* MInterceptedDescriptor                      = nullptr;

  clap_process_t*                 MInterceptedProcess                         = nullptr;
  const clap_input_events_t*      MInterceptedInputEvents                     = nullptr;
  const clap_output_events_t*     MInterceptedOutputEvents                    = nullptr;

  char                            MInterceptedPluginPath[SAT_MAX_PATH_LENGTH] = {0};
  sa_interceptor_host*            MInterceptorHost                            = nullptr;
  SAT_Library                     MLibrary                                    = {};
  
  //----------

//------------------------------
public:
//------------------------------

  const clap_plugin_ambisonic_t*    ext_ambisonic   = nullptr; // draft
  const clap_plugin_audio_ports_t*  ext_audio_ports = nullptr;

//------------------------------
public:
//------------------------------

  sa_interceptor_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_ClapPlugin(ADescriptor,AHost) {
    SAT_PRINT;
    if (loadInterceptedPlugin()) {
      SAT_Print("loaded\n");
      createInterceptedPlugin();
    }
  }

  //----------

  virtual ~sa_interceptor_plugin() {
    SAT_PRINT;
    destroyInterceptedPlugin();
    unloadInterceptedPlugin();
  }
  
//------------------------------
public:
//------------------------------

  bool loadInterceptedPlugin() {
    SAT_PRINT;
    const char* plugin_path = SAT_GLOBAL.getPluginPath();
    strcpy(MInterceptedPluginPath,plugin_path);
    SAT_ReplaceString(MInterceptedPluginPath,"_interceptor","");
    SAT_Print("loading %s\n",MInterceptedPluginPath);
    
    if (strcmp(plugin_path,MInterceptedPluginPath) == 0) {
      SAT_Print("..don't try to load ourselves.. %s == %s\n",plugin_path,MInterceptedPluginPath);
      return false;
    }
    
    if (!MLibrary.loadLib(MInterceptedPluginPath)) {
      SAT_Print("error! couldn't load %s\n",MInterceptedPluginPath);
      return false;
    }
    
    return true;
  }
  
  //----------
    
  void unloadInterceptedPlugin() {
    SAT_PRINT;
    MLibrary.unloadLib();
  }
  
  //----------
    
  bool createInterceptedPlugin(uint32_t AIndex=0) {
    SAT_PRINT;
    const clap_host_t* host = getClapHost();
    MInterceptorHost = new sa_interceptor_host(host);
    
    //const clap_host_t* host = MInterceptorHost->getClapHost();
    
    if (!host) { SAT_Print("! ERROR: Couldn't create host\n"); return false; }
    else {
      
      //MInterceptedEntry = &clap_entry;
      MInterceptedEntry = (const clap_plugin_entry_t*)MLibrary.getLibSymbol("clap_entry");
      if (!MInterceptedEntry) { SAT_Print("! ERROR: clap_entry is null\n"); return false; }
      else {
        //uint32_t count = entry->init(plugin_path);
        MInterfeptedFactory = (const clap_plugin_factory_t*)MInterceptedEntry->get_factory(CLAP_PLUGIN_FACTORY_ID);
        if (!MInterfeptedFactory) { SAT_Print("! ERROR: Couldn't get factory from entry\n"); return false; }
        else {
          uint32_t count = MInterfeptedFactory->get_plugin_count(MInterfeptedFactory);
          if (count == 0) { SAT_Print("! ERROR: Plugin count is 0\n"); return false; }
          else {
            MInterceptedDescriptor = MInterfeptedFactory->get_plugin_descriptor(MInterfeptedFactory,AIndex);
            if (!MInterceptedDescriptor) { SAT_Print("! ERROR: Couldn't get descriptor from factory\n"); return false; }
            else {
              const char* plugin_id = MInterceptedDescriptor->id;
              if (!plugin_id) { SAT_Print("! ERROR: plugin_id is null\n"); return false; }
              else {
                MInterceptedPlugin = MInterfeptedFactory->create_plugin(MInterfeptedFactory,host,plugin_id);
                if (!MInterceptedPlugin) { SAT_Print("! ERROR: Couldn't create plugin from factory\n"); return false; }
                else {
                  //handle_plugin(MInterceptedPlugin);
                  return true;
                } // plugin
              } // plugin_id
            } // descriptor
          } // count
        } // factory
      } // entry
      //delete hostimpl;
    }
    return true;
  }
  
  //----------
  
  void initExtensions() {
    SAT_PRINT;
    ext_ambisonic   = (clap_plugin_ambisonic_t*)MInterceptedPlugin->get_extension(MInterceptedPlugin,CLAP_EXT_AMBISONIC);
    ext_audio_ports = (clap_plugin_audio_ports_t*)MInterceptedPlugin->get_extension(MInterceptedPlugin,CLAP_EXT_AUDIO_PORTS);
    //...
    //...
    //...
  }
  
  //----------
    
  void destroyInterceptedPlugin() {
    SAT_PRINT;
    MInterceptedPlugin->destroy(MInterceptedPlugin);
    delete MInterceptedPlugin;
    delete MInterceptorHost;
    MInterceptedEntry->deinit(); // entry->init() called?
  }
  
//------------------------------
public:
//------------------------------

  bool init() final {
    SAT_PRINT;
    SAT_Assert(MInterceptedPlugin);
    bool result = MInterceptedPlugin->init(MInterceptedPlugin);
    initExtensions();
    return result;
  }
    
  void destroy() final {
    SAT_PRINT;
    SAT_Assert(MInterceptedPlugin);
    MInterceptedPlugin->destroy(MInterceptedPlugin);
  }

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    SAT_PRINT;
    SAT_Assert(MInterceptedPlugin);
    return MInterceptedPlugin->activate(MInterceptedPlugin,sample_rate,min_frames_count,max_frames_count);
  }

  void deactivate() final {
    SAT_PRINT;
    SAT_Assert(MInterceptedPlugin);
    MInterceptedPlugin->deactivate(MInterceptedPlugin);
  }

  bool start_processing() final {
    SAT_PRINT;
    SAT_Assert(MInterceptedPlugin);
    return MInterceptedPlugin->start_processing(MInterceptedPlugin);
  }

  void stop_processing() final {
    SAT_PRINT;
    SAT_Assert(MInterceptedPlugin);
    MInterceptedPlugin->stop_processing(MInterceptedPlugin);
  }

  void reset() final {
    SAT_PRINT;
    SAT_Assert(MInterceptedPlugin);
    MInterceptedPlugin->reset(MInterceptedPlugin);
  }

  clap_process_status process(const clap_process_t* process) final {
    //SAT_PRINT;
    SAT_Assert(MInterceptedPlugin);
    SAT_Assert(process);
    //SAT_Print("process: %p\n",process);
    //SAT_Print("process->steady_time: %i\n",process->steady_time);
    //SAT_Print("process->frames_count: %i\n",process->frames_count);
    //SAT_Print("process->transport: %p\n",process->transport);
    //SAT_Print("process->audio_inputs: %p\n",process->audio_inputs);
    //SAT_Print("process->audio_outputs: %p\n",process->audio_outputs);
    //SAT_Print("process->audio_inputs_count: %i\n",process->audio_inputs_count);
    //SAT_Print("process->audio_outputs_count: %i\n",process->audio_outputs_count);
    //SAT_Print("process->in_events: %p\n",process->in_events);
    //SAT_Print("process->out_events: %p\n",process->out_events);
    clap_process_status result = MInterceptedPlugin->process(MInterceptedPlugin,process);
    return result;
  }

  const void* get_extension(const char *id) final {
    //SAT_Print("id: %s\n",id);
    SAT_Assert(MInterceptedPlugin);
    return MInterceptedPlugin->get_extension(MInterceptedPlugin,id);
  }

  void on_main_thread() final {
    SAT_PRINT;
    SAT_Assert(MInterceptedPlugin);
    MInterceptedPlugin->on_main_thread(MInterceptedPlugin);
  }
  
//------------------------------
public:
//------------------------------


  bool ambisonic_get_info(bool is_input,  uint32_t port_index, clap_ambisonic_info_t *info) final {
    SAT_PRINT;
    return ext_ambisonic->get_info(MInterceptedPlugin,is_input,port_index,info);
  }
  
  //----------

  uint32_t audio_ports_count(bool is_input) final {
    SAT_PRINT;
    return ext_audio_ports->count(MInterceptedPlugin,is_input);
  }

  bool audio_ports_get(uint32_t index, bool is_input, clap_audio_port_info_t *info) final {
    SAT_PRINT;
    return ext_audio_ports->get(MInterceptedPlugin,index,is_input,info);
  }

  //----------

  bool audio_ports_activation_can_activate_while_processing() final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  bool audio_ports_activation_set_active(bool is_input, uint32_t port_index, bool is_active) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  //----------

  uint32_t audio_ports_config_count() final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return 0; 
  }

  bool audio_ports_config_get(uint32_t index, clap_audio_ports_config_t *config) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  bool audio_ports_config_select(clap_id config_id) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  //----------

  void check_for_updates_check(bool include_preview) final { 
    SAT_PRINT;
    //return MInterceptedPlugin->
  }

  //----------

  bool configurable_audio_ports_is_port_configurable(bool is_input, uint32_t port_index) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  bool configurable_audio_ports_request_configuration(const struct clap_audio_port_configuration_request *requests,uint32_t request_count) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  //----------

  bool context_menu_populate(const clap_context_menu_target_t *target, const clap_context_menu_builder_t *builder) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  bool context_menu_perform(const clap_context_menu_target_t *target, clap_id action_id) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  //----------

  bool cv_get_channel_type(bool is_input, uint32_t port_index, uint32_t channel_index, uint32_t *channel_type) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  //----------

  bool extensible_audio_ports_add_port(bool is_input, uint32_t channel_count, const char *port_type, const void *port_details) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  bool extensible_audio_ports_remove_port(bool is_input, uint32_t index) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  //----------

  bool gui_is_api_supported(const char *api, bool is_floating) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  bool gui_get_preferred_api(const char **api, bool *is_floating) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  bool gui_create(const char *api, bool is_floating) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  void gui_destroy() final { 
    SAT_PRINT;
    //return MInterceptedPlugin->
  }

  bool gui_set_scale(double scale) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  bool gui_get_size(uint32_t *width, uint32_t *height) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  bool gui_can_resize() final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  bool gui_get_resize_hints(clap_gui_resize_hints_t *hints) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  bool gui_adjust_size(uint32_t *width, uint32_t *height) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  bool gui_set_size(uint32_t width, uint32_t height) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  bool gui_set_parent(const clap_window_t *window) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  bool gui_set_transient(const clap_window_t *window) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  void gui_suggest_title(const char *title) final { 
    SAT_PRINT;
    //return MInterceptedPlugin->
  }

  bool gui_show() final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  bool gui_hide() final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  //----------

  uint32_t latency_get() final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return 0; 
  }

  //----------

  uint32_t midi_mappings_count() final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return 0; 
  }

  bool midi_mappings_get(uint32_t index, clap_midi_mapping_t *mapping) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  //----------

  uint32_t note_name_count() final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return 0; 
  }

  bool note_name_get(uint32_t index, clap_note_name_t *note_name) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  //----------

  uint32_t note_ports_count(bool is_input) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return 0; 
  }

  bool note_ports_get(uint32_t index, bool is_input, clap_note_port_info_t* info) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  //----------

  void param_indication_set_mapping(clap_id param_id, bool has_mapping, const clap_color_t *color, const char *label, const char *description) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
  }

  void param_indication_set_automation(clap_id param_id, uint32_t automation_state, const clap_color_t *color) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
  }

  //----------

  uint32_t params_count() final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return 0; 
  }

  bool params_get_info(uint32_t param_index, clap_param_info_t *param_info) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  bool params_get_value(clap_id param_id, double *value) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  bool params_value_to_text(clap_id param_id, double value, char *display, uint32_t size) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  bool params_text_to_value(clap_id param_id, const char *display, double *value) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  void params_flush(const clap_input_events_t *in, const clap_output_events_t *out) final { 
    SAT_PRINT;
    //return MInterceptedPlugin->
  }

  //----------

  void posix_fd_support_on_fd(int fd, clap_posix_fd_flags_t flags) final { 
    SAT_PRINT;
    //return MInterceptedPlugin->
  }

  //----------

  bool preset_load_from_location(uint32_t location_kind, const char *location, const char *load_key) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  //----------

  uint32_t remote_controls_count() final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return 0; 
  }

  bool remote_controls_get(uint32_t page_index, clap_remote_controls_page_t *page) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  //----------

  bool render_has_hard_realtime_requirement() final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  bool render_set(clap_plugin_render_mode mode) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  //----------

  void resource_directory_set_directory(const char *path, bool is_shared) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
  }

  void resource_directory_collect(bool all) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
  }

  uint32_t resource_directory_get_files_count() final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return 0; 
  }

  int32_t resource_directory_get_file_path(uint32_t index, char *path, uint32_t path_size) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return -1; 
  }

  //----------

  bool state_save(const clap_ostream_t *stream) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  bool state_load(const clap_istream_t *stream) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  //----------

  bool state_context_save(const clap_ostream_t *stream, uint32_t context_type) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  bool state_context_load(const clap_istream_t *stream, uint32_t context_type) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  //----------

  uint32_t surround_get_channel_map(bool is_input, uint32_t port_index, uint8_t *channel_map, uint32_t channel_map_capacity) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return 0; 
  }

  void surround_changed() final { 
    SAT_PRINT;
    //return MInterceptedPlugin->
  }

  //----------

  uint32_t tail_get() final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return 0; 
  }

  //----------

  void thread_pool_exec(uint32_t task_index) final { 
    SAT_PRINT;
    //return MInterceptedPlugin->
  }

  //----------

  void timer_support_on_timer(clap_id timer_id) final { 
    SAT_PRINT;
    //return MInterceptedPlugin->
  }

  //----------

  void track_info_changed() final { 
    SAT_PRINT;
    //return MInterceptedPlugin->
  }

  //----------

  uint32_t triggers_count() final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return 0; 
  }

  bool triggers_get_info(uint32_t index, clap_trigger_info_t *trigger_info) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

  //----------

  void tuning_changed() final { 
    SAT_PRINT;
    //return MInterceptedPlugin->
  }

  //----------

  bool voice_info_get(clap_voice_info_t *info) final {
    SAT_PRINT;
    //return MInterceptedPlugin->
    return false; 
  }

};

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY

  #include "plugin/sat_entry.h"
  
  void SAT_Register(SAT_Registry* ARegistry) {
    ARegistry->registerDescriptor(&sa_interceptor_descriptor);
  }

  const clap_plugin_t* SAT_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
    if (AIndex == 0) {
      SAT_ClapPlugin* plugin = new sa_interceptor_plugin(ADescriptor,AHost);
      return plugin->getClapPlugin();
    }
    return nullptr;
  }
  
  
#endif


//----------------------------------------------------------------------
#endif
