#ifndef sat_plugin_included
#define sat_plugin_included
//----------------------------------------------------------------------

#include "plugin/clap/sat_clap_plugin.h"
//#include "plugin/clap/sat_clap_host.h"

#include "plugin/sat_audio_port.h"
#include "plugin/sat_editor.h"
#include "plugin/sat_host.h"
#include "plugin/sat_note_port.h"
#include "plugin/sat_parameter.h"
#include "plugin/sat_process_context.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Plugin
: public SAT_ClapPlugin {

//------------------------------
private:
//------------------------------

  bool                        MIsInitialized            = false;
  bool                        MIsActivated              = false;
  bool                        MIsProcessing             = false;
//bool                        MIsEditorOpen             = false;
  clap_id                     MSelectedAudioPortsConfig = 0;
  clap_plugin_render_mode     MRenderMode               = CLAP_RENDER_REALTIME;
  SAT_Editor*                 MEditor                   = nullptr;
  SAT_Host*                   MHost                     = nullptr;
  SAT_ParameterArray          MParameters               = {};
  SAT_AudioPortArray          MAudioInputPorts          = {};
  SAT_AudioPortArray          MAudioOutputPorts         = {};
  SAT_NotePortArray           MNoteInputPorts           = {};
  SAT_NotePortArray           MNoteOutputPorts          = {};
  SAT_ProcessContext          MProcessContext           = {};
  SAT_ClapPluginExtensions    MExtensions               = {0};
//SAT_Dictionary<const void*> MExtensions               = {};

//------------------------------
public:
//------------------------------

  SAT_Plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_ClapPlugin(ADescriptor) {
    SAT_Log("SAT_Plugin()\n");
    MHost = new SAT_Host(AHost);
    SAT_Log("  host name %s\n",MHost->getName());
    SAT_Log("  host vendor %s\n",MHost->getVendor());
    SAT_Log("  host version %s\n",MHost->getVersion());
    SAT_Log("  host url %s\n",MHost->getUrl());
    logHostExtensions(AHost);
    //registerAllExtensions();
  }

//----------

  virtual ~SAT_Plugin() {
    SAT_Log("~SAT_Plugin()\n");
    #ifndef SAT_NO_AUTODELETE
      deleteParameters();
      deleteAudioInputPorts();
      deleteAudioOutputPorts();
      deleteNoteInputPorts();
      deleteNoteOutputPorts();
    #endif
    delete MHost;
  }

//------------------------------
public: // plugin
//------------------------------

  // Must be called after creating the plugin.
  // If init returns false, the host must destroy the plugin instance.
  // If init returns true, then the plugin is initialized and in the deactivated state.
  // [main-thread]

  bool init() override {
    //SAT_ClapPlugin::init();
    SAT_Log("SAT_Plugin.init\n");
    MIsInitialized = true;
    return true;
  }

  //----------

  // Free the plugin and its resources.
  // It is required to deactivate the plugin prior to this call.
  // [main-thread & !active]

  void destroy() override {
    SAT_Log("SAT_Plugin.destroy\n");
    MIsInitialized = false;
    //SAT_ClapPlugin::destroy();
    #ifdef SAT_DELETE_PLUGIN_IN_DESTROY
      delete this;
    #endif
  }

  //----------

  // Activate and deactivate the plugin.
  // In this call the plugin may allocate memory and prepare everything needed for the process
  // call. The process's sample rate will be constant and process's frame count will included in
  // the [min, max] range, which is bounded by [1, INT32_MAX].
  // Once activated the latency and port configuration must remain constant, until deactivation.
  // [main-thread & !active_state]

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) override {
    SAT_Log("SAT_Plugin.activate (srate %.1f minfr %i maxfr %i)\n",sample_rate,min_frames_count,max_frames_count);
    MProcessContext.samplerate = sample_rate;
    MProcessContext.minbufsize = min_frames_count;
    MProcessContext.maxbufsize = max_frames_count;
    MIsActivated = true;
    return true;
  }

  //----------

  // [main-thread & active_state]

  void deactivate() override {
    SAT_Log("SAT_Plugin.deactivate\n");
    MIsActivated = false;
  }

  //----------

  // Call start processing before processing.
  // [audio-thread & active_state & !processing_state]

  bool start_processing() override {
    SAT_Log("SAT_Plugin.start_processing\n");
    MIsProcessing = true;
    return true;
  }

  //----------

  // Call stop processing before sending the plugin to sleep.
  // [audio-thread & active_state & processing_state]

  void stop_processing() override {
    SAT_Log("SAT_Plugin.stop_processing\n");
    MIsProcessing = false;
  }

  //----------

  // - Clears all buffers, performs a full reset of the processing state (filters, oscillators,
  //   enveloppes, lfo, ...) and kills all voices.
  // - The parameter's value remain unchanged.
  // - clap_process.steady_time may jump backward.
  // [audio-thread & active_state]

  void reset() override {
    SAT_Log("SAT_Plugin.reset\n");
  }

  //----------

  // process audio, events, ...
  // All the pointers coming from clap_process_t and its nested attributes,
  // are valid until process() returns.
  // [audio-thread & active_state & processing_state]

  clap_process_status process(const clap_process_t* process) override {
    MProcessContext.process = process;
    // processTransport
    // processEvents
    // processAudio
    // postProcess
    return CLAP_PROCESS_CONTINUE;
  }

  //----------

  // Query an extension.
  // The returned pointer is owned by the plugin.
  // It is forbidden to call it before plugin->init().
  // You can call it within plugin->init() call, and after.
  // [thread-safe]

  const void* get_extension(const char *id) override {
    SAT_Log("SAT_Plugin.get_extension (id %s)\n",id);

    if (strcmp(id,CLAP_EXT_AMBISONIC)               == 0) return MExtensions.ambisonic;
    if (strcmp(id,CLAP_EXT_AUDIO_PORTS_ACTIVATION)  == 0) return MExtensions.audio_ports_activation;
    if (strcmp(id,CLAP_EXT_AUDIO_PORTS_CONFIG)      == 0) return MExtensions.audio_ports_config;
    if (strcmp(id,CLAP_EXT_AUDIO_PORTS)             == 0) return MExtensions.audio_ports;
    if (strcmp(id,CLAP_EXT_CHECK_FOR_UPDATE)        == 0) return MExtensions.check_for_update;
    if (strcmp(id,CLAP_EXT_CONTEXT_MENU)            == 0) return MExtensions.context_menu;
    if (strcmp(id,CLAP_EXT_CV)                      == 0) return MExtensions.cv;
    if (strcmp(id,CLAP_EXT_GUI)                     == 0) return MExtensions.gui;
    if (strcmp(id,CLAP_EXT_LATENCY)                 == 0) return MExtensions.latency;
    if (strcmp(id,CLAP_EXT_MIDI_MAPPINGS)           == 0) return MExtensions.midi_mappings;
    if (strcmp(id,CLAP_EXT_NOTE_NAME)               == 0) return MExtensions.note_name;
    if (strcmp(id,CLAP_EXT_NOTE_PORTS)              == 0) return MExtensions.note_ports;
    if (strcmp(id,CLAP_EXT_PARAM_INDICATION)        == 0) return MExtensions.param_indication;
    if (strcmp(id,CLAP_EXT_PARAMS)                  == 0) return MExtensions.params;
    if (strcmp(id,CLAP_EXT_PRESET_LOAD)             == 0) return MExtensions.preset_load;
    if (strcmp(id,CLAP_EXT_REMOTE_CONTROLS)         == 0) return MExtensions.remote_controls;
    if (strcmp(id,CLAP_EXT_RESOURCE_DIRECTORY)      == 0) return MExtensions.resource_directory;
    if (strcmp(id,CLAP_EXT_RENDER)                  == 0) return MExtensions.render;
    if (strcmp(id,CLAP_EXT_STATE)                   == 0) return MExtensions.state;
    if (strcmp(id,CLAP_EXT_STATE_CONTEXT)           == 0) return MExtensions.state_context;
    if (strcmp(id,CLAP_EXT_SURROUND)                == 0) return MExtensions.surround;
    if (strcmp(id,CLAP_EXT_THREAD_POOL)             == 0) return MExtensions.thread_pool;
    if (strcmp(id,CLAP_EXT_TIMER_SUPPORT)           == 0) return MExtensions.timer_support;
    if (strcmp(id,CLAP_EXT_TRACK_INFO)              == 0) return MExtensions.track_info;
    if (strcmp(id,CLAP_EXT_TRIGGERS)                == 0) return MExtensions.triggers;
    if (strcmp(id,CLAP_EXT_TUNING)                  == 0) return MExtensions.tuning;
    if (strcmp(id,CLAP_EXT_VOICE_INFO)              == 0) return MExtensions.voice_info;

    //for (uint32_t i=0; i<NUM_CLAP_PLUGIN_EXTENSIONS; i++) {
    //  if (strcmp(id,SAT_ClapPluginExtensionIds[i]) == 0)
    //    return SAT_ClapPluginExtensions[i]; // todo
    //}

    return nullptr;
  }

  //----------

  // Called by the host on the main thread in response to a previous call to:
  //   host->request_callback(host);
  // [main-thread]

  void on_main_thread() override {
    SAT_Print("\n");
  }

