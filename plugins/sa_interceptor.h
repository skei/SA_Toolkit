#ifndef sa_interceptor_included
#define sa_interceptor_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "base/system/sat_library.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/clap/sat_clap_plugin.h"
//#include "plugin/clap/sat_clap_host_implementation.h"

#include "sa_interceptor_host.h"

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
                    //CLAP_PLUGIN_FEATURE_AUDIO_EFFECT,
                    CLAP_PLUGIN_FEATURE_INSTRUMENT,
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

//------------------------------
private:
//------------------------------

  const clap_plugin_entry_t*      MInterceptedEntry                           = nullptr;
  const clap_plugin_factory_t*    MInterceptedFactory                         = nullptr;
  const clap_plugin_t*            MInterceptedPlugin                          = nullptr;
  const clap_plugin_descriptor_t* MInterceptedDescriptor                      = nullptr;

  clap_process_t*                 MInterceptedProcess                         = nullptr;
  const clap_input_events_t*      MInterceptedInputEvents                     = nullptr;
  const clap_output_events_t*     MInterceptedOutputEvents                    = nullptr;

  char                            MInterceptedPluginPath[SAT_MAX_PATH_LENGTH] = {0};
  sa_interceptor_host*            MInterceptorHost                            = nullptr;
  SAT_Library                     MLibrary                                    = {};
  
  SAT_ClapPluginExtensions        ext                                         = {};

//------------------------------
public:
//------------------------------

  sa_interceptor_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_ClapPlugin(ADescriptor,AHost) {
    SAT_Print("\n");
    uint32_t index = 0; // !!!
    if (loadInterceptedPlugin()) {
      if (createInterceptedPlugin(index)) {
        // yes
      }
    }
  }

  //----------

  virtual ~sa_interceptor_plugin() {
    SAT_Print("\n");
    destroyInterceptedPlugin();
    unloadInterceptedPlugin();
  }
  
