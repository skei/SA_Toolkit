#ifndef sat_plugin_included
#define sat_plugin_included
//----------------------------------------------------------------------

// aka SAT_PluginImplementation

#include "base/sat.h"

#include "plugin/sat_audio_port.h"
#include "plugin/sat_editor.h"
#include "plugin/sat_editor_listener.h"
#include "plugin/sat_host.h"
#include "plugin/sat_note.h"
#include "plugin/sat_note_port.h"
#include "plugin/sat_parameter.h"
#include "plugin/sat_process_context.h"

#include "plugin/clap/sat_clap_plugin.h"
#include "plugin/clap/sat_clap_utils.h"

#include "audio/sat_audio_utils.h"
#include "gui/sat_widgets.h"

//----------------------------------------------------------------------

struct SAT_QueueItem {
  uint32_t    type;
  uint32_t    index;
  union {
    double    value;
    SAT_Note  note;
  };
};

typedef SAT_LockFreeQueue<SAT_QueueItem,SAT_PLUGIN_MAX_PARAM_EVENTS_PER_BLOCK>  SAT_ModFromHostToGuiQueue;
typedef SAT_LockFreeQueue<SAT_QueueItem,SAT_PLUGIN_MAX_PARAM_EVENTS_PER_BLOCK>  SAT_ParamFromHostToGuiQueue;
typedef SAT_LockFreeQueue<SAT_QueueItem,SAT_PLUGIN_MAX_GUI_EVENTS_PER_BLOCK>    SAT_ParamFromGuiToAudioQueue;
typedef SAT_LockFreeQueue<SAT_QueueItem,SAT_PLUGIN_MAX_GUI_EVENTS_PER_BLOCK>    SAT_ParamFromGuiToHostQueue;

#define SAT_PLUGIN_DEFAULT_CONSTRUCTOR(PLUGIN)                                  \
  PLUGIN(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) \
  : SAT_Plugin(ADescriptor,AHost) {                                             \
  }

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

  const char*                       MPluginFormat                                 = "CLAP";

  SAT_Host*                         MHost                                         = nullptr;
  SAT_Editor*                       MEditor                                       = nullptr;
  SAT_Dictionary<const void*>       MExtensions                                   = {};
  SAT_ParameterArray                MParameters                                   = {};
  SAT_AudioPortArray                MAudioInputPorts                              = {};
  SAT_AudioPortArray                MAudioOutputPorts                             = {};
  SAT_NotePortArray                 MNoteInputPorts                               = {};
  SAT_NotePortArray                 MNoteOutputPorts                              = {};
  uint32_t                          MInitialEditorWidth                           = 0;//512;
  uint32_t                          MInitialEditorHeight                          = 0;//512;
  double                            MInitialEditorScale                           = 2.0;

  SAT_ProcessContext                MProcessContext                               = {};
  bool                              MIsInitialized                                = false;
  bool                              MIsActivated                                  = false;
  bool                              MIsProcessing                                 = false;
  clap_id                           MSelectedAudioPortsConfig                     = 0;
  clap_plugin_render_mode           MRenderMode                                   = CLAP_RENDER_REALTIME;

  SAT_ParamFromHostToGuiQueue       MParamFromHostToGuiQueue                      = {};   // when the host changes a parameter, we need to redraw it
  SAT_ModFromHostToGuiQueue         MModFromHostToGuiQueue                        = {};   // --"-- modulation
  SAT_ParamFromGuiToAudioQueue      MParamFromGuiToAudioQueue                     = {};   // twweak knob, send parameter value to audio process
  SAT_ParamFromGuiToHostQueue       MParamFromGuiToHostQueue                      = {};   // tell host about parameter change

  bool                              MProcessThreaded                              = false; // not used here?
  uint32_t                          MEventMode                                    = SAT_PLUGIN_EVENT_MODE_BLOCK;