//------------------------------
public: // ambisonic
//------------------------------

  // Returns true on success
  // config_id: the configuration id, see clap_plugin_audio_ports_config.
  // If config_id is CLAP_INVALID_ID, then this function queries the current port info.
  // [main-thread]

  bool ambisonic_get_info(clap_id config_id, bool is_input,  uint32_t port_index, clap_ambisonic_info_t *info) override {
    SAT_Print("\n");
    return false;
  }

//------------------------------
public: // audio ports activation
//------------------------------

  // Returns true if the plugin supports activation/deactivation while processing.
  // [main-thread]

  bool audio_ports_activation_can_activate_while_processing() override {
    SAT_Print("\n");
    return false;
  }

  //----------

  // Activate the given port.
  // It is only possible to activate and de-activate on the audio-thread if
  // can_activate_while_processing() returns true.
  // returns false if failed, or invalid parameters
  // [active ? audio-thread : main-thread]

  bool audio_ports_activation_set_active(bool is_input, uint32_t port_index, bool is_active) override {
    SAT_Print("\n");
    return false;
  }

//------------------------------
public: // audio ports config
//------------------------------

  // gets the number of available configurations
  // [main-thread]

  uint32_t audio_ports_config_count() override {
    SAT_Print("\n");
    return 1;
  }

  //----------

  // gets information about a configuration
  // [main-thread]

  //TODO: hack.. fix this..

  bool audio_ports_config_get(uint32_t index, clap_audio_ports_config_t *config) override {
    SAT_Print("\n");
    config->id                        = 0;
    strcpy(config->name,"audio ports config 1");
    config->input_port_count          = 2;
    config->output_port_count         = 2;
    config->has_main_input            = true;
    config->main_input_channel_count  = 2;
    config->main_input_port_type      = CLAP_PORT_STEREO;
    config->has_main_output           = true;
    config->main_output_channel_count = 2;
    config->main_output_port_type     = CLAP_PORT_STEREO;
    return true;
  }

  //----------

  // selects the configuration designated by id
  // returns true if the configuration could be applied.
  // Once applied the host should scan again the audio ports.
  // [main-thread,plugin-deactivated]

  bool audio_ports_config_select(clap_id config_id) override {
    SAT_Print("\n");
    MSelectedAudioPortsConfig = config_id;
    return false;
  }

//------------------------------
public: // audio ports
//------------------------------

  // number of ports, for either input or output
  // [main-thread]

  uint32_t audio_ports_count(bool is_input) override {
    SAT_Print("\n");
    if (is_input) return MAudioInputPorts.size();
    else return MAudioOutputPorts.size();
  }

  //----------

  // get info about about an audio port.
  // [main-thread]

  bool audio_ports_get(uint32_t index, bool is_input, clap_audio_port_info_t *info) override {
    SAT_Print("\n");
    if (is_input) {
      clap_audio_port_info_t* pinfo = MAudioInputPorts[index]->getInfo();
      memcpy(info,pinfo,sizeof(clap_audio_port_info_t));
      return true;
    }
    else {
      clap_audio_port_info_t* pinfo = MAudioOutputPorts[index]->getInfo();
      memcpy(info,pinfo,sizeof(clap_audio_port_info_t));
      return true;
    }
  }

//------------------------------
public: // check for updates
//------------------------------

  // [main-thread]

  void check_for_updates_check(bool include_preview) override {
    SAT_Print("\n");
  }