//------------------------------
public:
//------------------------------

  bool loadInterceptedPlugin() {
    SAT_Print("\n");
    const char* plugin_path = SAT_GLOBAL.getPluginPath();
    strcpy(MInterceptedPluginPath,plugin_path);
    SAT_ReplaceString(MInterceptedPluginPath,"_interceptor","");
    SAT_Print("loading '%s'\n",MInterceptedPluginPath);
    if (strcmp(plugin_path,MInterceptedPluginPath) == 0) {
      SAT_Print("don't load ourselves.. '%s' == '%s'\n",plugin_path,MInterceptedPluginPath);
      return false;
    }
    if (!MLibrary.loadLib(MInterceptedPluginPath)) {
      SAT_Print("error! couldn't load '%s'\n",MInterceptedPluginPath);
      return false;
    }
    SAT_Print("'%s' loaded\n",MInterceptedPluginPath);
    return true;
  }
  
  //----------
    
  void unloadInterceptedPlugin() {
    SAT_Print("\n");
    MLibrary.unloadLib();
    SAT_Print("unloaded\n");
  }
  
  //----------
    
  bool createInterceptedPlugin(uint32_t AIndex) {
    SAT_Print("\n");
    
//    const clap_host_t* host = getClapHost();                    // host of interceptor plugin
//    MInterceptorHost = new sa_interceptor_host(host);           // host we give to intercepted plugin

    MInterceptorHost = new sa_interceptor_host(getClapHost());
    const clap_host_t* host = MInterceptorHost->getClapHost();
    
    //if (!host) { SAT_Print("! ERROR: Couldn't create host\n"); return false; }
    //else {
      //MInterceptedEntry = &clap_entry;
      MInterceptedEntry = (const clap_plugin_entry_t*)MLibrary.getLibSymbol("clap_entry");
      if (!MInterceptedEntry) { SAT_Print("! ERROR: clap_entry is null\n"); return false; }
      else {
        //uint32_t count = entry->init(plugin_path);
        MInterceptedFactory = (const clap_plugin_factory_t*)MInterceptedEntry->get_factory(CLAP_PLUGIN_FACTORY_ID);
        if (!MInterceptedFactory) { SAT_Print("! ERROR: Couldn't get factory from entry\n"); return false; }
        else {
          uint32_t count = MInterceptedFactory->get_plugin_count(MInterceptedFactory);
          if (count == 0) { SAT_Print("! ERROR: Plugin count is 0\n"); return false; }
          else {
            MInterceptedDescriptor = MInterceptedFactory->get_plugin_descriptor(MInterceptedFactory,AIndex);
            if (!MInterceptedDescriptor) { SAT_Print("! ERROR: Couldn't get descriptor from factory\n"); return false; }
            else {
              const char* plugin_id = MInterceptedDescriptor->id;
              if (!plugin_id) { SAT_Print("! ERROR: plugin_id is null\n"); return false; }
              else {
                
                MInterceptedPlugin = MInterceptedFactory->create_plugin(MInterceptedFactory,host,plugin_id);
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
    //}
    return true;
  }
  
  //----------
    
  void destroyInterceptedPlugin() {
    SAT_Print("\n");
    MInterceptedPlugin->destroy(MInterceptedPlugin);
    delete MInterceptedPlugin;
    delete MInterceptorHost;
    MInterceptedEntry->deinit(); // entry->init() called?
  }
  
  //----------
  
  void initExtensions() {
    SAT_Print("\n");
    ext.ambisonic               = (clap_plugin_ambisonic_t*)MInterceptedPlugin->get_extension(MInterceptedPlugin,CLAP_EXT_AMBISONIC);
    ext.audio_ports             = (clap_plugin_audio_ports_t*)MInterceptedPlugin->get_extension(MInterceptedPlugin,CLAP_EXT_AUDIO_PORTS);
    ext.audio_ports_activation  = (clap_plugin_audio_ports_activation_t*)MInterceptedPlugin->get_extension(MInterceptedPlugin,CLAP_EXT_AUDIO_PORTS_ACTIVATION);
    ext.audio_ports_config      = (clap_plugin_audio_ports_config_t*)MInterceptedPlugin->get_extension(MInterceptedPlugin,CLAP_EXT_AUDIO_PORTS_CONFIG);
    ext.check_for_update        = (clap_plugin_check_for_update_t*)MInterceptedPlugin->get_extension(MInterceptedPlugin,CLAP_EXT_CHECK_FOR_UPDATE);
    ext.context_menu            = (clap_plugin_context_menu_t*)MInterceptedPlugin->get_extension(MInterceptedPlugin,CLAP_EXT_CONTEXT_MENU);
    ext.cv                      = (clap_plugin_cv_t*)MInterceptedPlugin->get_extension(MInterceptedPlugin,CLAP_EXT_CV);
    ext.gui                     = (clap_plugin_gui_t*)MInterceptedPlugin->get_extension(MInterceptedPlugin,CLAP_EXT_GUI);
    ext.latency                 = (clap_plugin_latency_t*)MInterceptedPlugin->get_extension(MInterceptedPlugin,CLAP_EXT_LATENCY);
    ext.midi_mappings           = (clap_plugin_midi_mappings_t*)MInterceptedPlugin->get_extension(MInterceptedPlugin,CLAP_EXT_MIDI_MAPPINGS);
    ext.note_name               = (clap_plugin_note_name_t*)MInterceptedPlugin->get_extension(MInterceptedPlugin,CLAP_EXT_NOTE_NAME);
    ext.note_ports              = (clap_plugin_note_ports_t*)MInterceptedPlugin->get_extension(MInterceptedPlugin,CLAP_EXT_NOTE_PORTS);
    ext.param_indication        = (clap_plugin_param_indication_t*)MInterceptedPlugin->get_extension(MInterceptedPlugin,CLAP_EXT_PARAM_INDICATION);
    ext.params                  = (clap_plugin_params_t*)MInterceptedPlugin->get_extension(MInterceptedPlugin,CLAP_EXT_PARAMS);
    ext.posix_fd_support        = (clap_plugin_posix_fd_support_t*)MInterceptedPlugin->get_extension(MInterceptedPlugin,CLAP_EXT_POSIX_FD_SUPPORT);
    ext.preset_load             = (clap_plugin_preset_load_t*)MInterceptedPlugin->get_extension(MInterceptedPlugin,CLAP_EXT_PRESET_LOAD);
    ext.remote_controls         = (clap_plugin_remote_controls_t*)MInterceptedPlugin->get_extension(MInterceptedPlugin,CLAP_EXT_REMOTE_CONTROLS);
    ext.render                  = (clap_plugin_render_t*)MInterceptedPlugin->get_extension(MInterceptedPlugin,CLAP_EXT_RENDER);
    ext.resource_directory      = (clap_plugin_resource_directory_t*)MInterceptedPlugin->get_extension(MInterceptedPlugin,CLAP_EXT_RESOURCE_DIRECTORY);
    ext.state                   = (clap_plugin_state_t*)MInterceptedPlugin->get_extension(MInterceptedPlugin,CLAP_EXT_STATE);
    ext.state_context           = (clap_plugin_state_context_t*)MInterceptedPlugin->get_extension(MInterceptedPlugin,CLAP_EXT_STATE_CONTEXT);
    ext.surround                = (clap_plugin_surround_t*)MInterceptedPlugin->get_extension(MInterceptedPlugin,CLAP_EXT_SURROUND);
    ext.tail                    = (clap_plugin_tail_t*)MInterceptedPlugin->get_extension(MInterceptedPlugin,CLAP_EXT_TAIL);
    ext.thread_pool             = (clap_plugin_thread_pool_t*)MInterceptedPlugin->get_extension(MInterceptedPlugin,CLAP_EXT_THREAD_POOL);
    ext.timer_support           = (clap_plugin_timer_support_t*)MInterceptedPlugin->get_extension(MInterceptedPlugin,CLAP_EXT_TIMER_SUPPORT);
    ext.track_info              = (clap_plugin_track_info_t*)MInterceptedPlugin->get_extension(MInterceptedPlugin,CLAP_EXT_TRACK_INFO);
    ext.triggers                = (clap_plugin_triggers_t*)MInterceptedPlugin->get_extension(MInterceptedPlugin,CLAP_EXT_TRIGGERS);
    ext.tuning                  = (clap_plugin_tuning_t*)MInterceptedPlugin->get_extension(MInterceptedPlugin,CLAP_EXT_TUNING);
    ext.voice_info              = (clap_plugin_voice_info_t*)MInterceptedPlugin->get_extension(MInterceptedPlugin,CLAP_EXT_VOICE_INFO);
  }
  
//------------------------------
public:
//------------------------------

  bool init() final {
    SAT_Print("\n");
    SAT_Assert(MInterceptedPlugin);
    bool result = MInterceptedPlugin->init(MInterceptedPlugin);
    initExtensions();
    return result;
  }
    
  void destroy() final {
    SAT_Print("\n");
    SAT_Assert(MInterceptedPlugin);
    MInterceptedPlugin->destroy(MInterceptedPlugin);
  }

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    SAT_Print(": sample_rate %.2f min_frames_count %i max_frames_count %i\n",sample_rate,min_frames_count,max_frames_count);
    SAT_Assert(MInterceptedPlugin);
    return MInterceptedPlugin->activate(MInterceptedPlugin,sample_rate,min_frames_count,max_frames_count);
  }

  void deactivate() final {
    SAT_Print("\n");
    SAT_Assert(MInterceptedPlugin);
    MInterceptedPlugin->deactivate(MInterceptedPlugin);
  }

  bool start_processing() final {
    SAT_Print("\n");
    SAT_Assert(MInterceptedPlugin);
    return MInterceptedPlugin->start_processing(MInterceptedPlugin);
  }

  void stop_processing() final {
    SAT_Print("\n");
    SAT_Assert(MInterceptedPlugin);
    MInterceptedPlugin->stop_processing(MInterceptedPlugin);
  }

  void reset() final {
    SAT_Print("\n");
    SAT_Assert(MInterceptedPlugin);
    MInterceptedPlugin->reset(MInterceptedPlugin);
  }

  clap_process_status process(const clap_process_t* process) final {
    //SAT_Print("\n");
    SAT_Assert(MInterceptedPlugin);
    SAT_Assert(process);
    clap_process_status result = MInterceptedPlugin->process(MInterceptedPlugin,process);
    return result;
  }

  const void* get_extension(const char *id) final {
    //SAT_Print("id: %s\n",id);
    SAT_Print(" id: %s -> ",id);
    SAT_Assert(MInterceptedPlugin);
    const void* ext = MInterceptedPlugin->get_extension(MInterceptedPlugin,id);
    SAT_DPrint("%s\n",ext?"true":"false");
    return  ext;
  }

  void on_main_thread() final {
    SAT_Print("\n");
    SAT_Assert(MInterceptedPlugin);
    MInterceptedPlugin->on_main_thread(MInterceptedPlugin);
  }
  
//------------------------------
public:
//------------------------------

  bool ambisonic_get_info(bool is_input,  uint32_t port_index, clap_ambisonic_info_t *info) final {
    SAT_PRINT;
    return ext.ambisonic->get_info(MInterceptedPlugin,is_input,port_index,info);
  }
  
  //----------

  uint32_t audio_ports_count(bool is_input) final {
    SAT_PRINT;
    return ext.audio_ports->count(MInterceptedPlugin,is_input);
  }

  bool audio_ports_get(uint32_t index, bool is_input, clap_audio_port_info_t *info) final {
    SAT_PRINT;
    return ext.audio_ports->get(MInterceptedPlugin,index,is_input,info);
  }

  //----------

  bool audio_ports_activation_can_activate_while_processing() final {
    SAT_PRINT;
    return ext.audio_ports_activation->can_activate_while_processing(MInterceptedPlugin);
  }

  bool audio_ports_activation_set_active(bool is_input, uint32_t port_index, bool is_active) final {
    SAT_PRINT;
    return ext.audio_ports_activation->set_active(MInterceptedPlugin,is_input,port_index,is_active);
  }

  //----------

  uint32_t audio_ports_config_count() final {
    SAT_PRINT;
    return ext.audio_ports_config->count(MInterceptedPlugin);
  }

  bool audio_ports_config_get(uint32_t index, clap_audio_ports_config_t *config) final {
    SAT_PRINT;
    return ext.audio_ports_config->get(MInterceptedPlugin,index,config);
  }

  bool audio_ports_config_select(clap_id config_id) final {
    SAT_PRINT;
    return ext.audio_ports_config->select(MInterceptedPlugin,config_id);
  }

  //----------

  void check_for_updates_check(bool include_preview) final { 
    SAT_PRINT;
    ext.check_for_update->check(MInterceptedPlugin,include_preview);
  }

  //----------

  bool configurable_audio_ports_is_port_configurable(bool is_input, uint32_t port_index) final {
    SAT_PRINT;
    return ext.configurable_audio_ports->is_port_configurable(MInterceptedPlugin,is_input,port_index);
  }

  bool configurable_audio_ports_request_configuration(const struct clap_audio_port_configuration_request *requests,uint32_t request_count) final {
    SAT_PRINT;
    return ext.configurable_audio_ports->request_configuration(MInterceptedPlugin,requests,request_count);
  }

  //----------

  bool context_menu_populate(const clap_context_menu_target_t *target, const clap_context_menu_builder_t *builder) final {
    SAT_PRINT;
    return ext.context_menu->populate(MInterceptedPlugin,target,builder);
  }

  bool context_menu_perform(const clap_context_menu_target_t *target, clap_id action_id) final {
    SAT_PRINT;
    return ext.context_menu->perform(MInterceptedPlugin,target,action_id);
  }

  //----------

  bool cv_get_channel_type(bool is_input, uint32_t port_index, uint32_t channel_index, uint32_t *channel_type) final {
    SAT_PRINT;
    return ext.cv->get_channel_type(MInterceptedPlugin,is_input,port_index,channel_index,channel_type);
  }

  //----------

  bool extensible_audio_ports_add_port(bool is_input, uint32_t channel_count, const char *port_type, const void *port_details) final {
    SAT_PRINT;
    return ext.extensible_audio_ports->add_port(MInterceptedPlugin,is_input,channel_count,port_type,port_details);
  }

  bool extensible_audio_ports_remove_port(bool is_input, uint32_t index) final {
    SAT_PRINT;
    return ext.extensible_audio_ports->remove_port(MInterceptedPlugin,is_input,index);
  }

  //----------

  bool gui_is_api_supported(const char *api, bool is_floating) final {
    SAT_Print(": api %s is_floating %i\n",api,is_floating);
    return ext.gui->is_api_supported(MInterceptedPlugin,api,is_floating);
  }

  bool gui_get_preferred_api(const char **api, bool *is_floating) final {
    SAT_Print(": api %s is_floating %i\n",api,is_floating);
    return ext.gui->get_preferred_api(MInterceptedPlugin,api,is_floating);
  }

  bool gui_create(const char *api, bool is_floating) final {
    SAT_Print(": api %s is_floating %i\n",api,is_floating);
    return ext.gui->create(MInterceptedPlugin,api,is_floating);
  }

  void gui_destroy() final { 
    SAT_Print("\n");
    return ext.gui->destroy(MInterceptedPlugin);
  }

  bool gui_set_scale(double scale) final {
    SAT_Print(": scale %f\n",scale);
    return ext.gui->set_scale(MInterceptedPlugin,scale);
  }

  bool gui_get_size(uint32_t *width, uint32_t *height) final {
    SAT_Print(" ->\n");
    bool result = ext.gui->get_size(MInterceptedPlugin,width,height);
    SAT_DPrint("(%i,%i)\n",*width,*height);
    return result;
  }

  bool gui_can_resize() final {
    SAT_Print("\n");
    return ext.gui->can_resize(MInterceptedPlugin);
  }

  bool gui_get_resize_hints(clap_gui_resize_hints_t *hints) final {
    SAT_Print("\n");
    return ext.gui->get_resize_hints(MInterceptedPlugin,hints);
  }

  bool gui_adjust_size(uint32_t *width, uint32_t *height) final {
    SAT_Print(": *width %i *height %i -> ",*width,*height);
    return ext.gui->adjust_size(MInterceptedPlugin,width,height);
    SAT_DPrint("(%i,%i)\n",*width,*height);
  }

  bool gui_set_size(uint32_t width, uint32_t height) final {
    SAT_Print(": width %i height %i\n",width,height);
    return ext.gui->set_size(MInterceptedPlugin,width,height);
  }

  bool gui_set_parent(const clap_window_t *window) final {
    SAT_Print("\n");
    return ext.gui->set_parent(MInterceptedPlugin,window);
  }

  bool gui_set_transient(const clap_window_t *window) final {
    SAT_Print("\n");
    return ext.gui->set_transient(MInterceptedPlugin,window);
  }

  void gui_suggest_title(const char *title) final { 
    SAT_Print(": title %s\n",title);
    return ext.gui->suggest_title(MInterceptedPlugin,title);
  }

  bool gui_show() final {
    SAT_Print("\n");
    return ext.gui->show(MInterceptedPlugin);
  }

  bool gui_hide() final {
    SAT_Print("\n");
    return ext.gui->hide(MInterceptedPlugin);
  }

  //----------

  uint32_t latency_get() final {
    SAT_PRINT;
    return ext.latency->get(MInterceptedPlugin);
  }

  //----------

  uint32_t midi_mappings_count() final {
    SAT_PRINT;
    return ext.midi_mappings->count(MInterceptedPlugin);
  }

  bool midi_mappings_get(uint32_t index, clap_midi_mapping_t *mapping) final {
    SAT_PRINT;
    return ext.midi_mappings->get(MInterceptedPlugin,index,mapping);
  }

  //----------

  uint32_t note_name_count() final {
    SAT_PRINT;
    return ext.note_name->count(MInterceptedPlugin);
  }

  bool note_name_get(uint32_t index, clap_note_name_t *note_name) final {
    SAT_PRINT;
    return ext.note_name->get(MInterceptedPlugin,index,note_name);
  }

  //----------

  uint32_t note_ports_count(bool is_input) final {
    SAT_PRINT;
    return ext.note_ports->count(MInterceptedPlugin,is_input);
  }

  bool note_ports_get(uint32_t index, bool is_input, clap_note_port_info_t* info) final {
    SAT_PRINT;
    return ext.note_ports->get(MInterceptedPlugin,index,is_input,info);
  }

  //----------

  void param_indication_set_mapping(clap_id param_id, bool has_mapping, const clap_color_t *color, const char *label, const char *description) final {
    SAT_PRINT;
    return ext.param_indication->set_mapping(MInterceptedPlugin,param_id,has_mapping,color,label,description);
  }

  void param_indication_set_automation(clap_id param_id, uint32_t automation_state, const clap_color_t *color) final {
    SAT_PRINT;
    return ext.param_indication->set_automation(MInterceptedPlugin,param_id,automation_state,color);
  }

  //----------

  uint32_t params_count() final {
    SAT_PRINT;
    return ext.params->count(MInterceptedPlugin);
  }

  bool params_get_info(uint32_t param_index, clap_param_info_t *param_info) final {
    SAT_PRINT;
    return ext.params->get_info(MInterceptedPlugin,param_index,param_info);
  }

  bool params_get_value(clap_id param_id, double *value) final {
    SAT_PRINT;
    return ext.params->get_value(MInterceptedPlugin,param_id,value);
  }

  bool params_value_to_text(clap_id param_id, double value, char *display, uint32_t size) final {
    SAT_PRINT;
    return ext.params->value_to_text(MInterceptedPlugin,param_id,value,display,size);
  }

  bool params_text_to_value(clap_id param_id, const char *display, double *value) final {
    SAT_PRINT;
    return ext.params->text_to_value(MInterceptedPlugin,param_id,display,value);
  }

  void params_flush(const clap_input_events_t *in, const clap_output_events_t *out) final { 
    SAT_PRINT;
    ext.params->flush(MInterceptedPlugin,in,out);
  }

  //----------

  void posix_fd_support_on_fd(int fd, clap_posix_fd_flags_t flags) final { 
    SAT_PRINT;
    ext.posix_fd_support->on_fd(MInterceptedPlugin,fd,flags);
  }

  //----------

  bool preset_load_from_location(uint32_t location_kind, const char *location, const char *load_key) final {
    SAT_PRINT;
    return ext.preset_load->from_location(MInterceptedPlugin,location_kind,location,load_key);
  }

  //----------

  uint32_t remote_controls_count() final {
    SAT_PRINT;
    return ext.remote_controls->count(MInterceptedPlugin);
  }

  bool remote_controls_get(uint32_t page_index, clap_remote_controls_page_t *page) final {
    SAT_PRINT;
    return ext.remote_controls->get(MInterceptedPlugin,page_index,page);
  }

  //----------

  bool render_has_hard_realtime_requirement() final {
    SAT_PRINT;
    return ext.render->has_hard_realtime_requirement(MInterceptedPlugin);
  }

  bool render_set(clap_plugin_render_mode mode) final {
    SAT_PRINT;
    return ext.render->set(MInterceptedPlugin,mode);
  }

  //----------

  void resource_directory_set_directory(const char *path, bool is_shared) final {
    SAT_PRINT;
    ext.resource_directory->set_directory(MInterceptedPlugin,path,is_shared);
  }

  void resource_directory_collect(bool all) final {
    SAT_PRINT;
    ext.resource_directory->collect(MInterceptedPlugin,all);
  }

  uint32_t resource_directory_get_files_count() final {
    SAT_PRINT;
    return ext.resource_directory->get_files_count(MInterceptedPlugin);
  }

  int32_t resource_directory_get_file_path(uint32_t index, char *path, uint32_t path_size) final {
    SAT_PRINT;
    return ext.resource_directory->get_file_path(MInterceptedPlugin,index,path,path_size);
  }

  //----------

  bool state_save(const clap_ostream_t *stream) final {
    SAT_PRINT;
    return ext.state->save(MInterceptedPlugin,stream);
  }

  bool state_load(const clap_istream_t *stream) final {
    SAT_PRINT;
    return ext.state->load(MInterceptedPlugin,stream);
  }

  //----------

  bool state_context_save(const clap_ostream_t *stream, uint32_t context_type) final {
    SAT_PRINT;
    return ext.state_context->save(MInterceptedPlugin,stream,context_type);
    return false; 
  }

  bool state_context_load(const clap_istream_t *stream, uint32_t context_type) final {
    SAT_PRINT;
    return ext.state_context->load(MInterceptedPlugin,stream,context_type);
  }

  //----------

  uint32_t surround_get_channel_map(bool is_input, uint32_t port_index, uint8_t *channel_map, uint32_t channel_map_capacity) final {
    SAT_PRINT;
    return ext.surround->get_channel_map(MInterceptedPlugin,is_input,port_index,channel_map,channel_map_capacity);
  }

  void surround_changed() final { 
    SAT_PRINT;
    ext.surround->changed(MInterceptedPlugin);
  }

  //----------

  uint32_t tail_get() final {
    SAT_PRINT;
    return ext.tail->get(MInterceptedPlugin);
  }

  //----------

  void thread_pool_exec(uint32_t task_index) final { 
    SAT_PRINT;
    return ext.thread_pool->exec(MInterceptedPlugin,task_index);
  }

  //----------

  void timer_support_on_timer(clap_id timer_id) final { 
    SAT_PRINT;
    return ext.timer_support->on_timer(MInterceptedPlugin,timer_id);
  }

  //----------

  void track_info_changed() final { 
    SAT_PRINT;
    return ext.track_info->changed(MInterceptedPlugin);
  }

  //----------

  uint32_t triggers_count() final {
    SAT_PRINT;
    return ext.triggers->count(MInterceptedPlugin);
  }

  bool triggers_get_info(uint32_t index, clap_trigger_info_t *trigger_info) final {
    SAT_PRINT;
    return ext.triggers->get_info(MInterceptedPlugin,index,trigger_info);
  }

  //----------

  void tuning_changed() final { 
    SAT_PRINT;
    return ext.tuning->changed(MInterceptedPlugin);
  }

  //----------

  bool voice_info_get(clap_voice_info_t *info) final {
    SAT_PRINT;
    return ext.voice_info->get(MInterceptedPlugin,info);
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
