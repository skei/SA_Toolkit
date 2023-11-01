#ifndef sat_plugin_included
#define sat_plugin_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/clap/sat_clap_plugin.h"
#include "plugin/sat_audio_port.h"
#include "plugin/sat_note_port.h"
#include "plugin/sat_parameter.h"
#include "plugin/sat_editor.h"
#include "plugin/sat_editor_listener.h"

//#include "plugin/sat_audio_processor.h"
// #include "plugin/sat_event_processor.h"
//#include "plugin/sat_parameter_manager.h"

//class SAT_ClapPlugin {};
// typedef SAT_Queue<uint32_t,SAT_PLUGIN_MAX_PARAM_EVENTS_PER_BLOCK> MHostParamQueue;
// typedef SAT_Queue<uint32_t,SAT_PLUGIN_MAX_GUI_EVENTS_PER_BLOCK>   MGuiParamQueue;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Plugin
: public SAT_ClapPlugin
, public SAT_EditorListener {

//------------------------------
private:
//------------------------------

  // MGuiParamQueue  MParamFromGuiToPluginQueue  = {};
  // MGuiParamQueue  MParamFromGuiToHostQueue    = {};
  // MHostParamQueue MParamFromHostToGui = {};
  // MHostParamQueue MModFromHostToGui = {};

//------------------------------
private:
//------------------------------

  const clap_plugin_descriptor_t* MDescriptor     = nullptr;
  SAT_ParameterArray              MParameters     = {};
  SAT_AudioPortArray              MAudioPorts     = {};
  SAT_NotePortArray               MNotePorts      = {};
  SAT_Editor*                     MEditor         = nullptr;
  SAT_Dictionary<const void*>     MExtensions     = {};
//SAT_AudioProcessor              MAudioProcessor = {};

//------------------------------
public:
//------------------------------

  SAT_Plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_ClapPlugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~SAT_Plugin() {
    #ifndef SAT_NO_AUTODELETE
      deleteAudioPorts();
      deleteNotePorts();
      deleteParameters();
    #endif
  }

//------------------------------
public: // extensions
//------------------------------

  void registerExtension(const char* AId, const void* APtr) {
    MExtensions.addItem(AId,APtr);
  }

//------------------------------
public: // parameters
//------------------------------

  virtual int32_t appendParameter(SAT_Parameter* AParameter) {
    int32_t index = MParameters.size();
    AParameter->setIndex(index);
    MParameters.append(AParameter);
    return index;
  }

  //----------

  virtual int32_t appendParameter(const clap_param_info_t* AInfo) {
    SAT_Parameter* parameter = new SAT_Parameter(AInfo);
    return appendParameter(parameter);
  }

  //----------

  virtual void deleteParameters() {
    uint32_t num = MParameters.size();
    for (uint32_t i=0; i<num; i++) {
      if (MParameters[i]) {
        delete MParameters[i];
        MParameters[i] = nullptr;
      }
    }
    MParameters.clear(true);
  }

//------------------------------
public: // audio ports
//------------------------------

  virtual int32_t appendAudioPort(SAT_AudioPort* APort) {
    int32_t index = MAudioPorts.size();
    APort->setIndex(index);
    MAudioPorts.append(APort);
    return index;
  }

  //----------

  virtual int32_t appendAudioPort(const clap_audio_port_info_t* AInfo) {
    SAT_AudioPort* port = new SAT_AudioPort(AInfo);
    return appendAudioPort(port);
  }

  //----------

  virtual void deleteAudioPorts() {
    uint32_t num = MAudioPorts.size();
    for (uint32_t i=0; i<num; i++) {
      if (MAudioPorts[i]) {
        delete MAudioPorts[i];
        MAudioPorts[i] = nullptr;
      }
    }
    MAudioPorts.clear(true);
  }

//------------------------------
public: // note ports
//------------------------------

  virtual int32_t appendNotePort(SAT_NotePort* APort) {
    int32_t index = MNotePorts.size();
    APort->setIndex(index);
    MNotePorts.append(APort);
    return index;
  }

  //----------

  virtual int32_t appendNotePort(const clap_note_port_info_t* AInfo) {
    SAT_NotePort* port = new SAT_NotePort(AInfo);
    return appendNotePort(port);
  }

  //----------

  virtual void deleteNotePorts() {
    uint32_t num = MNotePorts.size();
    for (uint32_t i=0; i<num; i++) {
      if (MNotePorts[i]) {
        delete MNotePorts[i];
        MNotePorts[i] = nullptr;
      }
    }
    MNotePorts.clear(true);
  }

//------------------------------
protected: // clap_plugin
//------------------------------

  bool init() override {
    return true;
  }

  //----------

  void destroy() override {
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) override {
    return true;
  }

  //----------

  void deactivate() override {
  }

  //----------

  bool start_processing() override {
    return true;
  }

  //----------

  void stop_processing() override {
  }

  //----------

  void reset() override {
  }

  //----------

  clap_process_status process(const clap_process_t *process) override {
    return CLAP_PROCESS_CONTINUE;
  }

  //----------

  const void* get_extension(const char *id) override {
    if (MExtensions.hasItem(id)) {
      return MExtensions.getItem(id);
    }
    return nullptr;
  }

  //----------

  void on_main_thread() override {
  }

//------------------------------
protected: // clap extensions
//------------------------------

  uint32_t audio_ports_count(bool is_input) override {
    return MAudioPorts.size();
  }

  //----------

  bool audio_ports_get(uint32_t index, bool is_input, clap_audio_port_info_t *info) override {
    memcpy(info,MAudioPorts[index]->getInfo(),sizeof(clap_audio_port_info_t));
    return true;
  }

  //------------------------------
  //
  //------------------------------

  uint32_t audio_ports_config_count() override {
    return 0;
  }

  //----------

  bool audio_ports_config_get(uint32_t index, clap_audio_ports_config_t *config) override {
    return true;
  }

  //----------

  bool audio_ports_config_select(clap_id config_id) override {
    return true;
  }

  //------------------------------
  //
  //------------------------------

  bool gui_is_api_supported(const char *api, bool is_floating) override {
    return false;
  }

  //----------

  bool gui_get_preferred_api(const char **api, bool *is_floating) override {
    return false;
  }

  //----------

  bool gui_create(const char *api, bool is_floating) override {
    return false;
  }

  //----------

  void gui_destroy() override {
  }

  //----------

  bool gui_set_scale(double scale) override {
    return false;
  }

  //----------

  bool gui_get_size(uint32_t *width, uint32_t *height) override {
    return false;
  }

  //----------

  bool gui_can_resize() override {
    return false;
  }

  //----------

  bool gui_get_resize_hints(clap_gui_resize_hints_t *hints) override {
    return false;
  }

  //----------

  bool gui_adjust_size(uint32_t *width, uint32_t *height) override {
    return false;
  }

  //----------

  bool gui_set_size(uint32_t width, uint32_t height) override {
    return false;
  }

  //----------

  bool gui_set_parent(const clap_window_t *window) override {
    return false;
  }

  //----------

  bool gui_set_transient(const clap_window_t *window) override {
    return false;
  }

  //----------

  void gui_suggest_title(const char *title) override {
  }

  //----------

  bool gui_show() override {
    return false;
  }

  //----------

  bool gui_hide() override {
    return false;
  }

  //------------------------------
  //
  //------------------------------

  uint32_t latency_get() override {
    return 0;
  }

  //------------------------------
  //
  //------------------------------

  uint32_t note_name_count() override {
    return 0;
  }

  //----------

  bool note_name_get(uint32_t index, clap_note_name_t *note_name) override {
    return false;
  }

  //------------------------------
  //
  //------------------------------

  uint32_t note_ports_count(bool is_input) override {
    return 0;
  }

  //----------

  bool note_ports_get(uint32_t index, bool is_input, clap_note_port_info_t *info) override {
    return false;
  }

  //------------------------------
  // params
  //------------------------------

  uint32_t params_count() override {
    return MParameters.size();
  }

  //----------

  bool params_get_info(uint32_t param_index, clap_param_info_t *param_info) override {
    memcpy(param_info,MParameters[param_index]->getInfo(),sizeof(clap_param_info_t));
    return true;
  }
  
  //----------

  bool params_get_value(clap_id param_id, double *out_value) override {
    *out_value = MParameters[param_id]->getValue();
    return true;
  }
  
  //----------

  bool params_value_to_text(clap_id param_id, double value, char *out_buffer, uint32_t out_buffer_capacity) override {
    return false;
  }
  
  //----------

  bool params_text_to_value(clap_id param_id, const char *param_value_text, double *out_value) override {
    return false;
  }
  
  //----------

  void params_flush(const clap_input_events_t *in, const clap_output_events_t *out) override {
  }

  //------------------------------
  //
  //------------------------------

  void posix_fd_support_on_fd(int fd, clap_posix_fd_flags_t flags) override {
  }

  //------------------------------
  //
  //------------------------------

  bool render_has_hard_realtime_requirement() override {
    return false;
  }

  //----------

  bool render_set(clap_plugin_render_mode mode) override {
    return false;
  }

  //------------------------------
  //
  //------------------------------

  bool state_save(const clap_ostream_t *stream) override {
    return false;
  }

  //----------

  bool state_load(const clap_istream_t *stream) override {
    return false;
  }

  //------------------------------
  //
  //------------------------------

  uint32_t tail_get() override {
    return 0;
  }

  //------------------------------
  //
  //------------------------------

  void thread_pool_exec(uint32_t task_index) override {
  }

  //------------------------------
  //
  //------------------------------

  void timer_on_timer(clap_id timer_id) override {
  }

  //------------------------------
  //
  //------------------------------

  bool voice_info_get(clap_voice_info_t *info) override {
    return false;
  }

//------------------------------
protected: // clap draft extensions
//------------------------------

  bool ambisonic_is_config_supported(const clap_ambisonic_config_t *config) override {
    return false; 
  }

  //----------

  bool ambisonic_get_config(bool is_input, uint32_t port_index, clap_ambisonic_config_t *config) override {
    return false; 
  }

  //------------------------------
  //
  //------------------------------

  bool audio_ports_activation_can_activate_while_processing() override {
    return false; 
  }

  //----------

  bool audio_ports_activation_set_active(bool is_input, uint32_t port_index, bool is_active, uint32_t sample_size) override {
    return false; 
  }

  //------------------------------
  //
  //------------------------------

  void check_for_update_check(bool include_preview) override {
  }

  //------------------------------
  //
  //------------------------------

  bool configurable_audio_ports_can_apply_configuration(const struct clap_audio_port_configuration_request *requests, uint32_t request_count) override {
    return false; 
  }

  //----------

  bool configurable_audio_ports_apply_configuration(const struct clap_audio_port_configuration_request *requests, uint32_t request_count) override {
    return false; 
  }

  //------------------------------
  //
  //------------------------------

  bool context_menu_populate(const clap_context_menu_target_t  *target, const clap_context_menu_builder_t *builder) override {
    return false; 
  }

  //----------

  bool context_menu_perform(const clap_context_menu_target_t *target, clap_id action_id) override {
    return false; 
  }

  //------------------------------
  //
  //------------------------------

  bool cv_get_channel_type(bool is_input, uint32_t port_index, uint32_t channel_index, uint32_t *channel_type) override {
    return false; 
  }

  //------------------------------
  //
  //------------------------------

  bool extensible_audio_ports_add_port(bool is_input, uint32_t channel_count, const char *port_type, const void *port_details) override {
    return false; 
  }

  //----------

  bool extensible_audio_ports_remove_port(bool is_input, uint32_t index) override {
    return false; 
  }

  //------------------------------
  //
  //------------------------------

  uint32_t midi_mappings_count() override {
    return 0;
  }

  //----------

  bool midi_mappings_get(uint32_t index, clap_midi_mapping_t *mapping) override {
    return false;
  }
  
  //------------------------------
  //
  //------------------------------

  void param_indication_set_mapping(clap_id param_id, bool has_mapping, const clap_color_t *color, const char *label, const char *description) override {
  }
  
  //----------

  void param_indication_set_automation(clap_id param_id, uint32_t automation_state, const clap_color_t *color) override {
  }
  
  //------------------------------
  //
  //------------------------------

  bool preset_load_from_location(uint32_t location_kind, const char *location, const char *load_key) override {
    return false;
  }
  
  //------------------------------
  //
  //------------------------------

  uint32_t remote_controls_count() override {
    return 0;
  }
  
  //----------

  bool remote_controls_get(uint32_t page_index, clap_remote_controls_page_t *page) override {
    return false;
  }
  
  //------------------------------
  //
  //------------------------------

  void resource_directory_set_directory(const char *path, bool is_shared) override {
  }

  //----------

  void resource_directory_collect(bool all) override {
  }

  //----------

  uint32_t resource_directory_get_files_count() override {
    return 0;
  }

  //----------

  int32_t resource_directory_get_file_path(uint32_t index, char *path, uint32_t path_size) override {
    return 0;
  }

  //------------------------------
  //
  //------------------------------

  bool state_context_save(const clap_ostream_t *stream, uint32_t context_type) override {
    return false;
  }
  
  //----------

  bool state_context_load(const clap_istream_t *stream, uint32_t context_type) override {
    return false;
  }

  //------------------------------
  //
  //------------------------------

  bool surround_is_channel_mask_supported(uint64_t channel_mask) override {
    return false;
  }

  //----------

  uint32_t surround_get_channel_map(bool is_input, uint32_t port_index, uint8_t *channel_map, uint32_t channel_map_capacity) override {
    return 0;
  }

  //------------------------------
  //
  //------------------------------

  void track_info_changed() override {
  }

  //------------------------------
  //
  //------------------------------

  uint32_t triggers_count() override {
    return 0;
  }

  //----------

  bool triggers_get_info(uint32_t index, clap_trigger_info_t *trigger_info) override {
    return false;
  }

  //------------------------------
  //
  //------------------------------

  void tuning_changed() override {
  }

//------------------------------
protected: // SAT_EditorListener
//------------------------------

};

//----------------------------------------------------------------------
#endif