//------------------------------
public: // context menu
//------------------------------

  // Insert plugin's menu items into the menu builder.
  // If target is null, assume global context.
  // [main-thread]

  //typedef struct clap_context_menu_target {
  //   uint32_t kind;
  //   clap_id  id;
  //} clap_context_menu_target_t;

  //typedef struct clap_context_menu_builder {
  //   void *ctx;
  //   bool(CLAP_ABI *add_item)(const struct clap_context_menu_builder *builder, clap_context_menu_item_kind_t item_kind, const void *item_data);
  //   bool(CLAP_ABI *supports)(const struct clap_context_menu_builder *builder, clap_context_menu_item_kind_t item_kind);
  //} clap_context_menu_builder_t;

  bool context_menu_populate(const clap_context_menu_target_t *target, const clap_context_menu_builder_t *builder) override {
    SAT_Print("\n");
    return false;
  }

  //----------

  // Performs the given action, which was previously provided to the host via populate().
  // If target is null, assume global context.
  // [main-thread]

  bool context_menu_perform(const clap_context_menu_target_t *target, clap_id action_id) override {
    SAT_Print("\n");
    return false;
  }

//------------------------------
public: // cv
//------------------------------

  // Returns true on success.
  // [main-thread]

  bool cv_get_channel_type(bool is_input, uint32_t port_index, uint32_t channel_index, uint32_t *channel_type) override {
    SAT_Print("\n");
    return false;
  }

//------------------------------
public: // gui
//------------------------------

  // Returns true if the requested gui api is supported
  // [main-thread]

  bool gui_is_api_supported(const char *api, bool is_floating) override {
    SAT_Print("\n");
    if (is_floating == true) return false;
    #ifdef SAT_LINUX
      if (strcmp(api,CLAP_WINDOW_API_X11) != 0) return false;
    #endif
    #ifdef SAT_WIN32
      if (strcmp(api,CLAP_WINDOW_API_WIN32) != 0) return false;
    #endif
    return true;
  }

  //----------

  // Returns true if the plugin has a preferred api.
  // The host has no obligation to honor the plugin preferrence, this is just a hint.
  // The const char **api variable should be explicitly assigned as a pointer to
  // one of the CLAP_WINDOW_API_ constants defined above, not strcopied.
  // [main-thread]

  bool gui_get_preferred_api(const char **api, bool *is_floating) override {
    SAT_Print("\n");
    *is_floating = false;
    #ifdef SAT_LINUX
      *api = CLAP_WINDOW_API_X11;
    #endif
    #ifdef SAT_WIN32
      *api = CLAP_WINDOW_API_WIN32;
    #endif
    return true;
  }

  //----------

  // Create and allocate all resources necessary for the gui.
  // If is_floating is true, then the window will not be managed by the host. The plugin
  // can set its window to stays above the parent window, see set_transient().
  // api may be null or blank for floating window.
  // If is_floating is false, then the plugin has to embbed its window into the parent window, see
  // set_parent().
  // After this call, the GUI may not be visible yet; don't forget to call show().
  // [main-thread]

  bool gui_create(const char *api, bool is_floating) override {
    SAT_Print("\n");
    if (is_floating == true) return false;
    #ifdef SAT_LINUX
      if (strcmp(api,CLAP_WINDOW_API_X11) != 0) return false;
    #endif
    #ifdef SAT_WIN32
      if (strcmp(api,CLAP_WINDOW_API_WIN32) != 0) return false;
    #endif
    MEditor = new SAT_Editor();
    if (MEditor) {
      //MEditor->create();
      return true;
    }
    return false;
  }

  //----------

  // Free all resources associated with the gui.
  // [main-thread]

  void gui_destroy() override {
    SAT_Print("\n");
    //MEditor->destroy();
    delete MEditor;
  }

  //----------

  // Set the absolute GUI scaling factor, and override any OS info.
  // Should not be used if the windowing api relies upon logical pixels.
  // If the plugin prefers to work out the scaling factor itself by querying the OS directly,
  // then ignore the call.
  // Returns true if the scaling could be applied
  // Returns false if the call was ignored, or the scaling could not be applied.
  // [main-thread]

  bool gui_set_scale(double scale) override {
    SAT_Print("\n");
    return MEditor->set_scale(scale);
  }

  //----------

  // Get the current size of the plugin UI.
  // clap_plugin_gui->create() must have been called prior to asking the size.
  // [main-thread]

  bool gui_get_size(uint32_t *width, uint32_t *height) override {
    SAT_Print("\n");
    return MEditor->get_size(width,height);
  }

  //----------

  // Returns true if the window is resizeable (mouse drag).
  // Only for embedded windows.
  // [main-thread]

  bool gui_can_resize() override {
    SAT_Print("\n");
    return MEditor->can_resize();
  }

  //----------

  // Returns true if the plugin can provide hints on how to resize the window.
  // [main-thread]

  //typedef struct clap_gui_resize_hints {
  //   bool can_resize_horizontally;
  //   bool can_resize_vertically;
  //   bool     preserve_aspect_ratio;
  //   uint32_t aspect_ratio_width;
  //   uint32_t aspect_ratio_height;
  //} clap_gui_resize_hints_t;

  bool gui_get_resize_hints(clap_gui_resize_hints_t *hints) override {
    SAT_Print("\n");
    return MEditor->get_resize_hints(hints);
  }

  //----------

  // If the plugin gui is resizable, then the plugin will calculate the closest
  // usable size which fits in the given size.
  // This method does not change the size.
  // Only for embedded windows.
  // [main-thread]

  bool gui_adjust_size(uint32_t *width, uint32_t *height) override {
    SAT_Print("\n");
    return MEditor->adjust_size(width,height);
  }

  //----------

  // Sets the window size. Only for embedded windows.
  // [main-thread]

  bool gui_set_size(uint32_t width, uint32_t height) override {
    SAT_Print("\n");
    return MEditor->set_size(width,height);
  }

  //----------

  // Embbeds the plugin window into the given window.
  // [main-thread & !floating]

  bool gui_set_parent(const clap_window_t *window) override {
    SAT_Print("\n");
    return MEditor->set_parent(window);
  }

  //----------

  // Set the plugin floating window to stay above the given window.
  // [main-thread & floating]

  bool gui_set_transient(const clap_window_t *window) override {
    SAT_Print("\n");
    return MEditor->set_transient(window);
  }

  //----------

  // Suggests a window title. Only for floating windows.
  // [main-thread & floating]

  void gui_suggest_title(const char *title) override {
    SAT_Print("\n");
    return MEditor->suggest_title(title);
  }

  //----------

  // Show the window.
  // [main-thread]

  bool gui_show() override {
    SAT_Print("\n");
    return MEditor->show();
  }

  //----------

  // Hide the window, this method does not free the resources, it just hides
  // the window content. Yet it may be a good idea to stop painting timers.
  // [main-thread]

  bool gui_hide() override {
    SAT_Print("\n");
    return MEditor->hide();
  }