//------------------------------
public:
//------------------------------

  SAT_Plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_ClapPlugin(ADescriptor) {
    //SAT_PRINT;
    MHost = new SAT_Host(AHost);
    SAT_LogClapHostInfo(MHost);
  }

  //----------

  virtual ~SAT_Plugin() {
    SAT_Log("~SAT_Plugin()\n");
    //SAT_PRINT;
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

  SAT_Editor*         getEditor()         { return MEditor; }
  SAT_Host*           getHost()           { return MHost; }
  SAT_ProcessContext* getProcessContext() { return &MProcessContext; }

  const char*         getPluginFormat()   { return MPluginFormat; }

  //----------

  void setPluginFormat(const char* AFormat)     { MPluginFormat = AFormat; }

  void setProcessThreaded(bool AThreaded=true)  { MProcessThreaded = AThreaded; }
  void setEventMode(uint32_t AMode)             { MEventMode = AMode; }

  void setInitialEditorSize(uint32_t AWidth, uint32_t AHeight, double AScale=1.0) {
    MInitialEditorWidth = AWidth;
    MInitialEditorHeight = AHeight;
    MInitialEditorScale = AScale;
  }

//------------------------------
public: // plugin
//------------------------------

  // Must be called after creating the plugin.
  // If init returns false, the host must destroy the plugin instance.
  // If init returns true, then the plugin is initialized and in the deactivated state.
  // [main-thread]

  bool init() override {
    SAT_Log("SAT_Plugin.init\n");
    //uint32_t num = MParameters.size();
    //uint32_t size = num * sizeof(sat_param_t);
    setDefaultParameterValues();
    MIsInitialized = true;
    return true;
  }

  //----------

  // Free the plugin and its resources.
  // It is required to deactivate the plugin prior to this call.
  // [main-thread & !active]
  
  /*
    https://isocpp.org/wiki/faq/freestore-mgmt#delete-this  
    "As long as you’re careful, it’s okay (not evil) for an object to commit suicide (delete this)"
    
    what if we have a destroy() in our own plugin, that calls this one (SAT_Plugin::destroy),
    and then called 'delete this' there
    and we could set that up as the default destructor (macro, like we do for the constructor)
    (would it change anything?)
  */

  void destroy() override {
    SAT_Log("SAT_Plugin.destroy\n");
    MIsInitialized = false;
    
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
    MProcessContext.counter = 0;
    //TODO
  }

  //----------

  // process audio, events, ...
  // All the pointers coming from clap_process_t and its nested attributes,
  // are valid until process() returns.
  // [audio-thread & active_state & processing_state]

  clap_process_status process(const clap_process_t* process) override {
    MProcessContext.process = process;
    flushParamFromGuiToAudio();
    if (process->transport) handleTransportEvent(process->transport);
    //preProcessEvents(process->in_events,process->out_events);
    switch (MEventMode) {
      case SAT_PLUGIN_EVENT_MODE_BLOCK: {
        processEvents(process->in_events,process->out_events);
        processAudio(&MProcessContext);
        break;
      }
      case SAT_PLUGIN_EVENT_MODE_INTERLEAVED: {
        SAT_PRINT;
        processInterleaved(&MProcessContext);
        break;
      }
      case SAT_PLUGIN_EVENT_MODE_QUANTIZED: {
        processQuantized(&MProcessContext);
        break;
      }
    }
    postProcessEvents(process->in_events,process->out_events);
    flushParamFromGuiToHost(process->out_events);
    MProcessContext.counter += 1;
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
    //SAT_Print("id %s -> %p\n",id,MExtensions.getItem(id));
    return MExtensions.getItem(id);
  }

  //----------

  // Called by the host on the main thread in response to a previous call to:
  //   host->request_callback(host);
  // [main-thread]

  void on_main_thread() override {
    //SAT_Print("\n");
  }

//------------------------------
public: // host
//------------------------------

  // host.get_extension()
  // host.request_restart()
  // host.request_process()
  // host.request_callback()

//------------------------------
public: // ambisonic (draft)
//------------------------------

  // host.ambisonic_changed()
  
  //----------

  // Returns true on success
  // config_id: the configuration id, see clap_plugin_audio_ports_config.
  // If config_id is CLAP_INVALID_ID, then this function queries the current port info.
  // [main-thread]

  bool ambisonic_get_info(bool is_input,  uint32_t port_index, clap_ambisonic_info_t *info) override {
    SAT_Print("is_input %i port_index %i\n",is_input,port_index);
    return false;
  }

//------------------------------
public: // audio ports
//------------------------------

  // host.audio_ports_is_rescan_flag_supported()
  // host.audio_ports_rescan()
  
  //----------

  // number of ports, for either input or output
  // [main-thread]

  uint32_t audio_ports_count(bool is_input) override {
    //SAT_Print("is_input %i\n",is_input);
    if (is_input) return MAudioInputPorts.size();
    else return MAudioOutputPorts.size();
  }

  //----------

  // get info about about an audio port.
  // [main-thread]

  bool audio_ports_get(uint32_t index, bool is_input, clap_audio_port_info_t *info) override {
    //SAT_Print("index %i is_input %i\n",index,is_input);
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
public: // audio ports activation (draft)
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
    SAT_Print("is_input %i port_index %i is_active %i\n",is_input,port_index,is_active);
    return false;
  }

//------------------------------
public: // audio ports config
//------------------------------

  // host.audio_ports_config_rescan()
  
  //----------

  // gets the number of available configurations
  // [main-thread]

  uint32_t audio_ports_config_count() override {
    SAT_Print("\n");
    return 1;
  }

  //----------

  // gets information about a configuration
  // [main-thread]

  //TODO: fix this..
  // uses first input/output audio port

  bool audio_ports_config_get(uint32_t index, clap_audio_ports_config_t *config) override {
    SAT_Print("index %i\n",index);
    config->id = 0;
    strcpy(config->name,"apconfig1");
    if (MAudioInputPorts.size() > 0) {
      clap_audio_port_info_t* info      = MAudioInputPorts[0]->getInfo();
      config->input_port_count          = 1;
      config->has_main_input            = true;
      config->main_input_channel_count  = info->channel_count;
      config->main_input_port_type      = info->port_type;
    }
    else {
      config->input_port_count          = 0;
      config->has_main_input            = false;
      config->main_input_channel_count  = 0;
      config->main_input_port_type      = "";
    }
    if (MAudioOutputPorts.size() > 0) {
      clap_audio_port_info_t* info      = MAudioOutputPorts[0]->getInfo();
      config->output_port_count          = 1;
      config->has_main_output            = true;
      config->main_output_channel_count  = info->channel_count;
      config->main_output_port_type      = info->port_type;
    }
    else {
      config->output_port_count         = 0;
      config->has_main_output           = false;
      config->main_output_channel_count = 0;
      config->main_output_port_type     = "";
    }
    return true;
  }

  //----------

  // selects the configuration designated by id
  // returns true if the configuration could be applied.
  // Once applied the host should scan again the audio ports.
  // [main-thread,plugin-deactivated]

  bool audio_ports_config_select(clap_id config_id) override {
    SAT_Print("config_id %i\n",config_id);
    MSelectedAudioPortsConfig = config_id;
    return true;
  }

//------------------------------
public: // check for updates
//------------------------------

  // host.check_for_update_on_new_version()
  
  //----------

  // [main-thread]

  void check_for_updates_check(bool include_preview) override {
    SAT_Print("\n");
  }

//------------------------------
public: // configurable audio ports
//------------------------------

  bool configurable_audio_ports_is_port_configurable(bool is_input, uint32_t port_index) override {
    SAT_Print("\n");
    return false;
  }
  
  //----------
  
  bool configurable_audio_ports_request_configuration(const struct clap_audio_port_configuration_request *requests,uint32_t request_count) override {
    SAT_Print("\n");
    return false;
  }

//------------------------------
public: // context menu
//------------------------------

  // host.context_menu_populate()
  // host.context_menu_perform()
  // host.context_menu_can_popup()
  // host.context_menu_popup()
  
  //----------
  
  
  
  


  // CLAP_CONTEXT_MENU_TARGET_KIND_GLOBAL = 0,
  // CLAP_CONTEXT_MENU_TARGET_KIND_PARAM = 1,

  // CLAP_CONTEXT_MENU_ITEM_ENTRY
  // CLAP_CONTEXT_MENU_ITEM_CHECK_ENTRY
  // CLAP_CONTEXT_MENU_ITEM_SEPARATOR
  // CLAP_CONTEXT_MENU_ITEM_BEGIN_SUBMENU
  // CLAP_CONTEXT_MENU_ITEM_END_SUBMENU
  // CLAP_CONTEXT_MENU_ITEM_TITLE

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
  
//  clap_context_menu_entry entry = {
//    "hello world!", // const char *label;
//    true,           // bool is_enabled;
//    0               //clap_id     action_id;
//  };

  bool context_menu_populate(const clap_context_menu_target_t *target, const clap_context_menu_builder_t *builder) override {
    SAT_Print("target kind %i id %i\n",target->kind,target->id);
    return true;
//    if (builder->supports(builder,CLAP_CONTEXT_MENU_ITEM_ENTRY)) {
//      builder->add_item(builder,CLAP_CONTEXT_MENU_ITEM_ENTRY,&entry);
//    }
//    return true;
  }

  //----------

  // Performs the given action, which was previously provided to the host via populate().
  // If target is null, assume global context.
  // [main-thread]

  bool context_menu_perform(const clap_context_menu_target_t *target, clap_id action_id) override {
    SAT_Print("target kind %i id %i action %i\n",target->kind,target->id,action_id);
    return true;
//    if (action_id == 0) {
//      SAT_Print("hello world!!!");
//    }
//    return true;
  }

//------------------------------
public: // cv
//------------------------------

  // host.cv_changed()

  //----------
  
  // Returns true on success.
  // [main-thread]

  bool cv_get_channel_type(bool is_input, uint32_t port_index, uint32_t channel_index, uint32_t *channel_type) override {
    SAT_Print("\n");
    return false;
  }

//------------------------------
public: // event registry
//------------------------------

  // host.event_registry_query()

//------------------------------
public: // extensible audio ports
//------------------------------

  bool extensible_audio_ports_add_port(bool is_input, uint32_t channel_count, const char *port_type, const void *port_details) override {
    SAT_Print("\n");
    return false;
  }
  
  //----------
  
  bool extensible_audio_ports_remove_port(bool is_input, uint32_t index) override {
    SAT_Print("\n");
    return false;
  }

//------------------------------
public: // gui
//------------------------------

  // host.gui_resize_hints_changed()
  // host.gui_request_resize()
  // host.gui_request_show()
  // host.gui_request_hide()
  // host.gui_closed()

  //----------

  // Returns true if the requested gui api is supported
  // [main-thread]

  bool gui_is_api_supported(const char *api, bool is_floating) override {
    //SAT_Print("api %s is_floating %i\n",api,is_floating);
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
    //SAT_Print("\n");
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
  
  // see also initEditorWindow (default editor)
  
  bool gui_create(const char *api, bool is_floating) override {
    //SAT_Print("api %s is_floating %i\n",api,is_floating);
    if (is_floating == true) return false;

    #ifdef SAT_LINUX
      if (strcmp(api,CLAP_WINDOW_API_X11) != 0) return false;
    #endif
    #ifdef SAT_WIN32
      if (strcmp(api,CLAP_WINDOW_API_WIN32) != 0) return false;
    #endif
    
    // if we haven't set/called setInitialEditorSize, use calculated, generic editor size
    if ((MInitialEditorWidth <= 0) || (MInitialEditorHeight <= 0)) {
      uint32_t num = MParameters.size();
      
      double w =  300 +             // slider width
                  10 + 10;          // inner border (top/bottom)
      
      double h =  40 +              // header
                  20 +              // footer
                  10 + 10 +         // inner border
                  (num * 20) +      // sliders
                  ((num - 1) * 5);  // spacing
                  
      double s =  2.0;
      
      setInitialEditorSize(w,h,s);
      
    }

    uint32_t w = (double)MInitialEditorWidth * MInitialEditorScale;
    uint32_t h = (double)MInitialEditorHeight * MInitialEditorScale;
    MEditor = createEditor(this,w,h);//MInitialEditorWidth,MInitialEditorHeight);
    if (!MEditor) {
      //SAT_Print("todo: createGenericEditor\n");
      //createGenericEditor(this,w,h);
    }
    return (MEditor);
  }

  //----------

  // Free all resources associated with the gui.
  // [main-thread]

  void gui_destroy() override {
    //SAT_Print("\n");
    MEditor->destroy();
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
    //SAT_Print("%f\n",scale);
    return MEditor->set_scale(scale);
  }

  //----------

  // Get the current size of the plugin UI.
  // clap_plugin_gui->create() must have been called prior to asking the size.
  // [main-thread]

  bool gui_get_size(uint32_t *width, uint32_t *height) override {
    //SAT_Print("\n");
    if (MEditor) {
      return MEditor->get_size(width,height);
    }
    else {
      *width = MInitialEditorWidth;
      *height = MInitialEditorHeight;
      return true;
    }
  }

  //----------

  // Returns true if the window is resizeable (mouse drag).
  // Only for embedded windows.
  // [main-thread]

  bool gui_can_resize() override {
    //SAT_Print("\n");
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
    //SAT_Print("\n");
    return MEditor->get_resize_hints(hints);
  }

  //----------

  // If the plugin gui is resizable, then the plugin will calculate the closest
  // usable size which fits in the given size.
  // This method does not change the size.
  // Only for embedded windows.
  // [main-thread]

  bool gui_adjust_size(uint32_t *width, uint32_t *height) override {
    //SAT_Print("\n");
    return MEditor->adjust_size(width,height);
  }

  //----------

  // Sets the window size. Only for embedded windows.
  // [main-thread]

  bool gui_set_size(uint32_t width, uint32_t height) override {
    //SAT_Print("\n");
    return MEditor->set_size(width,height);
  }

  //----------

  // Embbeds the plugin window into the given window.
  // [main-thread & !floating]

  /*
    - looks like bitwig checks a while (a second or two) afterwards,
      and destroyed the gui if it's not parented (yet)..
  */

  bool gui_set_parent(const clap_window_t *window) override {
    //SAT_Print("\n");
    bool result = MEditor->set_parent(window);
    if (result) {
      SAT_Window* win = MEditor->getWindow();
      win->setInitialSize(MInitialEditorWidth,MInitialEditorHeight);
      initEditorWindow(MEditor,win);
      initEditorParameterValues();
    }
    return result;
  }

  //----------

  // Set the plugin floating window to stay above the given window.
  // [main-thread & floating]

  bool gui_set_transient(const clap_window_t *window) override {
    //SAT_Print("\n");
    return MEditor->set_transient(window);
  }

  //----------

  // Suggests a window title. Only for floating windows.
  // [main-thread & floating]

  void gui_suggest_title(const char *title) override {
    //SAT_Print("\n");
    return MEditor->suggest_title(title);
  }

  //----------

  // Show the window.
  // [main-thread]

  bool gui_show() override {
    //SAT_Print("\n");
    return MEditor->show();
  }

  //----------

  // Hide the window, this method does not free the resources, it just hides
  // the window content. Yet it may be a good idea to stop painting timers.
  // [main-thread]

  bool gui_hide() override {
    //SAT_Print("\n");
    return MEditor->hide();
  }

//------------------------------
public: // latency
//------------------------------

  // host.latency_changed()
  
  //----------

  // Returns the plugin latency.
  // [main-thread]

  uint32_t latency_get() override {
    //SAT_Print("\n");
    return 0;
  }

//------------------------------
public: // log
//------------------------------

  // host.log_log()

//------------------------------
public: // midi mappings
//------------------------------

  // host.midi_mappings_changed()

  //----------
  
  // [main-thread]

  uint32_t midi_mappings_count() override {
    SAT_Print("\n");
    return 0;
  }

  //----------

  // [main-thread]

  bool midi_mappings_get(uint32_t index, clap_midi_mapping_t *mapping) override {
    SAT_Print("index %i\n",index);
    //mapping->channel
    //mapping->number
    //mapping->param_id
    return false;
  }

//------------------------------
public: // note name
//------------------------------

  // host.note_name_changed()
  
  //----------

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
    SAT_Print("index %i\n",index);
    //note_name->name
    //note_name->port
    //note_name->key
    //note_name->channel
    return false;
  }

//------------------------------
public: // note ports
//------------------------------

  // host.note_ports_supported_dialects()
  // host.note_ports_rescan()

  //
  
  // number of ports, for either input or output
  // [main-thread]

  uint32_t note_ports_count(bool is_input) override {
    //SAT_Print("is_input %i\n",is_input);
    if (is_input) return MNoteInputPorts.size();
    else return MNoteOutputPorts.size();
  }

  //----------

  // get info about about a note port.
  // [main-thread]

  bool note_ports_get(uint32_t index, bool is_input, clap_note_port_info_t* info) override {
    //SAT_Print("index %i is_input %i\n",index,is_input);
    const clap_note_port_info_t* port_info = nullptr;
    if (is_input) port_info = MNoteInputPorts[index]->getInfo();
    else port_info = MNoteOutputPorts[index]->getInfo();
    memcpy(info,port_info,sizeof(clap_note_port_info_t));
    return true;
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
    SAT_Print("param_id %i has_mapping %i color %i.%i.%i label %s desc %s\n",param_id,has_mapping,color->red,color->green,color->blue,label,description);
    SAT_Parameter* param = MParameters[param_id];
    param->setMappingIndication(has_mapping,color,label,description);
    SAT_Widget* widget = (SAT_Widget*)param->getConnection();
    if (widget && MEditor && MEditor->isOpen()) widget->parentRedraw();
  }

  //----------

  // Sets or clears an automation indication.
  // automation_state: current automation state for the given parameter
  // color: if set, the color to use to display the automation indication in the plugin GUI
  // Parameter indications should not be saved in the plugin context, and are off by default.
  // [main-thread]
  
  
  /*
  state:
  0 CLAP_PARAM_INDICATION_AUTOMATION_NONE     // The host doesn't have an automation for this parameter
  1 CLAP_PARAM_INDICATION_AUTOMATION_PRESENT  // The host has an automation for this parameter, but it isn't playing it
  2 CLAP_PARAM_INDICATION_AUTOMATION_PLAYING = 2 // // The host is playing an automation for this parameter
  3 CLAP_PARAM_INDICATION_AUTOMATION_RECORDING = 3, // // The host is recording an automation on this parameter
  4 CLAP_PARAM_INDICATION_AUTOMATION_OVERRIDING // The host should play an automation for this parameter, but the user has started to adjust this parameter and is overriding the automation playback
  */

  void param_indication_set_automation(clap_id param_id, uint32_t automation_state, const clap_color_t *color) override {
    SAT_Print("param_id %i automation_state %i color %i.%i.%i\n",param_id,automation_state,color->red,color->green,color->blue);
    SAT_Parameter* param = MParameters[param_id];
    param->setAutomationIndication(automation_state,color);
    SAT_Widget* widget = (SAT_Widget*)param->getConnection();
    if (widget && MEditor && MEditor->isOpen()) widget->parentRedraw();
  }

//------------------------------
public: // params
//------------------------------

  // host.params_rescan()
  // host.params_clear()
  // host.params_request_flush()

  //----------
  
  // Returns the number of parameters.
  // [main-thread]

  uint32_t params_count() override {
    //SAT_Print("\n");
    return MParameters.size();
  }

  //----------

  // Copies the parameter's info to param_info. Returns true on success.
  // [main-thread]

  bool params_get_info(uint32_t param_index, clap_param_info_t* param_info) override {
    //SAT_Print("param_index %i\n",param_index);
    SAT_Parameter* parameter = MParameters[param_index];
    clap_param_info_t* info = parameter->getParamInfo();
    memcpy(param_info,info,sizeof(clap_param_info_t));
    return true;
  }

  //----------

  // Writes the parameter's current value to out_value. Returns true on success.
  // [main-thread]

  bool params_get_value(clap_id param_id, double *out_value) override {
    //SAT_Print("param_id %i\n",param_id);
    double value = MParameters[param_id]->getValue();
    //double value = MParameterValues[param_id];
    *out_value = value;
    return true;
  }

  //----------

  // Fills out_buffer with a null-terminated UTF-8 string that represents the parameter at the
  // given 'value' argument. eg: "2.3 kHz". Returns true on success. The host should always use
  // this to format parameter values before displaying it to the user. [main-thread]

  bool params_value_to_text(clap_id param_id, double value, char* out_buffer, uint32_t out_buffer_capacity) override {
    //SAT_Print("param_id %i value %f\n",param_id,value);
    uint32_t index = param_id;
    SAT_Parameter* parameter = MParameters[index];
    return parameter->valueToText(value,out_buffer,out_buffer_capacity);
  }

  //----------

  // Converts the null-terminated UTF-8 param_value_text into a double and writes it to out_value.
  // Returns true on success. The host can use this to convert user input into a parameter value.
  // [main-thread]

  bool params_text_to_value(clap_id param_id, const char* param_value_text, double* out_value) override {
    //SAT_Print("param_id %i text %s\n",param_id,param_value_text);
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
    processEvents(in,out);
    //flushParamFromGuiToHost();
    //flushNoteEndFromProcessToHost();
  }

//------------------------------
public: // posix fd support
//------------------------------

  // host.posix_fd_support_register_fd()
  // host.posix_fd_support_modify_fd()
  // host.posix_fd_support_unregister_fd()

  //----------

  // This callback is "level-triggered".
  // It means that a writable fd will continuously produce "on_fd()" events;
  // don't forget using modify_fd() to remove the write notification once you're
  // done writting.
  // [main-thread]

  void posix_fd_support_on_fd(int fd, clap_posix_fd_flags_t flags) override {
    SAT_Print("fd %i flags %i\n",fd,flags);
  }

//------------------------------
public: // preset load
//------------------------------

  // host.preset_load_on_error()
  // host.preset_load_loaded()

  //----------

  // Loads a preset in the plugin native preset file format from a location.
  // The preset discovery provider defines the location and load_key to be passed to this function.
  // [main-thread]

  bool preset_load_from_location(uint32_t location_kind, const char *location, const char *load_key) override {
    SAT_PRINT;
    switch (location_kind) {
      case CLAP_PRESET_DISCOVERY_LOCATION_FILE: {
        SAT_Print("CLAP_PRESET_DISCOVERY_LOCATION_FILE location '%s', load_key '%s'\n",location,load_key);
        MHost->preset_load_loaded(location_kind,location,load_key);
        return true;
      }
      case CLAP_PRESET_DISCOVERY_LOCATION_PLUGIN: {
        SAT_Print("CLAP_PRESET_DISCOVERY_LOCATION_PLUGIN location '%s', load_key '%s'\n",location,load_key);
        MHost->preset_load_loaded(location_kind,location,load_key);
        return true;
      }
    }
    return false;
  }

//------------------------------
public: // remote controls
//------------------------------

  // host.remote_controls_changed()
  // host.remote_controls_suggest_page()

  //----------
  
  // Returns the number of pages.
  // [main-thread]

  uint32_t remote_controls_count() override {
    SAT_Print("-> 1\n");
    return 1;
  }

  //----------

  // Get a page by index.
  // [main-thread]

  bool remote_controls_get(uint32_t page_index, clap_remote_controls_page_t *page) override {
    SAT_Print("-> true\n");
    switch (page_index) {
      case 0: {
        page->page_id = 0;
        strcpy(page->page_name,"Page 1");
        strcpy(page->section_name,"Section 1");
        page->param_ids[0] = 0;
        page->param_ids[1] = 1;
        page->param_ids[2] = 2;
        page->param_ids[3] = 3;
        page->param_ids[4] = 0;
        page->param_ids[5] = 1;
        page->param_ids[6] = 2;
        page->param_ids[7] = 3;
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
    return true;
  }

//------------------------------
public: // resource directory
//------------------------------

  // host.resource_directory_request_directory()
  // host.resource_directory_release_directory()

  //----------
  
  // Sets the directory in which the plugin can save its resources.
  // The directory remains valid until it is overriden or the plugin is destroyed.
  // If path is null or blank, it clears the directory location.
  // path must be absolute.
  // [main-thread]

  void resource_directory_set_directory(const char *path, bool is_shared) override {
    SAT_Print("path %s is_shared %i\n",path,is_shared);
  }

  //----------

  // Asks the plugin to put its resources into the resources directory.
  // It is not necessary to collect files which belongs to the plugin's
  // factory content unless the param all is true.
  // [main-thread]

  void resource_directory_collect(bool all) override {
    SAT_Print("all %i\n",all);
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
    SAT_Print("index %i\n");
    return -1;
  }

//------------------------------
public: // state
//------------------------------

  /*
    the host might work in 'chunks', so check return values to see if it actually saved
    everything you gave it.. spin a loop to be sure everything is written..
    same thing with reading..
    (maybe the host loads/saves 4k/64k chunks from disk or whatever)
  */

  //----------

  // host.state_mark_dirty()
  
  //----------

  // Saves the plugin state into stream.
  // Returns true if the state was correctly saved.
  // [main-thread]

  bool state_save(const clap_ostream_t *stream) override {
    SAT_PRINT;
    uint32_t total = 0;
    uint32_t written = 0;
    uint32_t version = 0;
    uint32_t num_params = MParameters.size();
    // version
    written = stream->write(stream,&version,sizeof(uint32_t));
    if (written != sizeof(uint32_t)) {
      //SAT_Print("state_save: error writing version\n");
      return false;
    }
    total += sizeof(uint32_t);
    // num params
    written = stream->write(stream,&num_params,sizeof(uint32_t));
    if (written != sizeof(uint32_t)) {
      //SAT_Print("state_save: error writing parameter count\n");
      return false;
    }
    total += sizeof(uint32_t);
    // param values
    for (uint32_t i=0; i<num_params; i++) {
      double value = MParameters[i]->getValue();
      //double value = MParameterValues[i];
      written = stream->write(stream,&value,sizeof(double));
      if (written != sizeof(double)) {
        //SAT_Print("state_load: error writing parameter %i\n",i);
        return false;
      }
      total += sizeof(double);
    }
    //SAT_Print("total: %i\n",total);
    return true;
  }

  //----------

  // Loads the plugin state from stream.
  // Returns true if the state was correctly restored.
  // [main-thread]

  bool state_load(const clap_istream_t *stream) override {
    SAT_PRINT;
    uint32_t total = 0;
    uint32_t read = 0;
    uint32_t version = 0;
    uint32_t num_params = 0;
    // version
    read = stream->read(stream,&version,sizeof(uint32_t));
    if (read != sizeof(uint32_t)) {
      //SAT_Print("state_load: error reading version\n");
      return false;
    }
    total += sizeof(uint32_t);
    //TODO: check version
    // num params
    read = stream->read(stream,&num_params,sizeof(uint32_t));
    if (read != sizeof(uint32_t)) {
      //SAT_Print("state_load: error reading parameter count\n");
      return false;
    }
    total += sizeof(uint32_t);
    //TODO: check num params = marameters.size
    if (num_params != MParameters.size()) {
      //SAT_Print("state_load: wrong parameter count\n");
      return false;
    }
    // param values
    for (uint32_t i=0; i<num_params; i++) {
      double value = 0.0;
      read = stream->read(stream,&value,sizeof(double));
      if (read != sizeof(double)) {
        //SAT_Print("state_load: error reading parameter %i\n",i);
        return false;
      }
      total += sizeof(double);
      MParameters[i]->setValue(value);
      //MParameterValues[i] = value;
    }

    //SAT_Print("total: %i\n",total);
    //updateParameterValues();
    //updateEditorParameterValues();

    return true;
  }

//------------------------------
public: // state context (draft)
//------------------------------

  /*
    This extension lets the host save and load the plugin state with different semantics depending
    on the context.
    Briefly, when loading a preset or duplicating a device, the plugin may want to partially load
    the state and initialize certain things differently.
    Save and Load operations may have a different context.
    All three operations should be equivalent:
    1. clap_plugin_state_context.load(clap_plugin_state.save(), CLAP_STATE_CONTEXT_FOR_PRESET)
    2. clap_plugin_state.load(clap_plugin_state_context.save(CLAP_STATE_CONTEXT_FOR_PRESET))
    3. clap_plugin_state_context.load(clap_plugin_state_context.save(CLAP_STATE_CONTEXT_FOR_PRESET),CLAP_STATE_CONTEXT_FOR_PRESET)
    If the plugin implements CLAP_EXT_STATE_CONTEXT then it is mandatory to also implement
    CLAP_EXT_STATE.
  */

  //----------

  // Saves the plugin state into stream, according to context_type.
  // Returns true if the state was correctly saved.
  // Note that the result may be loaded by both clap_plugin_state.load() and
  // clap_plugin_state_context.load().
  // [main-thread]

  bool state_context_save(const clap_ostream_t *stream, uint32_t context_type) override {
    SAT_PRINT;
    switch (context_type) {
      case CLAP_STATE_CONTEXT_FOR_DUPLICATE:  return state_save(stream);
      case CLAP_STATE_CONTEXT_FOR_PRESET:     return state_save(stream);
    }
    return false;
  }

  //----------

  // Loads the plugin state from stream, according to context_type.
  // Returns true if the state was correctly restored.
  // Note that the state may have been saved by clap_plugin_state.save() or
  // clap_plugin_state_context.save() with a different context_type.
  // [main-thread]

  bool state_context_load(const clap_istream_t *stream, uint32_t context_type) override {
    SAT_PRINT;
    switch (context_type) {
      case CLAP_STATE_CONTEXT_FOR_DUPLICATE:  return state_load(stream);
      case CLAP_STATE_CONTEXT_FOR_PRESET:     return state_load(stream);
    }
    return false;
  }

//------------------------------
public: // surround
//------------------------------

  // host.surround_changed()
  // host.surround_get_preferred_channel_map()

  //----------
  
  // Stores into the channel_map array, the surround identifer of each channels.
  // Returns the number of elements stored in channel_map.
  // config_id: the configuration id, see clap_plugin_audio_ports_config.
  // If config_id is CLAP_INVALID_ID, then this function queries the current port info.
  // [main-thread]

  uint32_t surround_get_channel_map(bool is_input, uint32_t port_index, uint8_t *channel_map, uint32_t channel_map_capacity) override {
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

  // host.tail_changed()
  
  //----------

  // Returns tail length in samples.
  // Any value greater or equal to INT32_MAX implies infinite tail.
  // [main-thread,audio-thread]

  uint32_t tail_get() override {
    SAT_Print("-> 0\n");
    return 0;
  }

//------------------------------
public: // thread check
//------------------------------

  // host.thread_check_is_main_thread()
  // host.thread_check_is_audio_thread()

//------------------------------
public: // thread pool
//------------------------------

  // host.thread_pool_request_exec()
  
  //----------

  // Called by the thread pool

  void thread_pool_exec(uint32_t task_index) override {
    SAT_Print("\n");
  }

//------------------------------
public: // timer support
//------------------------------

  // host.timer_support_register_timer()
  // host.timer_support_unregister_timer()
  
  //----------

  // [main-thread]

  void timer_support_on_timer(clap_id timer_id) override {
    SAT_Print("timer id %i\n",timer_id);
  }

//------------------------------
public: // track info
//------------------------------

  // host.track_info_get()
  
  //----------

  // Called when the info changes.
  // [main-thread]

  void track_info_changed() override {
    SAT_Print("\n");
  }

//------------------------------
public: // transport control
//------------------------------

  // host.transport_control_request_start()
  // host.transport_control_request_stop()
  // host.transport_control_request_continue()
  // host.transport_control_request_pause()
  // host.transport_control_request_toggle_play()
  // host.transport_control_request_jump(clap_beattime position)
  // host.transport_control_request_loop_region(clap_beattime start, clap_beattime duration)
  // host.transport_control_request_toggle_loop()
  // host.transport_control_request_enable_loop(bool is_enabled)
  // host.transport_control_request_record(bool is_recording)
  // host.transport_control_request_toggle_record()

//------------------------------
public: // triggers
//------------------------------

  // host.triggers_rescan()
  // host.triggers_clear()

  //----------
  
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
    SAT_Print("index %i\n",index);
    return false;
  }

//------------------------------
public: // tuning
//------------------------------

  // host.tuning_get_relative()
  // host.tuning_should_play()
  // host.tuning_get_tuning_count()
  // host.tuning_get_info()
  
  //----------

  // Called when a tuning is added or removed from the pool.
  // [main-thread]

  void tuning_changed() override {
    SAT_Print("\n");
  }

//------------------------------
public: // voice info
//------------------------------

  // host.voice_info_changed()
  
  //----------

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
    info->voice_count     = 16;
    info->voice_capacity  = 16;
    info->flags           = CLAP_VOICE_INFO_SUPPORTS_OVERLAPPING_NOTES;
    return true;
  }



//------------------------------------------------------------
//
//
//
//------------------------------------------------------------



//------------------------------
public: // extensions
//------------------------------

  void registerExtension(const char* id, const void* ptr) {
    MExtensions.addItem(id,ptr);
  }

  //----------

  void registerDefaultExtensions() {
    registerExtension(CLAP_EXT_AUDIO_PORTS,               &MAudioPortsExt);
    #ifndef SAT_NO_GUI
      registerExtension(CLAP_EXT_GUI,                     &MGuiExt);
    #endif
    registerExtension(CLAP_EXT_NOTE_PORTS,                &MNotePortsExt);
    registerExtension(CLAP_EXT_PARAMS,                    &MParamsExt);
    registerExtension(CLAP_EXT_STATE,                     &MStateExt);
  }

  //----------

  void registerDefaultSynthExtensions() {
    registerDefaultExtensions();
    registerExtension(CLAP_EXT_THREAD_POOL,               &MThreadPoolExt);
    registerExtension(CLAP_EXT_VOICE_INFO,                &MVoiceInfoExt);
  }

  //----------

  void registerAllExtensions() {
    registerExtension(CLAP_EXT_AMBISONIC,                 &MAmbisonicExt);
    registerExtension(CLAP_EXT_AUDIO_PORTS_ACTIVATION,    &MAudioPortsActivationExt);
    registerExtension(CLAP_EXT_AUDIO_PORTS_CONFIG,        &MAudioPortsConfigExt);
    registerExtension(CLAP_EXT_AUDIO_PORTS,               &MAudioPortsExt);
    registerExtension(CLAP_EXT_CHECK_FOR_UPDATE,          &MCheckForUpdateExt);
    registerExtension(CLAP_EXT_CONFIGURABLE_AUDIO_PORTS,  &MConfigurableAudioPortsExt);
    registerExtension(CLAP_EXT_CONTEXT_MENU,              &MContextMenuExt);
    registerExtension(CLAP_EXT_CV,                        &MCVExt);
    registerExtension(CLAP_EXT_EXTENSIBLE_AUDIO_PORTS,    &MExtensibleAudioPortsExt);
    #ifndef SAT_NO_GUI
      registerExtension(CLAP_EXT_GUI,                     &MGuiExt);
    #endif
    registerExtension(CLAP_EXT_LATENCY,                   &MLatencyExt);
    registerExtension(CLAP_EXT_MIDI_MAPPINGS,             &MMidiMappingsExt);
    registerExtension(CLAP_EXT_NOTE_NAME,                 &MNoteNameExt);
    registerExtension(CLAP_EXT_NOTE_PORTS,                &MNotePortsExt);
    registerExtension(CLAP_EXT_PARAM_INDICATION,          &MParamIndicationExt);
    registerExtension(CLAP_EXT_PARAMS,                    &MParamsExt);
    registerExtension(CLAP_EXT_POSIX_FD_SUPPORT,          &MPosixFdSupportExt);
    registerExtension(CLAP_EXT_PRESET_LOAD,               &MPresetLoadExt);
    registerExtension(CLAP_EXT_REMOTE_CONTROLS,           &MRemoteControlsExt);
    registerExtension(CLAP_EXT_RENDER,                    &MRenderExt);
    registerExtension(CLAP_EXT_RESOURCE_DIRECTORY,        &MResourceDirectoryExt);
    registerExtension(CLAP_EXT_STATE,                     &MStateExt);
    registerExtension(CLAP_EXT_STATE_CONTEXT,             &MStateContextExt);
    registerExtension(CLAP_EXT_SURROUND,                  &MSurroundExt);
    registerExtension(CLAP_EXT_TAIL,                      &MTailExt);
    registerExtension(CLAP_EXT_THREAD_POOL,               &MThreadPoolExt);
    registerExtension(CLAP_EXT_TIMER_SUPPORT,             &MTimerSupportExt);
    registerExtension(CLAP_EXT_TRACK_INFO,                &MTrackInfoExt);
    registerExtension(CLAP_EXT_TRIGGERS,                  &MTriggersExt);
    registerExtension(CLAP_EXT_TUNING,                    &MTuningExt);
    registerExtension(CLAP_EXT_VOICE_INFO,                &MVoiceInfoExt);
  }

//------------------------------
public: // editor
//------------------------------

  // called from SAT_Plugin.gui_create()

  virtual SAT_Editor* createEditor(SAT_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight) {
    //SAT_PRINT;
    return new SAT_Editor(AListener,AWidth,AHeight);
  }

  //----------

  // called from SAT_Plugin.gui_set_parent()
  // see also gui_create (calculating default editor size if not defined)

  virtual bool initEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) {

    double width  = AWindow->getInitialWidth();
    //double height = AWindow->getInitialHeight();    
    //double scale  = AWindow->getScale();
    
    //AEditor->get_size(&width,&height);
    //SAT_Print("width %.f height %.f scale %.2f\n",width,height,scale);
    
    SAT_PanelWidget* root = new SAT_PanelWidget(0);
    AWindow->appendRootWidget(root);
    root->setFillBackground(false);
  
    const clap_plugin_descriptor_t* descriptor = getDescriptor();
    const char* format = getPluginFormat();
    const char* name = descriptor->name;
    
    SAT_PluginHeaderWidget* header = new SAT_PluginHeaderWidget(SAT_Rect(0,0,width,40),name,format);
    root->appendChildWidget(header);
    
    SAT_PluginFooterWidget* footer = new SAT_PluginFooterWidget(SAT_Rect(0,0,width,20));
    root->appendChildWidget(footer);
    
    SAT_PanelWidget* panel = new SAT_PanelWidget(0);
    root->appendChildWidget(panel);
    panel->setLayout(SAT_WIDGET_ALIGN_NONE,SAT_WIDGET_STRETCH_ALL);
    panel->setInnerBorder(SAT_Rect(10,10,10,10));
    panel->setSpacing(SAT_Point(5,5));
    
    uint32_t num = MParameters.size();
    for (uint32_t i=0; i<num; i++) {
      double x = 10;
      double y = 10 + (i * 20);
      double w = width - 20;
      double h = 20;
      SAT_SliderWidget* slider = new SAT_SliderWidget(SAT_Rect(x,y,w,h),"slider",0.0);
      panel->appendChildWidget(slider);
      slider->setLayout(SAT_WIDGET_ALIGN_TOP,SAT_WIDGET_STRETCH_HORIZONTAL);
      AEditor->connect(slider,getParameter(i));
    }
  
    //SAT_KnobWidget* knob = new SAT_KnobWidget(SAT_Rect(100,140,200,200),"Knob",0.5);
    //root->appendChildWidget(knob);
    //knob->setArcThickness(20);
    //knob->setTextSize(20);
    //knob->setValueSize(50);    
    
    return true;
  }

//------------------------------
public: // editor listener
//------------------------------

  // window -> editor -> this

  void do_editorListener_timer() override {
    //SAT_PRINT;
    flushModFromHostToGui();
    flushParamFromHostToGui();
  }

  //----------

  /*
    a knob or some other widget has been tweaked on the gui.
    we need to notify both the audio processor and the host
    process queue is flushed at the start of process()
    host queue is flushed at the end of process()
  */
  
  // AValue = widget-space (0..1)

  void do_editorListener_parameter_update(uint32_t AIndex, sat_param_t AValue) override {
    //SAT_PRINT;
    
    // parameters are in clap-space
    // widgets are 0..1
    
    //double value = AValue;
    double value = MParameters[AIndex]->denormalizeValue(AValue);
    
    setParameterValue(AIndex,value);
    queueParamFromGuiToHost(AIndex,value);
    queueParamFromGuiToAudio(AIndex,value);
  }

//------------------------------
public: // queues
//------------------------------

  // parameter, host -> gui
  // (automation, host-provided generic plugin interface)

  //-----

  /*
    TODO: check for duplicates
    pop updates to array, check array for duplicates..
    called from:
    - SAT_Plugin.handleParamValueEvent
  */

  void queueParamFromHostToGui(uint32_t AIndex, sat_param_t AValue) {
    //SAT_Print("%i = %f\n",AIndex,AValue);
    SAT_QueueItem item;
    item.type   = CLAP_EVENT_PARAM_VALUE;
    item.index  = AIndex;
    item.value  = AValue;
    if (!MParamFromHostToGuiQueue.write(item)) {
      //SAT_Log("queueParamFromHostToGui: couldn't write to queue\n");
      SAT_Print("queueParamFromHostToGui: couldn't write to queue\n");
    }

  }

  //----------

  /*
    TODO: check for duplicated (param/mod)
    called from:
    - SAT_Plugin.do_editorListener_timer
  */

  void flushParamFromHostToGui() {
    uint32_t count = 0;
    //SAT_Print("\n");
    SAT_QueueItem item;
    while (MParamFromHostToGuiQueue.read(&item)) {
      count += 1;
      SAT_Parameter* parameter = MParameters[item.index];
      
      // parameters are in clap-space
      // widgets are 0..1
      
      MEditor->updateParameterFromHost(parameter,item.value);
    }
    //if (count > 0) { SAT_Print("flushParamFromHostToGui: %i events\n",count); }
  }

  //----------

  // modulation, host -> gui
  // (modulation)

  //-----

  /*
    called from:
    - SAT_Plugin.handleParamModEvent
  */

  void queueModFromHostToGui(uint32_t AIndex, sat_param_t AValue) {
    //SAT_Print("%i = %f\n",AIndex,AValue);
    SAT_QueueItem item;
    item.type   = CLAP_EVENT_PARAM_MOD;
    item.index  = AIndex;
    item.value  = AValue;
    if (!MModFromHostToGuiQueue.write(item)) {
      //SAT_Log("queueModFromHostToGui: couldn't write to queue\n");
      SAT_Print("queueModFromHostToGui: couldn't write to queue\n");
    }
  }

  //----------

  /*
    TODO: check for duplicates
    pop updates to array, check array for duplicates..
    called from:
    - SAT_Plugin.do_editorListener_timer
  */

  void flushModFromHostToGui() {
    //SAT_Print("\n");
    SAT_QueueItem item;
    uint32_t count = 0;
    while (MModFromHostToGuiQueue.read(&item)) {
      count += 1;
      SAT_Parameter* parameter = MParameters[item.index];
      MEditor->updateModulationFromHost(parameter,item.value);
    }
    //if (count > 0) { SAT_Print("flushModFromHostToGui: %i events\n",count); }
  }

  //----------

  // parameter, gui -> audio
  // (tweak knob)

  //-----

  /*
    called from:
    - SAT_Plugin.do_editorListener_parameter_update
  */

  void queueParamFromGuiToAudio(uint32_t AIndex, sat_param_t AValue) {
    //SAT_Print("%i = %f\n",AIndex,AValue);
    SAT_QueueItem item;
    item.type   = CLAP_EVENT_PARAM_VALUE;
    item.index  = AIndex;
    item.value  = AValue;
    if (!MParamFromGuiToAudioQueue.write(item)) {
      //SAT_Log("queueParamFromGuiToAudio: couldn't write to queue\n");
//      SAT_Print("queueParamFromGuiToAudio: couldn't write to queue\n");
    }
  }

  //----------

  /*
    TODO: check for duplicates? can we have duplicates from gui?
    called from:
    - SAT_Plugin.process
  */

  void flushParamFromGuiToAudio() {
    uint32_t count = 0;
    //SAT_Print("\n");
    SAT_QueueItem item;
    while (MParamFromGuiToAudioQueue.read(&item)) {
      count += 1;
      //SAT_PRINT;
      clap_event_param_value_t event;
      event.header.size     = sizeof(clap_event_param_value_t);
      event.header.time     = 0;
      event.header.space_id = CLAP_CORE_EVENT_SPACE_ID; // SAT_EVENT_SPACE_ID
      event.header.type     = CLAP_EVENT_PARAM_VALUE;
      event.header.flags    = 0; // CLAP_EVENT_IS_LIVE, CLAP_EVENT_DONT_RECORD
      event.param_id        = item.index;
      event.cookie          = nullptr; // set?
      event.note_id         = -1;
      event.port_index      = -1;
      event.channel         = -1;
      event.key             = -1;
      event.value           = item.value;
      handleParamValue(&event); // handleParamValueEvent ?
    }
    //if (count > 0) { SAT_Print("flushParamFromGuiToAudio: %i events\n",count); }
  }

  //----------

  // parameter, gui -> host
  // (tweak knob)

  //-----

  /*
    called from:
    - SAT_Plugin.do_editorListener_parameter_update
  */

  void queueParamFromGuiToHost(uint32_t AIndex, sat_param_t AValue) {
    //SAT_Print("%i = %f\n",AIndex,AValue);
    SAT_QueueItem item;
    item.type   = CLAP_EVENT_PARAM_VALUE;
    item.index  = AIndex;
    item.value  = AValue;
    if (!MParamFromGuiToHostQueue.write(item)) {
      //SAT_Log("queueParamFromGuiToHost: couldn't write to queue\n");
//      SAT_Print("queueParamFromGuiToHost: couldn't write to queue\n");
    }
  }

  //----------

  /*
    TODO: check for duplicates? can we have duplicates from gui?
    called from:
    - SAT_Plugin.process()
  */

  void flushParamFromGuiToHost(const clap_output_events_t *out_events) {
    uint32_t count = 0;
    //SAT_Print("\n");
    SAT_QueueItem item;
    while (MParamFromGuiToHostQueue.read(&item)) {
      count += 1;
      //SAT_Print("%i = %.3f\n",item.index,item.value);
      SAT_Parameter* parameter = getParameter(item.index);

      // gesture begin

      {
        clap_event_param_gesture_t event;
        event.header.size     = sizeof(clap_event_param_gesture_t);
        event.header.time     = 0;
        event.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
        event.header.type     = CLAP_EVENT_PARAM_GESTURE_BEGIN;
        event.header.flags    = 0;
        event.param_id        = item.index;
        const clap_event_header_t* header = (const clap_event_header_t*)&event;
        out_events->try_push(out_events,header);
      }

      // param value

      {
        clap_event_param_value_t event;
        event.header.size     = sizeof(clap_event_param_value_t);
        event.header.time     = 0;
        event.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
        event.header.type     = CLAP_EVENT_PARAM_VALUE;
        event.header.flags    = 0; // CLAP_EVENT_IS_LIVE, CLAP_EVENT_DONT_RECORD
        event.param_id        = item.index;
        event.cookie          = parameter->getCookie(); // reaper needs the event.cookie ??
        event.note_id         = -1;
        event.port_index      = -1;
        event.channel         = -1;
        event.key             = -1;
        event.value           = item.value;
        // to host..
        const clap_event_header_t* header = (const clap_event_header_t*)&event;
        out_events->try_push(out_events,header);
      }

      // gesture end

      {
        clap_event_param_gesture_t event;
        event.header.size     = sizeof(clap_event_param_gesture_t);
        event.header.time     = 0;
        event.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
        event.header.type     = CLAP_EVENT_PARAM_GESTURE_END;
        event.header.flags    = 0;
        event.param_id        = item.index;
        const clap_event_header_t* header = (const clap_event_header_t*)&event;
        out_events->try_push(out_events,header);
      }

    } // while

    //if (count > 0) { SAT_Print("flushParamFromGuiToHost: %i events\n",count); }

  }

//------------------------------
public: // parameters
//------------------------------

  SAT_Parameter* appendParameter(SAT_Parameter* AParameter) {
    uint32_t index = MParameters.size();
    AParameter->setIndex(index);
    MParameters.append(AParameter);
    return AParameter;
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

  int32_t findParameter(const char* AName, const char* AModule=nullptr) {
    for (uint32_t i=0; i<MParameters.size(); i++) {
      const char* name = MParameters[i]->getName();
      if (strcmp(name,AName) == 0) {
        if (AModule) {
          const char* module = MParameters[i]->getModule();
          if (strcmp(module,AModule) == 0) return i;
          else return -1;
        }
        return i;
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

  SAT_ParameterArray* getParameters() {
    return &MParameters;
  }

  //----------

  sat_param_t getParameterValue(uint32_t AIndex) {
    return MParameters[AIndex]->getValue();
  }

  //----------

  void setParameterValue(uint32_t AIndex, sat_param_t AValue) {
    //MParameterValues[AIndex] = AValue;
    MParameters[AIndex]->setValue(AValue);
  }

  //----------

  /*
    called from:
    - SAT_Plugin.init()
  */

  void setDefaultParameterValues() {
    uint32_t num = MParameters.size();
    for (uint32_t i=0; i<num; i++) {
      double value = MParameters[i]->getDefaultValue();
      MParameters[i]->setValue(value);
    }
  }
  
  //----------
  
  /*
    called from:
    - SAT_Plugin.gui_set_parent()
  */

  void initEditorParameterValues() {
    uint32_t num = MParameters.size();
    for (uint32_t i=0; i<num; i++) {
      SAT_Parameter* param = MParameters[i];
      double value = MParameters[i]->getValue();//getDefaultValue();
      
      // parameters are in clap-space
      // widgets are 0..1
      
      //double value = MParameters[i]->getNormalizedValue();//getDefaultValue();
      MEditor->initParameterValue(param,i,value);
    }
  }

  void setAllParameterFlags(clap_param_info_flags AFlag) {
    for (uint32_t i=0; i<MParameters.size(); i++) {
      MParameters[i]->setFlag(AFlag);
    }
  }
  
  void clearAllParameterFlags(clap_param_info_flags AFlag) {
    for (uint32_t i=0; i<MParameters.size(); i++) {
      MParameters[i]->clearFlag(AFlag);
    }
  }
  
//------------------------------
public: // modulation
//------------------------------

  sat_param_t getModulationValue(uint32_t AIndex) {
    return MParameters[AIndex]->getModulation();
  }

  //----------

  void setModulationValue(uint32_t AIndex, sat_param_t AValue) {
    MParameters[AIndex]->setModulation(AValue);
  }

//------------------------------
public: // audio input ports
//------------------------------

  SAT_AudioPort* appendAudioInputPort(SAT_AudioPort* APort) {
    MAudioInputPorts.append(APort);
    return APort;
  }

  //----------

  SAT_AudioPort* appendStereoInputPort() {
    uint32_t index = MAudioInputPorts.size();
    SAT_AudioPort* port = new SAT_AudioPort(index,"Audio",CLAP_AUDIO_PORT_IS_MAIN,2,CLAP_PORT_STEREO,CLAP_INVALID_ID);
    MAudioInputPorts.append(port);
    return port;
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

  SAT_AudioPort* appendAudioOutputPort(SAT_AudioPort* APort) {
    MAudioOutputPorts.append(APort);
    return APort;
  }

  //----------

  SAT_AudioPort* appendStereoOutputPort() {
    uint32_t index = MAudioOutputPorts.size();
    SAT_AudioPort* port = new SAT_AudioPort(index,"Audio",CLAP_AUDIO_PORT_IS_MAIN,2,CLAP_PORT_STEREO,CLAP_INVALID_ID);
    MAudioOutputPorts.append(port);
    return port;
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

  SAT_NotePort* appendNoteInputPort(SAT_NotePort* APort) {
    //uint32_t index = MNoteOutputPorts.size();
    MNoteInputPorts.append(APort);
    return APort;
  }

  //----------

  SAT_NotePort* appendClapNoteInputPort() {
    uint32_t index = MNoteInputPorts.size();
    SAT_NotePort* port = new SAT_NotePort( index, CLAP_NOTE_DIALECT_CLAP, CLAP_NOTE_DIALECT_CLAP, "Clap Notes" );
    MNoteInputPorts.append(port);
    return port;
  }

  //----------

  SAT_NotePort* appendMidiNoteInputPort() {
    uint32_t index = MNoteInputPorts.size();
    SAT_NotePort* port = new SAT_NotePort( index, CLAP_NOTE_DIALECT_MIDI, CLAP_NOTE_DIALECT_MIDI, "Midi Notes" );
    MNoteInputPorts.append(port);
    return port;
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

  SAT_NotePort* appendNoteOutputPort(SAT_NotePort* APort) {
    MNoteOutputPorts.append(APort);
    return APort;
  }

  //----------

  SAT_NotePort* appendClapNoteOutputPort() {
    uint32_t index = MNoteOutputPorts.size();
    SAT_NotePort* port = new SAT_NotePort( index, CLAP_NOTE_DIALECT_CLAP, CLAP_NOTE_DIALECT_CLAP, "Clap Notes" );
    MNoteOutputPorts.append(port);
    return port;
  }

  //----------

  SAT_NotePort* appendMidiNoteOutputPort() {
    uint32_t index = MNoteOutputPorts.size();
    SAT_NotePort* port = new SAT_NotePort( index, CLAP_NOTE_DIALECT_MIDI, CLAP_NOTE_DIALECT_MIDI, "Midi Notes" );
    MNoteOutputPorts.append(port);
    return port;
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

//------------------------------
public: // events
//------------------------------

  //virtual void preProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
  //}

  //----------

  virtual void postProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
  }

  //----------

  /*
    if threading: push events onto each voice's oqn event queue, and handle
    them individually for each voice during audio processing..
  */

  //virtual void prepareEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
  //}

  //----------

  // called from SAT_Plugin.process(), just before processAudio()

  virtual void processEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
    if (!in_events) return;
    //if (!out_events) return;
    uint32_t size = in_events->size(in_events);
    for (uint32_t i=0; i<size; i++) {
      const clap_event_header_t* header = in_events->get(in_events,i);
      handleEvent(header);
    }
  }

  //----------

  //virtual void postProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
  //}

  //----------

  virtual bool handleNoteOn(const clap_event_note_t* event) { return false; }
  virtual bool handleNoteOff(const clap_event_note_t* event) { return false; }
  virtual bool handleNoteChoke(const clap_event_note_t* event) { return false; }
  virtual bool handleNoteExpression(const clap_event_note_expression_t* event) { return false; }
  virtual bool handleParamValue(const clap_event_param_value_t* event) { return false; }
  virtual bool handleParamMod(const clap_event_param_mod_t* event) { return false; }
  virtual bool handleTransport(const clap_event_transport_t* event) { return false; }
  virtual bool handleMidi(const clap_event_midi_t* event) { return false; }
  virtual bool handleMidiSysex(const clap_event_midi_sysex_t* event) { return false; }
  virtual bool handleMidi2(const clap_event_midi2_t* event) { return false; }

  //----------

  void handleEvent(const clap_event_header_t* header) {
    switch (header->type) {
      case CLAP_EVENT_NOTE_ON:          handleNoteOnEvent(        (clap_event_note_t*)header);            break;
      case CLAP_EVENT_NOTE_OFF:         handleNoteOffEvent(       (clap_event_note_t*)header);            break;
      case CLAP_EVENT_NOTE_CHOKE:       handleNoteChokeEvent(     (clap_event_note_t*)header);            break;
      case CLAP_EVENT_NOTE_EXPRESSION:  handleNoteExpressionEvent((clap_event_note_expression_t*)header); break;
      case CLAP_EVENT_PARAM_VALUE:      handleParamValueEvent(    (clap_event_param_value_t*)header);     break;
      case CLAP_EVENT_PARAM_MOD:        handleParamModEvent(      (clap_event_param_mod_t*)header);       break;
      case CLAP_EVENT_TRANSPORT:        handleTransportEvent(     (clap_event_transport_t*)header);       break;
      case CLAP_EVENT_MIDI:             handleMidiEvent(          (clap_event_midi_t*)header);            break;
      case CLAP_EVENT_MIDI_SYSEX:       handleMidiSysexEvent(     (clap_event_midi_sysex_t*)header);      break;
      case CLAP_EVENT_MIDI2:            handleMidi2Event(         (clap_event_midi2_t*)header);           break;
    }
  }

  //----------

  //void prepareEvent(const clap_event_header_t* header) {
  //}

//------------------------------
private: // handle events
//------------------------------

  /*
    note: if you override handleEvents(), no event handling is done
    automatically, so be sure to update MParameterValues, etc, manually if you
    need them..
  */

  //----------

  void handleNoteOnEvent(const clap_event_note_t* event) {
    /*bool handled =*/ handleNoteOn(event);
  }

  //----------

  void handleNoteOffEvent(const clap_event_note_t* event) {
    /*bool handled =*/ handleNoteOff(event);
    //if (!handled) {
    //  SAT_Note note;
    //  note.port     = event->port_index;
    //  note.channel  = event->channel;
    //  note.key      = event->key;
    //  note.noteid   = event->note_id;
    //  queueNoteEndFromAudioToHost(note);
    //}
  }

  //----------

  void handleNoteChokeEvent(const clap_event_note_t* event) {
    handleNoteChoke(event);
  }

  //----------

  void handleNoteExpressionEvent(const clap_event_note_expression_t* event) {
    //uint32_t expression = event->expression_id;
    //double value = event->value;
    //switch (expression) {
    //  case CLAP_NOTE_EXPRESSION_VOLUME:     SAT_Print("NOTE VOLUME EXPRESSION %.3f\n",value);     break; // with 0 < x <= 4, plain = 20 * log(x)
    //  case CLAP_NOTE_EXPRESSION_PAN:        SAT_Print("NOTE PAN EXPRESSION %.3f\n",value);        break; // pan, 0 left, 0.5 center, 1 right
    //  case CLAP_NOTE_EXPRESSION_TUNING:     SAT_Print("NOTE TUNING EXPRESSION %.3f\n",value);     break; // relative tuning in semitone, from -120 to +120
    //  case CLAP_NOTE_EXPRESSION_VIBRATO:    SAT_Print("NOTE VIBRATO EXPRESSION %.3f\n",value);    break; // 0..1
    //  case CLAP_NOTE_EXPRESSION_EXPRESSION: SAT_Print("NOTE EXPRESSION EXPRESSION %.3f\n",value); break; // 0..1
    //  case CLAP_NOTE_EXPRESSION_BRIGHTNESS: SAT_Print("NOTE BRIGHTNESS EXPRESSION %.3f\n",value); break; // 0..1
    //  case CLAP_NOTE_EXPRESSION_PRESSURE:   SAT_Print("NOTE PRESSURE EXPRESSION %.3f\n",value);   break; // 0..1
    //}
    handleNoteExpression(event);
  }

  //----------

  void handleParamValueEvent(const clap_event_param_value_t* event) {
    uint32_t  index         = event->param_id;
    double    value         = event->value;
    MParameters[index]->setValue(value);
//    uint32_t  process_count = MProcessContext.counter;
//    MParameters[index]->setLastUpdated(process_count);
//    MParameters[index]->setLastUpdatedValue(value);
    if (MEditor && MEditor->isOpen()) {
      queueParamFromHostToGui(index,value);
    }
    handleParamValue(event);
  }

  //----------

  /*
    TODO: don't send ALL mods to gui.. only last one in block
    set flag (this modulator changed..) and check in at end of process
  */

  void handleParamModEvent(const clap_event_param_mod_t* event) {
    uint32_t  index   = event->param_id;
    double    value   = event->amount;
    MParameters[index]->setModulation(value);
//    uint32_t  process_count = MProcessContext.counter;
//    MParameters[index]->setLastModulated(process_count);
//    MParameters[index]->setLastModulatedValue(value);
    if (MEditor && MEditor->isOpen()) {
      queueModFromHostToGui(index,value);
    }
    handleParamMod(event);
  }

  //----------

  void handleTransportEvent(const clap_event_transport_t* event) {
    bool handled = handleTransport(event);
    if (!handled) {
      if (event->flags & CLAP_TRANSPORT_HAS_TEMPO) {
        //MProcessContext.tempo     = event->tempo;             // in bpm
        //MProcessContext.tempo_inc = event->tempo_inc;         // tempo increment for each samples and until the next time info event
      }
      if (event->flags & CLAP_TRANSPORT_HAS_BEATS_TIMELINE) {
        //MProcessContext. = clap_beattime song_pos_beats;      // position in beats
        //clap_beattime loop_start_beats;
        //clap_beattime loop_end_beats;
        //clap_beattime bar_start;                              // start pos of the current bar
        //int32_t       bar_number;                             // bar at song pos 0 has the number 0
      }
      if (event->flags & CLAP_TRANSPORT_HAS_SECONDS_TIMELINE) {
        //clap_sectime  song_pos_seconds;                       // position in seconds
        //clap_sectime  loop_start_seconds;
        //clap_sectime  loop_end_seconds;
      }
      if (event->flags & CLAP_TRANSPORT_HAS_TIME_SIGNATURE) {
        //uint16_t      tsig_num;                               // time signature numerator
        //uint16_t      tsig_denom;                             // time signature denominator
      }
      if (event->flags & CLAP_TRANSPORT_IS_PLAYING) {}
      if (event->flags & CLAP_TRANSPORT_IS_RECORDING) {}
      if (event->flags & CLAP_TRANSPORT_IS_LOOP_ACTIVE) {}
      if (event->flags & CLAP_TRANSPORT_IS_WITHIN_PRE_ROLL) {}
    }
  }

  //----------

  void handleMidiEvent(const clap_event_midi_t* event) {
    bool handled = handleMidi(event);
    if (!handled) {
      uint8_t msg0 = event->data[0];
      //uint8_t index = event->data[1];
      //uint8_t value = event->data[2];
      uint32_t msg  = msg0 & 0xf0;
      //uint32_t chan = msg0 & 0x0f;
      switch (msg) {
        case SAT_MIDI_NOTE_OFF: {
          //SAT_Print("MIDI NOTE OFF. chan %i key %i vel %i\n",chan,data2,data3);
          //clap_event_note_t note_event;
          //note_event.header.size     = sizeof(clap_event_note_t);
          //note_event.header.time     = 0;
          //note_event.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
          //note_event.header.type     = CLAP_EVENT_NOTE_OFF;
          //note_event.header.flags    = 0; // CLAP_EVENT_IS_LIVE, CLAP_EVENT_DONT_RECORD
          //note_event.note_id         = (chan * 128) + index;
          //note_event.port_index      = event->port_index;
          //note_event.channel         = chan;
          //note_event.key             = index;
          //note_event.velocity        = value * (double)SAT_INV127;
          //const clap_event_header_t* header = (const clap_event_header_t*)&note_event;
          break;
        }
        case SAT_MIDI_NOTE_ON: {
          //SAT_Print("MIDI NOTE ON. chan %i key %i vel %i\n",chan,data2,data3);
          //clap_event_note_t note_event;
          //note_event.header.size     = sizeof(clap_event_note_t);
          //note_event.header.time     = 0;
          //note_event.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
          //note_event.header.type     = CLAP_EVENT_NOTE_ON;
          //note_event.header.flags    = 0; // CLAP_EVENT_IS_LIVE, CLAP_EVENT_DONT_RECORD
          //note_event.note_id         = (chan * 128) + index;
          //note_event.port_index      = event->port_index;
          //note_event.channel         = chan;
          //note_event.key             = index;
          //note_event.velocity        = value * (double)SAT_INV127;
          //const clap_event_header_t* header = (const clap_event_header_t*)&note_event;
          break;
        }
        case SAT_MIDI_POLY_AFTERTOUCH: {
          //SAT_Print("MIDI POLY AFTERTOUCH. chan %i data1 %i data2 %i\n",chan,data2,data3);
          // pressure note expression
          break;
        }
        case SAT_MIDI_CONTROL_CHANGE: {
          //SAT_Print("MIDI CONTROL_CHANGE. chan %i index %i val %i\n",chan,data2,data3);
          // cc74: brightness
          break;
        }
        case SAT_MIDI_PROGRAM_CHANGE: {
          //SAT_Print("MIDI PROGRAM CHANGE. chan %i data1 %i data2 %i\n",chan,data2,data3);
          break;
        }
        case SAT_MIDI_CHANNEL_AFTERTOUCH: {
          //SAT_Print("MIDI CHANNEL AFTERTOUCH. chan %i data1 %i data2 %i\n",chan,data2,data3);
          // pressure note expression
          break;
        }
        case SAT_MIDI_PITCHBEND: {
          //SAT_Print("MIDI PITCH BEND. chan %i data1 %i data2 %i\n",chan,data2,data3);
          // tuning note expression
          break;
        }
        case SAT_MIDI_SYS: {
          //SAT_Print("MIDI SYS. chan %i data1 %i data2 %i\n",chan,data2,data3);
          break;
        }
      }
    }
  }

  //----------

  void handleMidiSysexEvent(const clap_event_midi_sysex_t* event) {
    bool handled = handleMidiSysex(event);
    if (!handled) {
      //TODO
    }
  }

  //----------

  void handleMidi2Event(const clap_event_midi2_t* event) {
    handleMidi2(event);
  }

//------------------------------
public: // process audio
//------------------------------

  virtual void processAudio(SAT_ProcessContext* AContext) {
    const clap_process_t* process = AContext->process;
    uint32_t length = process->frames_count;
    if ((MAudioInputPorts.size() > 0) && (MAudioOutputPorts.size() > 0)) {
      float** inputs = process->audio_inputs[0].data32;
      float** outputs = process->audio_outputs[0].data32;
      SAT_CopyStereoBuffer(outputs,inputs,length);
    }
  }

  //----------

  /*
    if MEventMode is SAT_PLUGIN_EVENT_MODE_BLOCK (the default)
    handleEvents will first be called, and then this, to habndle the
    entire audio block in one go..

    if SAT_PLUGIN_EVENT_MODE_INTERLEAVED or SAT_PLUGIN_EVENT_MODE_QUANTIZED
    this will be called to fill each smaller block inbetween events

    also called from instruments, as the voices handles the event handling
    themselves..

  */

  virtual void processAudio(SAT_ProcessContext* AContext, uint32_t offset, uint32_t length) {
    const clap_process_t* process = AContext->process;
    //uint32_t length = process->frames_count;
    float* inputs[2];
    float* outputs[2];
    inputs[0]  = process->audio_inputs[0].data32[0] + offset;
    inputs[1]  = process->audio_inputs[0].data32[1] + offset;
    outputs[0] = process->audio_outputs[0].data32[0] + offset;
    outputs[1] = process->audio_outputs[0].data32[1] + offset;
    SAT_CopyStereoBuffer(outputs,inputs,length);
  }

  //----------

  // called from QUANTIZED, when we know the (const) slice size
  // (SAT_AUDIO_QUANTIZED_SIZE), and hope the optimizer will run
  // amok with loop unrolling, inlining, const propagation, etc, etc..
  //
  // but currently, we just cheat.. :-/

  virtual void processAudio(SAT_ProcessContext* AContext, uint32_t offset) {
    processAudio(AContext,offset,SAT_AUDIO_QUANTIZED_SIZE);
  }

  //----------

  // processes events at their sample accurate place, and audio inbetween

  virtual void processInterleaved(SAT_ProcessContext* AContext) {

    const clap_input_events_t* in_events = AContext->process->in_events;
    uint32_t remaining = AContext->process->frames_count;
    uint32_t num_events = in_events->size(in_events);
    uint32_t current_time = 0;
    uint32_t current_event = 0;

    while (remaining > 0) {
      if (current_event < num_events) {
        const clap_event_header_t* header = in_events->get(in_events,current_event);
        current_event += 1;
        int32_t length = header->time - current_time;

        //while (length > 0) {
        if (length > 0) {
          processAudio(AContext,current_time,length);
          remaining -= length;    // -= 32;
          current_time += length; // -= 32;
        }
        //processEventInterleaved(header);
        handleEvent(header);
      }
      else { // no more events
        int32_t length = remaining;
        processAudio(AContext,current_time,length);
        remaining -= length;
        current_time += length;
      }
    }
    //SAT_Assert( events.read(&event) == false );
  }

  //----------

  // split audio block in smaller, regular sizes, and quantize events
  // (process all events 'belonging' to the slice, at the atart ot the slice,
  // and then the audio)..
  // events could be processed up to (slicesize - 1) samples 'early'..

  void processQuantized(SAT_ProcessContext* AContext) {

    uint32_t buffer_length = AContext->process->frames_count;
    uint32_t remaining = buffer_length;
    uint32_t current_time = 0;
    uint32_t current_event = 0;
    uint32_t next_event_time = 0;
    const clap_input_events_t* in_events = AContext->process->in_events;
    uint32_t num_events = in_events->size(in_events);
    if (num_events > 0) {
      const clap_event_header_t* header = in_events->get(in_events,current_event);
      current_event += 1;
      next_event_time = header->time;
      do {
        // process events for next slice
        while (next_event_time < (current_time + SAT_AUDIO_QUANTIZED_SIZE)) {
          handleEvent(header);
          if (current_event < num_events) {
            header = in_events->get(in_events,current_event);
            // if (header)
            current_event += 1;
            next_event_time = header->time;
          }
          else {
            next_event_time = buffer_length; // ???
          }
        }
        // process next slice
        if (remaining < SAT_AUDIO_QUANTIZED_SIZE) {
          processAudio(AContext,current_time,remaining);
          current_time += remaining;
          remaining = 0;
        }
        else {
          processAudio(AContext,current_time);
          current_time += SAT_AUDIO_QUANTIZED_SIZE;
          remaining -= SAT_AUDIO_QUANTIZED_SIZE;
        }
      } while (remaining > 0);
    }
    else { // no events..
      do {
        if (remaining < SAT_AUDIO_QUANTIZED_SIZE) processAudio(AContext,current_time,remaining);
        else processAudio(AContext,current_time);
        current_time += SAT_AUDIO_QUANTIZED_SIZE;
        remaining -= SAT_AUDIO_QUANTIZED_SIZE;
      } while (remaining > 0);
    }
  }

};

//----------------------------------------------------------------------
#endif