//------------------------------
public: // latency
//------------------------------

  // Returns the plugin latency.
  // [main-thread]

  uint32_t latency_get() override {
    SAT_Print("\n");
    return 0;
  }

//------------------------------
public: // midi mappings
//------------------------------

  // [main-thread]

  uint32_t midi_mappings_count() override {
    SAT_Print("\n");
    return 0;
  }

  //----------

  // [main-thread]

  bool midi_mappings_get(uint32_t index, clap_midi_mapping_t *mapping) override {
    SAT_Print("\n");
    return false;
  }

//------------------------------
public: // note name
//------------------------------

  // Return the number of note names
  // [main-thread]

  uint32_t note_name_count() override {
    SAT_Print("\n");
    return 0;
  }

  //----------

  // Returns true on success and stores the result into note_name
  // [main-thread]

  bool note_name_get(uint32_t index, clap_note_name_t *note_name) override {
    SAT_Print("\n");
    return false;
  }

//------------------------------
public: // note ports
//------------------------------

  // number of ports, for either input or output
  // [main-thread]

  uint32_t note_ports_count(bool is_input) override {
    SAT_Print("\n");
    return 0;
  }

  //----------

  // get info about about a note port.
  // [main-thread]

  bool note_ports_get(uint32_t index, bool is_input, clap_note_port_info_t* info) override {
    SAT_Print("\n");
    return false;
  }

//------------------------------
public: // param indication
//------------------------------

  // Sets or clears a mapping indication.
  // has_mapping: does the parameter currently has a mapping?
  // color: if set, the color to use to highlight the control in the plugin GUI
  // label: if set, a small string to display on top of the knob which identifies the hardware
  // controller description: if set, a string which can be used in a tooltip, which describes the
  // current mapping
  // Parameter indications should not be saved in the plugin context, and are off by default.
  // [main-thread]

  void param_indication_set_mapping(clap_id param_id, bool has_mapping, const clap_color_t *color, const char *label, const char *description) override {
    SAT_Print("\n");
  }

  //----------

  // Sets or clears an automation indication.
  // automation_state: current automation state for the given parameter
  // color: if set, the color to use to display the automation indication in the plugin GUI
  // Parameter indications should not be saved in the plugin context, and are off by default.
  // [main-thread]

  void param_indication_set_automation(clap_id param_id, uint32_t automation_state, const clap_color_t *color) override {
    SAT_Print("\n");
  }

//------------------------------
public: // params
//------------------------------

  // Returns the number of parameters.
  // [main-thread]

  uint32_t params_count() override {
    SAT_Print("\n");
    return MParameters.size();
  }

  //----------

  // Copies the parameter's info to param_info. Returns true on success.
  // [main-thread]

  bool params_get_info(uint32_t param_index, clap_param_info_t* param_info) override {
    SAT_Print("\n");
    SAT_Parameter* parameter = MParameters[param_index];
    clap_param_info_t* info = parameter->getParamInfo();
    memcpy(param_info,info,sizeof(clap_param_info_t));
    return true;
  }

  //----------

  // Writes the parameter's current value to out_value. Returns true on success.
  // [main-thread]

  bool params_get_value(clap_id param_id, double *out_value) override {
    SAT_Print("\n");
    double value = MParameters[param_id]->getValue();
    *out_value = value;
    return true;
  }

  //----------

  // Fills out_buffer with a null-terminated UTF-8 string that represents the parameter at the
  // given 'value' argument. eg: "2.3 kHz". Returns true on success. The host should always use
  // this to format parameter values before displaying it to the user. [main-thread]

  bool params_value_to_text(clap_id param_id, double value, char* out_buffer, uint32_t out_buffer_capacity) override {
    SAT_Print("\n");
    uint32_t index = param_id;
    SAT_Parameter* parameter = MParameters[index];
    return parameter->valueToText(value,out_buffer,out_buffer_capacity);
  }

  //----------

  // Converts the null-terminated UTF-8 param_value_text into a double and writes it to out_value.
  // Returns true on success. The host can use this to convert user input into a parameter value.
  // [main-thread]

  bool params_text_to_value(clap_id param_id, const char* param_value_text, double* out_value) override {
    SAT_Print("\n");
    uint32_t index = param_id;
    SAT_Parameter* parameter = MParameters[index];
    return parameter->textToValue(param_value_text,out_value);
  }

  //----------

  // Flushes a set of parameter changes.
  // This method must not be called concurrently to clap_plugin->process().
  // NOTE: if the plugin is processing, then the process() call will already achieve the
  // parameter update (bi-directional), so a call to flush isn't required, also be aware
  // that the plugin may use the sample offset in process(), while this information would be
  // lost within flush().
  // [active ? audio-thread : main-thread]

  void params_flush(const clap_input_events_t  *in, const clap_output_events_t *out) override {
    SAT_Print("\n");
    //handleEvents(in,out);
  }

//------------------------------
public: // posix fd support
//------------------------------

  // This callback is "level-triggered".
  // It means that a writable fd will continuously produce "on_fd()" events;
  // don't forget using modify_fd() to remove the write notification once you're
  // done writting.
  // [main-thread]

  void posix_fd_support_on_fd(int fd, clap_posix_fd_flags_t flags) override {
    SAT_Print("\n");
  }

//------------------------------
public: // preset load
//------------------------------

  // Loads a preset in the plugin native preset file format from a URI. eg:
  // - "file:///home/abique/.u-he/Diva/Presets/Diva/HS Bass Nine.h2p", load_key: null
  // - "plugin://<plugin-id>", load_key: <XXX>
  // The preset discovery provider defines the uri and load_key to be passed to this function.
  // [main-thread]

  bool preset_load_from_uri(const char *uri, const char *load_key) override {
    SAT_Print("\n");
    return false;
  }

//------------------------------
public: // remote controls
//------------------------------

  // Returns the number of pages.
  // [main-thread]

  uint32_t remote_controls_count() override {
    SAT_Print("\n");
    return 1;
  }

  //----------

  // Get a page by index.
  // [main-thread]

  bool remote_controls_get(uint32_t page_index, clap_remote_controls_page_t *page) override {
    SAT_Print("\n");
    switch (page_index) {
      case 0: {
        page->page_id = 0;
        strcpy(page->page_name,"Page 1");
        strcpy(page->section_name,"Section 1");
        page->param_ids[0] = 0;
        page->param_ids[1] = 1;
        page->param_ids[2] = 2;
        page->param_ids[3] = 3;
        page->param_ids[4] = 4;
        page->param_ids[5] = 5;
        page->param_ids[6] = 6;
        page->param_ids[7] = 7;
        return true;
      }
    }
    return false;
  }

//------------------------------
public: // render
//------------------------------

  // Returns true if the plugin has an hard requirement to process in real-time.
  // This is especially useful for plugin acting as a proxy to an hardware device.
  // [main-thread]

  bool render_has_hard_realtime_requirement() override {
    SAT_Print("\n");
    return false;
  }

  //----------

  // Returns true if the rendering mode could be applied.
  // [main-thread]

  bool render_set(clap_plugin_render_mode mode) override {
    SAT_Print("\n");
    MRenderMode = mode;
    return false;
  }

//------------------------------
public: // resource directory
//------------------------------

  // Sets the directory in which the plugin can save its resources.
  // The directory remains valid until it is overriden or the plugin is destroyed.
  // If path is null or blank, it clears the directory location.
  // path must be absolute.
  // [main-thread]

  void resource_directory_set_directory(const char *path, bool is_shared) override {
    SAT_Print("\n");
  }

  //----------

  // Asks the plugin to put its resources into the resources directory.
  // It is not necessary to collect files which belongs to the plugin's
  // factory content unless the param all is true.
  // [main-thread]

  void resource_directory_collect(bool all) override {
    SAT_Print("\n");
  }

  //----------

  // Returns the number of files used by the plugin in the shared resource folder.
  // [main-thread]

  uint32_t resource_directory_get_files_count() override {
    SAT_Print("\n");
    return 0;
  }

  //----------

  // Retrieves relative file path to the resources directory.
  // @param path writable memory to store the path
  // @param path_size number of available bytes in path
  // Returns the number of bytes in the path, or -1 on error
  // [main-thread]

  int32_t resource_directory_get_file_path(uint32_t index, char *path, uint32_t path_size) override {
    SAT_Print("\n");
    return -1;
  }

//------------------------------
public: // state
//------------------------------

  // Saves the plugin state into stream.
  // Returns true if the state was correctly saved.
  // [main-thread]

  bool state_save(const clap_ostream_t *stream) override {
    //SAT_PRINT;
    uint32_t total = 0;
    uint32_t written = 0;
    uint32_t version = 0;
    uint32_t num_params = MParameters.size();
    // version
    written = stream->write(stream,&version,sizeof(uint32_t));
    if (written != sizeof(uint32_t)) {
      SAT_Print("state_save: error writing version\n");
      return false;
    }
    total += sizeof(uint32_t);
    // num params
    written = stream->write(stream,&num_params,sizeof(uint32_t));
    if (written != sizeof(uint32_t)) {
      SAT_Print("state_save: error writing parameter count\n");
      return false;
    }
    total += sizeof(uint32_t);
    // param values
    for (uint32_t i=0; i<num_params; i++) {
      double value = MParameters[i]->getValue();
      written = stream->write(stream,&value,sizeof(double));
      if (written != sizeof(double)) {
        SAT_Print("state_load: error writing parameter %i\n",i);
        return false;
      }
      total += sizeof(double);
    }
    SAT_Print("total: %i\n",total);
    return true;
  }

  //----------

  // Loads the plugin state from stream.
  // Returns true if the state was correctly restored.
  // [main-thread]

  bool state_load(const clap_istream_t *stream) override {
    //SAT_PRINT;
    uint32_t total = 0;
    uint32_t read = 0;
    uint32_t version = 0;
    uint32_t num_params = 0;
    // version
    read = stream->read(stream,&version,sizeof(uint32_t));
    if (read != sizeof(uint32_t)) {
      SAT_Print("state_load: error reading version\n");
      return false;
    }
    total += sizeof(uint32_t);
    //TODO: check version
    // num params
    read = stream->read(stream,&num_params,sizeof(uint32_t));
    if (read != sizeof(uint32_t)) {
      SAT_Print("state_load: error reading parameter count\n");
      return false;
    }
    total += sizeof(uint32_t);
    //TODO: check num params = marameters.size
    if (num_params != MParameters.size()) {
      SAT_Print("state_load: wrong parameter count\n");
      return false;
    }
    // param values
    for (uint32_t i=0; i<num_params; i++) {
      double value = 0.0;
      read = stream->read(stream,&value,sizeof(double));
      if (read != sizeof(double)) {
        SAT_Print("state_load: error reading parameter %i\n",i);
        return false;
      }
      total += sizeof(double);
      MParameters[i]->setValue(value);
    }

    //SAT_Print("total: %i\n",total);
    //updateParameterValues();
    //updateEditorParameterValues();

    return true;
  }

//------------------------------
public: // state context
//------------------------------

  // Saves the plugin state into stream, according to context_type.
  // Returns true if the state was correctly saved.
  // Note that the result may be loaded by both clap_plugin_state.load() and
  // clap_plugin_state_context.load().
  // [main-thread]

  bool state_context_save(const clap_ostream_t *stream, uint32_t context_type) override {
    SAT_Print("\n");
    return false;
  }

  //----------

  // Loads the plugin state from stream, according to context_type.
  // Returns true if the state was correctly restored.
  // Note that the state may have been saved by clap_plugin_state.save() or
  // clap_plugin_state_context.save() with a different context_type.
  // [main-thread]

  bool state_context_load(const clap_istream_t *stream, uint32_t context_type) override {
    SAT_Print("\n");
    return false;
  }

//------------------------------
public: // surround
//------------------------------

  // Stores into the channel_map array, the surround identifer of each channels.
  // Returns the number of elements stored in channel_map.
  // config_id: the configuration id, see clap_plugin_audio_ports_config.
  // If config_id is CLAP_INVALID_ID, then this function queries the current port info.
  // [main-thread]

  uint32_t surround_get_channel_map(clap_id config_id, bool is_input, uint32_t port_index, uint8_t *channel_map, uint32_t channel_map_capacity) override {
    SAT_Print("\n");
    return 0;
  }

  //----------

  // Informs the plugin that the host preferred channel map has changed.
  // [main-thread]

  void surround_changed() override {
    SAT_Print("\n");
  }

//------------------------------
public: // tail
//------------------------------

  // Returns tail length in samples.
  // Any value greater or equal to INT32_MAX implies infinite tail.
  // [main-thread,audio-thread]

  uint32_t tail_get() override {
    SAT_Print("\n");
    return 0;
  }

//------------------------------
public: // thread pool
//------------------------------

  // Called by the thread pool

  void thread_pool_exec(uint32_t task_index) override {
    SAT_Print("\n");
  }

//------------------------------
public: // timer support
//------------------------------

  // [main-thread]

  void timer_support_on_timer(clap_id timer_id) override {
    SAT_Print("\n");
  }

//------------------------------
public: // track info
//------------------------------

  // Called when the info changes.
  // [main-thread]

  void track_info_changed() override {
    SAT_Print("\n");
    clap_track_info_t info;
    if (MHost->ext.track_info) {
      char flags_text[256] = {0};
      MHost->ext.track_info->get(MHost->getHost(),&info);
      SAT_Print("track_info:\n");
      SAT_Print("  name: %s color %.2f,.2f,.2f\n",info.name,info.color.red,info.color.green,info.color.blue);
      SAT_Print("  channels: %i port: %s\n",info.audio_channel_count,info.audio_port_type);
      if (info.flags & CLAP_TRACK_INFO_HAS_TRACK_NAME)      SAT_Print("  CLAP_TRACK_INFO_HAS_TRACK_NAME");
      if (info.flags & CLAP_TRACK_INFO_HAS_TRACK_COLOR)     SAT_Print("  CLAP_TRACK_INFO_HAS_TRACK_COLOR");
      if (info.flags & CLAP_TRACK_INFO_HAS_AUDIO_CHANNEL)   SAT_Print("  CLAP_TRACK_INFO_HAS_AUDIO_CHANNEL");
      if (info.flags & CLAP_TRACK_INFO_IS_FOR_RETURN_TRACK) SAT_Print("  CLAP_TRACK_INFO_IS_FOR_RETURN_TRACK");
      if (info.flags & CLAP_TRACK_INFO_IS_FOR_BUS)          SAT_Print("  CLAP_TRACK_INFO_IS_FOR_BUS");
      if (info.flags & CLAP_TRACK_INFO_IS_FOR_MASTER)       SAT_Print("  CLAP_TRACK_INFO_IS_FOR_MASTER");
    }
  }

//------------------------------
public: // triggers
//------------------------------

  // Returns the number of triggers.
  // [main-thread]

  uint32_t triggers_count() override {
    SAT_Print("\n");
    return 0;
  }

  //----------

  // Copies the trigger's info to trigger_info and returns true on success.
  // Returns true on success.
  // [main-thread]

  /*
    struct clap_trigger_info {
      clap_id id;
      clap_trigger_info_flags flags;
      void *cookie;
      char name[CLAP_NAME_SIZE];
      char module[CLAP_PATH_SIZE];
    };
  */

  bool triggers_get_info(uint32_t index, clap_trigger_info_t *trigger_info) override {
    SAT_Print("\n");
    return false;
  }

//------------------------------
public: // tuning
//------------------------------

  // Called when a tuning is added or removed from the pool.
  // [main-thread]

  void tuning_changed() override {
    SAT_Print("\n");
  }

//------------------------------
public: // voice info
//------------------------------

  // gets the voice info, returns true on success
  // [main-thread && active]

  // voice_count is the current number of voices that the patch can use
  // voice_capacity is the number of voices allocated voices
  // voice_count should not be confused with the number of active voices.
  // 1 <= voice_count <= voice_capacity
  // For example, a synth can have a capacity of 8 voices, but be configured
  // to only use 4 voices: {count: 4, capacity: 8}.
  // If the voice_count is 1, then the synth is working in mono and the host
  // can decide to only use global modulation mapping.

  bool voice_info_get(clap_voice_info_t *info) override {
    SAT_Print("\n");
    //info->count     = 32;
    //info->capacity  = 32;
    //info->flags     = CLAP_VOICE_INFO_SUPPORTS_OVERLAPPING_NOTES;
    //return true;
    return false;
  }



//------------------------------------------------------------
//
//
//
//------------------------------------------------------------



//------------------------------
public: // extensions
//------------------------------

  void registerExtension(const char* id) {
    if (strcmp(id,CLAP_EXT_AMBISONIC) == 0)               MExtensions.ambisonic               = &MAmbisonicExt;
    if (strcmp(id,CLAP_EXT_AUDIO_PORTS_ACTIVATION) == 0)  MExtensions.audio_ports_activation  = &MAudioPortsActivationExt;
    if (strcmp(id,CLAP_EXT_AUDIO_PORTS_CONFIG) == 0)      MExtensions.audio_ports_config      = &MAudioPortsConfigExt;
    if (strcmp(id,CLAP_EXT_AUDIO_PORTS) == 0)             MExtensions.audio_ports             = &MAudioPortsExt;
    if (strcmp(id,CLAP_EXT_CHECK_FOR_UPDATE) == 0)        MExtensions.check_for_update        = &MCheckForUpdateExt;
    if (strcmp(id,CLAP_EXT_CONTEXT_MENU) == 0)            MExtensions.context_menu            = &MContextMenuExt;
    if (strcmp(id,CLAP_EXT_CV) == 0)                      MExtensions.cv                      = &MCVExt;
    if (strcmp(id,CLAP_EXT_GUI) == 0)                     MExtensions.gui                     = &MGuiExt;
    if (strcmp(id,CLAP_EXT_LATENCY) == 0)                 MExtensions.latency                 = &MLatencyExt;
    if (strcmp(id,CLAP_EXT_MIDI_MAPPINGS) == 0)           MExtensions.midi_mappings           = &MMidiMappingsExt;
    if (strcmp(id,CLAP_EXT_NOTE_NAME) == 0)               MExtensions.note_name               = &MNoteNameExt;
    if (strcmp(id,CLAP_EXT_NOTE_PORTS) == 0)              MExtensions.note_ports              = &MNotePortsExt;
    if (strcmp(id,CLAP_EXT_PARAM_INDICATION) == 0)        MExtensions.param_indication        = &MParamIndicationExt;
    if (strcmp(id,CLAP_EXT_PARAMS) == 0)                  MExtensions.params                  = &MParamsExt;
    if (strcmp(id,CLAP_EXT_POSIX_FD_SUPPORT) == 0)        MExtensions.posix_fd_support        = &MPosixFdSupportExt;
    if (strcmp(id,CLAP_EXT_PRESET_LOAD) == 0)             MExtensions.preset_load             = &MPresetLoadExt;
    if (strcmp(id,CLAP_EXT_REMOTE_CONTROLS) == 0)         MExtensions.remote_controls         = &MRemoteControlsExt;
    if (strcmp(id,CLAP_EXT_RENDER) == 0)                  MExtensions.render                  = &MRenderExt;
    if (strcmp(id,CLAP_EXT_RESOURCE_DIRECTORY) == 0)      MExtensions.resource_directory      = &MResourceDirectoryExt;
    if (strcmp(id,CLAP_EXT_STATE) == 0)                   MExtensions.state                   = &MStateExt;
    if (strcmp(id,CLAP_EXT_STATE_CONTEXT) == 0)           MExtensions.state_context           = &MStateContextExt;
    if (strcmp(id,CLAP_EXT_SURROUND) == 0)                MExtensions.surround                = &MSurroundExt;
    if (strcmp(id,CLAP_EXT_TAIL) == 0)                    MExtensions.tail                    = &MTailExt;
    if (strcmp(id,CLAP_EXT_THREAD_POOL) == 0)             MExtensions.thread_pool             = &MThreadPoolExt;
    if (strcmp(id,CLAP_EXT_TIMER_SUPPORT) == 0)           MExtensions.timer_support           = &MTimerSupportExt;
    if (strcmp(id,CLAP_EXT_TRACK_INFO) == 0)              MExtensions.track_info              = &MTrackInfoExt;
    if (strcmp(id,CLAP_EXT_TRIGGERS) == 0)                MExtensions.triggers                = &MTriggersExt;
    if (strcmp(id,CLAP_EXT_TUNING) == 0)                  MExtensions.tuning                  = &MTuningExt;
    if (strcmp(id,CLAP_EXT_VOICE_INFO) == 0)              MExtensions.voice_info              = &MVoiceInfoExt;
  }

  //----------

  void registerAllExtensions() {
    registerExtension(CLAP_EXT_AMBISONIC);
    registerExtension(CLAP_EXT_AUDIO_PORTS_ACTIVATION);
    registerExtension(CLAP_EXT_AUDIO_PORTS_CONFIG);
    registerExtension(CLAP_EXT_AUDIO_PORTS);
    registerExtension(CLAP_EXT_CHECK_FOR_UPDATE);
    registerExtension(CLAP_EXT_CONTEXT_MENU);
    registerExtension(CLAP_EXT_CV);
    registerExtension(CLAP_EXT_GUI);
    registerExtension(CLAP_EXT_LATENCY);
    registerExtension(CLAP_EXT_MIDI_MAPPINGS);
    registerExtension(CLAP_EXT_NOTE_NAME);
    registerExtension(CLAP_EXT_NOTE_PORTS);
    registerExtension(CLAP_EXT_PARAM_INDICATION);
    registerExtension(CLAP_EXT_PARAMS);
    registerExtension(CLAP_EXT_POSIX_FD_SUPPORT);
    registerExtension(CLAP_EXT_PRESET_LOAD);
    registerExtension(CLAP_EXT_REMOTE_CONTROLS);
    registerExtension(CLAP_EXT_RENDER);
    registerExtension(CLAP_EXT_RESOURCE_DIRECTORY);
    registerExtension(CLAP_EXT_STATE);
    registerExtension(CLAP_EXT_STATE_CONTEXT);
    registerExtension(CLAP_EXT_SURROUND);
    registerExtension(CLAP_EXT_TAIL);
    registerExtension(CLAP_EXT_THREAD_POOL);
    registerExtension(CLAP_EXT_TIMER_SUPPORT);
    registerExtension(CLAP_EXT_TRACK_INFO);
    registerExtension(CLAP_EXT_TRIGGERS);
    registerExtension(CLAP_EXT_TUNING);
    registerExtension(CLAP_EXT_VOICE_INFO);
  }

  //----------

  void logHostExtensions(const clap_host_t* host) {
    if (MHost->ext.ambisonic)           SAT_Log("  host supports draft ext %s\n", CLAP_EXT_AMBISONIC);
    if (MHost->ext.audio_ports_config)  SAT_Log("  host supports ext %s\n",       CLAP_EXT_AUDIO_PORTS_CONFIG);
    if (MHost->ext.audio_ports)         SAT_Log("  host supports ext %s\n",       CLAP_EXT_AUDIO_PORTS);
    if (MHost->ext.check_for_update)    SAT_Log("  host supports draft ext %s\n", CLAP_EXT_CHECK_FOR_UPDATE);
    if (MHost->ext.context_menu)        SAT_Log("  host supports draft ext %s\n", CLAP_EXT_CONTEXT_MENU);
    if (MHost->ext.cv)                  SAT_Log("  host supports draft ext %s\n", CLAP_EXT_CV);
    if (MHost->ext.event_registry)      SAT_Log("  host supports ext %s\n",       CLAP_EXT_EVENT_REGISTRY);
    if (MHost->ext.gui)                 SAT_Log("  host supports ext %s\n",       CLAP_EXT_GUI);
    if (MHost->ext.latency)             SAT_Log("  host supports ext %s\n",       CLAP_EXT_LATENCY);
    if (MHost->ext.log)                 SAT_Log("  host supports ext %s\n",       CLAP_EXT_LOG);
    if (MHost->ext.midi_mappings)       SAT_Log("  host supports draft ext %s\n", CLAP_EXT_MIDI_MAPPINGS);
    if (MHost->ext.note_name)           SAT_Log("  host supports ext %s\n",       CLAP_EXT_NOTE_NAME);
    if (MHost->ext.note_ports)          SAT_Log("  host supports ext %s\n",       CLAP_EXT_NOTE_PORTS);
    if (MHost->ext.params)              SAT_Log("  host supports ext %s\n",       CLAP_EXT_PARAMS);
    if (MHost->ext.posix_fd_support)    SAT_Log("  host supports ext %s\n",       CLAP_EXT_POSIX_FD_SUPPORT);
    if (MHost->ext.preset_load)         SAT_Log("  host supports draft ext %s\n", CLAP_EXT_PRESET_LOAD);
    if (MHost->ext.remote_controls)     SAT_Log("  host supports draft ext %s\n", CLAP_EXT_REMOTE_CONTROLS);
    if (MHost->ext.resource_directory)  SAT_Log("  host supports draft ext %s\n", CLAP_EXT_RESOURCE_DIRECTORY);
    if (MHost->ext.state)               SAT_Log("  host supports ext %s\n",       CLAP_EXT_STATE);
    if (MHost->ext.surround)            SAT_Log("  host supports draft ext %s\n", CLAP_EXT_SURROUND);
    if (MHost->ext.tail)                SAT_Log("  host supports ext %s\n",       CLAP_EXT_TAIL);
    if (MHost->ext.thread_check)        SAT_Log("  host supports ext %s\n",       CLAP_EXT_THREAD_CHECK);
    if (MHost->ext.thread_pool)         SAT_Log("  host supports ext %s\n",       CLAP_EXT_THREAD_POOL);
    if (MHost->ext.timer_support)       SAT_Log("  host supports ext %s\n",       CLAP_EXT_TIMER_SUPPORT);
    if (MHost->ext.track_info)          SAT_Log("  host supports draft ext %s\n", CLAP_EXT_TRACK_INFO);
    if (MHost->ext.transport_control)   SAT_Log("  host supports draft ext %s\n", CLAP_EXT_TRANSPORT_CONTROL);
    if (MHost->ext.triggers)            SAT_Log("  host supports draft ext %s\n", CLAP_EXT_TRIGGERS);
    if (MHost->ext.tuning)              SAT_Log("  host supports draft ext %s\n", CLAP_EXT_TUNING);
  }

//------------------------------
public: // parameters
//------------------------------

  void appendParameter(SAT_Parameter* AParameter) {
    uint32_t index = MParameters.size();
    AParameter->setIndex(index);
    MParameters.append(AParameter);
  }

  //----------

  void deleteParameters() {
    for (uint32_t i=0; i<MParameters.size(); i++) {
      if (MParameters[i]) {
        delete MParameters[i];
        MParameters[i] = nullptr;
      }
    }
  }

  //----------

  uint32_t getNumParameters() {
    return MParameters.size();
  }

  //----------

  SAT_Parameter* getParameter(uint32_t AIndex) {
    return MParameters[AIndex];
  }

  //----------

  void setDefaultParameterValues() {
    //for (uint32_t index=0; index<MParameters.size(); index++) {
    //  SAT_Parameter* parameter = MParameters[index];
    //  double value = parameter->getValue();
    //  queueProcessParamIndex(i);
    //  queueProcessParamValue(
    //}
  }

  //----------

  void updateProcessParameterValues() {
  }

  //----------

  void updateEditorParameterValues() {
  }

//------------------------------
public: // audio input ports
//------------------------------

  void appendAudioInputPort(SAT_AudioPort* APort) {
    uint32_t index = MAudioInputPorts.size();
    MAudioInputPorts.append(APort);
  }

  //----------

  void deleteAudioInputPorts() {
    for (uint32_t i=0; i<MAudioInputPorts.size(); i++) {
      if (MAudioInputPorts[i]) {
        delete MAudioInputPorts[i];
        MAudioInputPorts[i] = nullptr;
      }
    }
  }

  //----------

  uint32_t getNumAudioInputPorts() {
    return MAudioInputPorts.size();
  }

  //----------

  SAT_AudioPort* getAudioInputPort(uint32_t AIndex) {
    return MAudioInputPorts[AIndex];
  }

//------------------------------
public: // audio output ports
//------------------------------

  void appendAudioOutputPort(SAT_AudioPort* APort) {
    uint32_t index = MAudioOutputPorts.size();
    MAudioOutputPorts.append(APort);
  }

  //----------

  void deleteAudioOutputPorts() {
    for (uint32_t i=0; i<MAudioOutputPorts.size(); i++) {
      if (MAudioOutputPorts[i]) {
        delete MAudioOutputPorts[i];
        MAudioOutputPorts[i] = nullptr;
      }
    }
  }

  //----------

  uint32_t getNumAudioOutputPorts() {
    return MAudioOutputPorts.size();
  }

  //----------

  SAT_AudioPort* getAudioOutputPort(uint32_t AIndex) {
    return MAudioOutputPorts[AIndex];
  }

//------------------------------
public: // note input ports
//------------------------------

  void appendNoteInputPort(SAT_NotePort* APort) {
    uint32_t index = MNoteInputPorts.size();
    MNoteInputPorts.append(APort);
  }

  //----------

  void deleteNoteInputPorts() {
    for (uint32_t i=0; i<MNoteInputPorts.size(); i++) {
      if (MNoteInputPorts[i]) {
        delete MNoteInputPorts[i];
        MNoteInputPorts[i] = nullptr;
      }
    }
  }

  //----------

  uint32_t getNumNoteInputPorts() {
    return MNoteInputPorts.size();
  }

  //----------

  SAT_NotePort* getNoteInputPort(uint32_t AIndex) {
    return MNoteInputPorts[AIndex];
  }

//------------------------------
public: // note output ports
//------------------------------

  void appendNoteOutputPort(SAT_NotePort* APort) {
    uint32_t index = MNoteOutputPorts.size();
    MNoteOutputPorts.append(APort);
  }

  //----------

  void deleteNoteOutputPorts() {
    for (uint32_t i=0; i<MNoteOutputPorts.size(); i++) {
      if (MNoteOutputPorts[i]) {
        delete MNoteOutputPorts[i];
        MNoteOutputPorts[i] = nullptr;
      }
    }
  }

  //----------

  uint32_t getNumNoteOutputPorts() {
    return MNoteOutputPorts.size();
  }

  //----------

  SAT_NotePort* getNoteOutputPort(uint32_t AIndex) {
    return MNoteOutputPorts[AIndex];
  }

};

//----------------------------------------------------------------------
#endif
