#ifndef sat_plugin_included
#define sat_plugin_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/clap/sat_clap_plugin.h"
#include "plugin/sat_audio_port.h"
#include "plugin/sat_note_port.h"
#include "plugin/sat_parameter.h"
#include "plugin/sat_process_context.h"
#include "plugin/sat_editor.h"
#include "plugin/sat_editor_listener.h"

//#include "plugin/sat_audio_processor.h"
// #include "plugin/sat_event_processor.h"
//#include "plugin/sat_parameter_manager.h"

typedef SAT_LockFreeQueue<uint32_t,SAT_PLUGIN_MAX_PARAM_EVENTS_PER_BLOCK> SAT_FromHostQueue;
typedef SAT_LockFreeQueue<uint32_t,SAT_PLUGIN_MAX_GUI_EVENTS_PER_BLOCK>   SAT_FromGuiQueue;

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

  const clap_plugin_descriptor_t* MDescriptor           = nullptr;
  SAT_Editor*                     MEditor               = nullptr;

  SAT_ParameterArray              MParameters           = {};
  SAT_AudioPortArray              MAudioInputPorts      = {};
  SAT_NotePortArray               MNoteInputPorts       = {};
  SAT_AudioPortArray              MAudioOutputPorts     = {};
  SAT_NotePortArray               MNoteOutputPorts      = {};

  SAT_ProcessContext              MProcessContext       = {};
  SAT_Dictionary<const void*>     MExtensions           = {};

  uint32_t                        MEventMode            = SAT_PLUGIN_EVENT_MODE_BLOCK;
  SAT_FromHostQueue               MParamFromHostToGui   = {};
  SAT_FromHostQueue               MModFromHostToGui     = {};
  SAT_FromGuiQueue                MParamFromGuiToAudio  = {};
  SAT_FromGuiQueue                MParamFromGuiToHost   = {};

  bool                            MIsInitialized        = false;
  bool                            MIsActivated          = false;
  bool                            MIsProcessing         = false;
  double                          MSampleRate           = 0.0;
  uint32_t                        MMinBufferSize        = 0;
  uint32_t                        MMaxBufferSize        = 0;
  int32_t                         MRenderMode           = CLAP_RENDER_REALTIME;

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

  //----------

  void registerDefaultExtension() {
    MExtensions.addItem(CLAP_EXT_AUDIO_PORTS,&MExtAudioPorts);
    MExtensions.addItem(CLAP_EXT_NOTE_PORTS,&MExtNotePorts);
    MExtensions.addItem(CLAP_EXT_PARAMS,&MExtParams);
    MExtensions.addItem(CLAP_EXT_GUI,&MExtGui);
  }

  //----------

  void registerDefaultSynthExtension() {
  }

  //----------

  void registerAllExtension() {
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

  virtual int32_t appendAudioInputPort(SAT_AudioPort* APort) {
    int32_t index = MAudioInputPorts.size();
    APort->setIndex(index);
    MAudioInputPorts.append(APort);
    return index;
  }

  //----------

  virtual int32_t appendAudioInputPort(const clap_audio_port_info_t* AInfo) {
    SAT_AudioPort* port = new SAT_AudioPort(AInfo);
    return appendAudioInputPort(port);
  }

  //----------

  virtual int32_t appendStereoAudioInputPort(const char* name, uint32_t flags=CLAP_AUDIO_PORT_IS_MAIN) {
    SAT_AudioPort* port = new SAT_AudioPort(name,flags,2,CLAP_PORT_STEREO);
    return appendAudioInputPort(port);
  }

  //----------

  virtual int32_t appendAudioOutputPort(SAT_AudioPort* APort) {
    int32_t index = MAudioOutputPorts.size();
    APort->setIndex(index);
    MAudioOutputPorts.append(APort);
    return index;
  }

  //----------

  virtual int32_t appendAudioOutputPort(const clap_audio_port_info_t* AInfo) {
    SAT_AudioPort* port = new SAT_AudioPort(AInfo);
    return appendAudioOutputPort(port);
  }

  //----------

  virtual int32_t appendStereoAudioOutputPort(const char* name, uint32_t flags=CLAP_AUDIO_PORT_IS_MAIN) {
    SAT_AudioPort* port = new SAT_AudioPort(name,flags,2,CLAP_PORT_STEREO);
    return appendAudioOutputPort(port);
  }

  //----------

  virtual void deleteAudioPorts() {
    for (uint32_t i=0; i<MAudioInputPorts.size(); i++) {
      if (MAudioInputPorts[i]) delete MAudioInputPorts[i];
    }
    for (uint32_t i=0; i<MAudioOutputPorts.size(); i++) {
      if (MAudioOutputPorts[i]) delete MAudioOutputPorts[i];
    }
    MAudioInputPorts.clear(true);
    MAudioOutputPorts.clear(true);
  }

//------------------------------
public: // note ports
//------------------------------

  virtual int32_t appendNoteInputPort(SAT_NotePort* APort) {
    int32_t index = MNoteInputPorts.size();
    APort->setIndex(index);
    MNoteInputPorts.append(APort);
    return index;
  }

  //----------

  virtual int32_t appendNoteInputPort(const clap_note_port_info_t* AInfo) {
    SAT_NotePort* port = new SAT_NotePort(AInfo);
    return appendNoteInputPort(port);
  }

  //----------

  virtual int32_t appendClapNoteInputPort(const char* name) {
    SAT_NotePort* port = new SAT_NotePort(name,CLAP_NOTE_DIALECT_CLAP,CLAP_NOTE_DIALECT_CLAP);
    return appendNoteInputPort(port);
  }

  //----------

  virtual int32_t appendMidiNoteInputPort(const char* name) {
    SAT_NotePort* port = new SAT_NotePort(name,CLAP_NOTE_DIALECT_MIDI,CLAP_NOTE_DIALECT_MIDI);
    return appendNoteInputPort(port);
  }

  //----------

  virtual int32_t appendNoteOutputPort(SAT_NotePort* APort) {
    int32_t index = MNoteOutputPorts.size();
    APort->setIndex(index);
    MNoteOutputPorts.append(APort);
    return index;
  }

  //----------

  virtual int32_t appendNoteOutputPort(const clap_note_port_info_t* AInfo) {
    SAT_NotePort* port = new SAT_NotePort(AInfo);
    return appendNoteOutputPort(port);
  }

  //----------

  virtual int32_t appendClapNoteOutputPort(const char* name) {
    SAT_NotePort* port = new SAT_NotePort(name,CLAP_NOTE_DIALECT_CLAP,CLAP_NOTE_DIALECT_CLAP);
    return appendNoteOutputPort(port);
  }

  //----------

  virtual int32_t appendMidiNoteOutputPort(const char* name) {
    SAT_NotePort* port = new SAT_NotePort(name,CLAP_NOTE_DIALECT_MIDI,CLAP_NOTE_DIALECT_MIDI);
    return appendNoteOutputPort(port);
  }

  //----------

  virtual void deleteNotePorts() {
    for (uint32_t i=0; i<MNoteInputPorts.size(); i++) {
      if (MNoteInputPorts[i]) delete MNoteInputPorts[i];
    }
    for (uint32_t i=0; i<MNoteOutputPorts.size(); i++) {
      if (MNoteOutputPorts[i]) delete MNoteOutputPorts[i];
    }
    MNoteInputPorts.clear(true);
    MNoteOutputPorts.clear(true);
  }

//------------------------------
public: // editor
//------------------------------

  virtual SAT_Editor* createEditor(SAT_EditorListener* AListener) {
    return new SAT_Editor(AListener);
    //return nullptr;
  }

  virtual void deleteEditor(SAT_Editor* AEditor) {
    delete AEditor;
  }

  //----------

  virtual void setupEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) {
    SAT_PRINT;
  }

  //----------

  //virtual void cleanupEditor(SAT_Editor* AEditor) {
  //}

//------------------------------
public: // events
//------------------------------

  virtual void handleNoteOn(clap_event_note_t* event) {}
  virtual void handleNoteOff(clap_event_note_t* event) {}
  virtual void handleNoteChoke(clap_event_note_t* event) {}
  virtual void handleNoteExpression(clap_event_note_expression_t* headeventer) {}
  virtual void handleParamValue(clap_event_param_value_t* event) {}
  virtual void handleParamMod(clap_event_param_mod_t* event) {}
  virtual void handleTransport(clap_event_transport_t* event) {}
  virtual void handleMidi(clap_event_midi_t* event) {}
  virtual void handleMidiSysex(clap_event_midi_sysex_t* event) {}
  virtual void handleMidi2(clap_event_midi2_t* event) {}

  //----------

  virtual void handleEvents(SAT_ProcessContext* AContext) {
    const clap_process_t* process = AContext->process;
    const clap_input_events_t* in_events = process->in_events;
    uint32_t num = in_events->size(in_events);
    switch (MEventMode) {
      case SAT_PLUGIN_EVENT_MODE_BLOCK: {
        for (uint32_t i=0; i<num; i++) {
          const clap_event_header_t* header;
          header = in_events->get(in_events,i);
          handleEvent(header);
        }
        break;
      }
      case SAT_PLUGIN_EVENT_MODE_INTERLEAVED: {
        break;
      }
      case SAT_PLUGIN_EVENT_MODE_QUANTIZED: {
        break;
      }
    }
  }

  //----------

  // virtual
  void handleEvent(const clap_event_header_t* header) {
    if (header->space_id == CLAP_CORE_EVENT_SPACE_ID) {
      switch (header->type) {
        case CLAP_EVENT_NOTE_ON:          handleNoteOn((clap_event_note_t*)header);
        case CLAP_EVENT_NOTE_OFF:         handleNoteOff((clap_event_note_t*)header);
        case CLAP_EVENT_NOTE_CHOKE:       handleNoteChoke((clap_event_note_t*)header);
        case CLAP_EVENT_NOTE_EXPRESSION:  handleNoteExpression((clap_event_note_expression_t*)header);
        case CLAP_EVENT_PARAM_VALUE:      handleParamValue((clap_event_param_value_t*)header);
        case CLAP_EVENT_PARAM_MOD:        handleParamMod((clap_event_param_mod_t*)header);
        case CLAP_EVENT_TRANSPORT:        handleTransport((clap_event_transport_t*)header);
        case CLAP_EVENT_MIDI:             handleMidi((clap_event_midi_t*)header);
        case CLAP_EVENT_MIDI_SYSEX:       handleMidiSysex((clap_event_midi_sysex_t*)header);
        case CLAP_EVENT_MIDI2:            handleMidi2((clap_event_midi2_t*)header);
      }
    }
  }

  //----------

  void handleNoteOnEvent(clap_event_note_t* event) {
    //int32_t id   = event->note_id;
    //int16_t port = event->port_index;
    //int16_t chan = event->key;
    //int16_t key  = event->key;
    //double  vel  = event->velocity;
    handleNoteOn(event);
  }

  //----------

  void handleNoteOffEvent(clap_event_note_t* event) {
    handleNoteOff(event);
  }

  //----------

  void handleNoteChokeEvent(clap_event_note_t* event) {
    handleNoteChoke(event);
  }

  //----------

  void handleNoteExpressionEvent(clap_event_note_expression_t* event) {
    handleNoteExpression(event);
  }

  //----------

  void handleParamValueEvent(clap_event_param_value_t* event) {
    uint32_t index = event->param_id; // !!!
    double value = event->value;
    SAT_Print("index %i value %.3f\n",index,value);
    MParameters[index]->setValue(value);
    handleParamValue(event);
    // host_to_gui
  }

  //----------

  void handleParamModEvent(clap_event_param_mod_t* event) {
    uint32_t index = event->param_id; // !!!
    double amount = event->amount;
    SAT_Print("index %i amount %.3f\n",index,amount);
    MParameters[index]->setModulation(amount);
    handleParamMod(event);
    // host_to_gui
  }

  //----------

  void handleTransportEvent(clap_event_transport_t* event) {
    handleTransport(event);
  }

  //----------

  void handleMidiEvent(clap_event_midi_t* event) {
    handleMidi(event); // data1,data2,data3
    uint8_t data1 = event->data[0];
    uint8_t data2 = event->data[1];
    uint8_t data3 = event->data[2];
    #ifdef SAT_PLUGIN_CONVERT_MIDI
      uint8_t msg   = (data1 & 0xf0);
      uint8_t chan  = (data1 & 0x0f);
      switch (msg) {
        case SAT_MIDI_NOTE_OFF: break;
        case SAT_MIDI_NOTE_ON: break;
        case SAT_MIDI_POLY_AFTERTOUCH: break;
        case SAT_MIDI_CONTROL_CHANGE: break;
        case SAT_MIDI_PROGRAM_CHANGE: break;
        case SAT_MIDI_CHANNEL_AFTERTOUCH: break;
        case SAT_MIDI_PITCHBEND: break;
        case SAT_MIDI_SYS: break;
      }
    #endif
  }

  //----------

  void handleMidiSysexEvent(clap_event_midi_sysex_t* event) {
    handleMidiSysex(event);
  }

  //----------

  void handleMidi2Event(clap_event_midi2_t* event) {
    handleMidi2(event);
  }

//------------------------------
public: // audio
//------------------------------

  virtual void processAudio(SAT_ProcessContext* AContext) {
    switch (MEventMode) {
      case SAT_PLUGIN_EVENT_MODE_BLOCK: {
        processAudioBlock(AContext);
        break;
      }
      case SAT_PLUGIN_EVENT_MODE_INTERLEAVED: {
        break;
      }
      case SAT_PLUGIN_EVENT_MODE_QUANTIZED: {
        break;
      }
    }
  }

  //----------

  void processAudioBlock(SAT_ProcessContext* AContext) {
    const clap_process_t* process = AContext->process;
    uint32_t length = process->frames_count;
    if (MAudioInputPorts.size() < 1) return;
    if (MAudioOutputPorts.size() < 1) return;
    if (MAudioInputPorts[0]->getInfo()->channel_count != 2) return;
    if (MAudioOutputPorts[0]->getInfo()->channel_count != 2) return;
    float* input0 = process->audio_inputs[0].data32[0];
    float* input1 = process->audio_inputs[0].data32[1];
    float* output0 = process->audio_outputs[0].data32[0];
    float* output1 = process->audio_outputs[0].data32[1];
    for (uint32_t i=0; i<length; i++) {
      *output0++ = *input0++;
      *output1++ = *input1++;
    }
  }

  //----------

  //void processAudioInterleaved(SAT_ProcessContext* AContext, uint32_t AOffset) {
  //}

  //----------

  //void processAudioQuantized(SAT_ProcessContext* AContext) {
  //}

//------------------------------
public: // queues
//------------------------------

  bool queueParamFromHostToGui(uint32_t AIndex) {
    return MParamFromHostToGui.write(AIndex);
  }

  bool queueModFromHostToGui(uint32_t AIndex) {
    return MModFromHostToGui.write(AIndex);
  }

  bool queueParamFromGuiToHost(uint32_t AIndex) {
    return MParamFromGuiToHost.write(AIndex);
  }

  bool queueParamFromGuiToAudio(uint32_t AIndex) {
    return MParamFromGuiToAudio.write(AIndex);
  }

  //----------

  void flushParamFromHostToGui() {
    uint32_t index;
    while (MParamFromHostToGui.read(&index)) {
      SAT_Print("%i\n",index);
      // todo
    }
  }

  void flushModFromHostToGui() {
    uint32_t index;
    while (MParamFromHostToGui.read(&index)) {
      SAT_Print("%i\n",index);
      // todo
    }
  }

  void flushParamFromGuiToHost() {
    uint32_t index;
    while (MParamFromHostToGui.read(&index)) {
      SAT_Print("%i\n",index);
      // todo
    }
  }

  void flushParamFromGuiToAudio() {
    uint32_t index;
    while (MParamFromHostToGui.read(&index)) {
      SAT_Print("%i\n",index);
      // todo
    }
  }

//------------------------------
protected: // SAT_EditorListener
//------------------------------

  void on_editorListener_timer() override {
    //flushParamFromHostToGui();
    //flushModFromHostToGui();
  }

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------



//------------------------------
protected: // clap_plugin
//------------------------------

  bool init() override {
    MIsInitialized = true;
    return true;
  }

  //----------

  void destroy() override {
    MIsInitialized = false;
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) override {
    MIsActivated = true;
    MSampleRate     = sample_rate;
    MMinBufferSize  = min_frames_count;
    MMaxBufferSize  = max_frames_count;
    return true;
  }

  //----------

  void deactivate() override {
    MIsActivated = false;
  }

  //----------

  bool start_processing() override {
    MIsProcessing = true;
    return true;
  }

  //----------

  void stop_processing() override {
    MIsProcessing = false;
  }

  //----------

  void reset() override {
    MProcessContext.process_counter = 0;
    MProcessContext.sample_counter = 0;
  }

  //----------

  clap_process_status process(const clap_process_t *process) override {
    MProcessContext.process = process;
    MProcessContext.samplerate = MSampleRate;
    MProcessContext.process_counter += 1;
    flushParamFromGuiToAudio();
    handleEvents(&MProcessContext);
    processAudio(&MProcessContext);
    flushParamFromGuiToHost();
    MProcessContext.sample_counter += process->frames_count;
    return CLAP_PROCESS_CONTINUE;
  }

  //----------

  const void* get_extension(const char *id) override {
    if (MExtensions.hasItem(id)) return MExtensions.getItem(id);
    return nullptr;
  }

  //----------

  void on_main_thread() override {
  }

//------------------------------
protected: // audio_ports
//------------------------------

  uint32_t audio_ports_count(bool is_input) override {
    if (is_input) {
      return MAudioInputPorts.size();
    }
    else {
      return MAudioOutputPorts.size();
    }
  }

  //----------

  bool audio_ports_get(uint32_t index, bool is_input, clap_audio_port_info_t *info) override {
    if (is_input) {
      memcpy(info,MAudioInputPorts[index]->getInfo(),sizeof(clap_audio_port_info_t));
    }
    else {
      memcpy(info,MAudioOutputPorts[index]->getInfo(),sizeof(clap_audio_port_info_t));
    }
      return true;
  }

//------------------------------
protected: // audio_ports_config
//------------------------------

  uint32_t audio_ports_config_count() override {
    return 1;
  }

  //----------

  bool audio_ports_config_get(uint32_t index, clap_audio_ports_config_t *config) override {
    switch (index) {
      case 0: {
        config->id                        = 0; // index;
        SAT_Strlcpy(config->name,"",CLAP_NAME_SIZE);
        config->input_port_count          = 1;
        config->output_port_count         = 1;
        // main input info
        config->has_main_input            = true;
        config->main_input_channel_count  = 2;
        config->main_input_port_type      = CLAP_PORT_STEREO;
        // main output info
        config->has_main_output           = true;
        config->main_output_channel_count = 2;
        config->main_output_port_type     = CLAP_PORT_STEREO;
        return true;
      }
    }
    return false;
  }

  //----------

  bool audio_ports_config_select(clap_id config_id) override {
    return true;
  }

//------------------------------
protected: // gui
//------------------------------

  bool gui_is_api_supported(const char *api, bool is_floating) override {
    SAT_Print("api %s is_floating %i\n",api,is_floating);
    #if defined(SAT_GUI_WAYLAND)
      if ((strcmp(api,CLAP_WINDOW_API_WAYLAND) == 0) && (is_floating)) return true;
    #endif
    #if defined(SAT_GUI_WIN32)
      if ((strcmp(api,CLAP_WINDOW_API_WIN32) == 0) && (!is_floating)) return true;
    #endif
    #if defined(SAT_GUI_X11)
      if ((strcmp(api,CLAP_WINDOW_API_X11) == 0) && (!is_floating)) return true;
    #endif
    return false;
  }

  //----------

  bool gui_get_preferred_api(const char **api, bool *is_floating) override {
    SAT_Print("\n");
    #if defined(SAT_GUI_WAYLAND)
      *api = CLAP_WINDOW_API_WAYLAND;
      *is_floating = true;
      return true;
    #endif
    #if defined(SAT_GUI_WIN32)
      *api = CLAP_WINDOW_API_WIN32;
      *is_floating = false;
      return true;
    #endif
    #if defined(SAT_GUI_X11)
      *api = CLAP_WINDOW_API_X11;
      *is_floating = false;
      return true;
    #endif
    return false;
  }

  //----------

  bool gui_create(const char *api, bool is_floating) override {
    SAT_Print("api %s is_floating %i\n",api,is_floating);
    MEditor = createEditor(this);
    //SAT_Print("> MEditor %p\n",MEditor);
    if (MEditor) {
      //setupEditor(MEditor);
      return true;
    }
    return false;
  }

  //----------

  void gui_destroy() override {
    SAT_Print("\n");
    if (MEditor) {
      //cleanupEditor(MEditor);
      deleteEditor(MEditor);
      MEditor = nullptr;
    }
  }

  //----------

  bool gui_set_scale(double scale) override {
    if (MEditor) return MEditor->set_scale(scale);
    return false;
  }

  //----------

  bool gui_get_size(uint32_t *width, uint32_t *height) override {
    if (MEditor) return MEditor->get_size(width,height);
    return false;
  }

  //----------

  bool gui_can_resize() override {
    if (MEditor) return MEditor->can_resize();
    return false;
  }

  //----------

  bool gui_get_resize_hints(clap_gui_resize_hints_t *hints) override {
    if (MEditor) return MEditor->get_resize_hints(hints);
    return false;
  }

  //----------

  bool gui_adjust_size(uint32_t *width, uint32_t *height) override {
    if (MEditor) return MEditor->adjust_size(width,height);
    return false;
  }

  //----------

  bool gui_set_size(uint32_t width, uint32_t height) override {
    if (MEditor) return MEditor->set_size(width,height);
    return false;
  }

  //----------

  bool gui_set_parent(const clap_window_t *window) override {
    if (MEditor) {
      bool result = MEditor->set_parent(window);
      SAT_Window* window = MEditor->getWindow();
      if (window) setupEditorWindow(MEditor,window);
      return result;
    }
    return false;
  }

  //----------

  bool gui_set_transient(const clap_window_t *window) override {
    if (MEditor) return MEditor->set_transient(window);
    return false;
  }

  //----------

  void gui_suggest_title(const char *title) override {
    if (MEditor) MEditor->suggest_title(title);
  }

  //----------

  bool gui_show() override {
    if (MEditor) return MEditor->show();
    return false;
  }

  //----------

  bool gui_hide() override {
    if (MEditor) return MEditor->hide();
    return false;
  }

//------------------------------
protected: // latency
//------------------------------

  uint32_t latency_get() override {
    return 0;
  }

//------------------------------
protected: // note_name
//------------------------------

  uint32_t note_name_count() override {
    return 0;
  }

  //----------

  bool note_name_get(uint32_t index, clap_note_name_t *note_name) override {
    return false;
  }

//------------------------------
protected: // note_ports
//------------------------------

  uint32_t note_ports_count(bool is_input) override {
    return 1;
  }

  //----------

  bool note_ports_get(uint32_t index, bool is_input, clap_note_port_info_t *info) override {
    switch (index) {
      case 0: {
        return true;
        info->id                  = 0; // index;
        SAT_Strlcpy(info->name,"",CLAP_NAME_SIZE);
        info->preferred_dialect   = CLAP_NOTE_DIALECT_CLAP;
        info->supported_dialects  = CLAP_NOTE_DIALECT_CLAP;// | CLAP_NOTE_DIALECT_MIDI | CLAP_NOTE_DIALECT_MIDI_MPE | CLAP_NOTE_DIALECT_MIDI2;
      }
    }
    return false;
  }

//------------------------------
protected: // params
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
    SAT_Strlcpy( out_buffer, MParameters[param_id]->valueToText(value), out_buffer_capacity );
    return true;
  }
  
  //----------

  bool params_text_to_value(clap_id param_id, const char *param_value_text, double *out_value) override {
    *out_value = MParameters[param_id]->textToValue(param_value_text);
    return true;
  }
  
  //----------

  void params_flush(const clap_input_events_t *in, const clap_output_events_t *out) override {
    // flush queues
  }

//------------------------------
protected: // posix_fd_support
//------------------------------

  void posix_fd_support_on_fd(int fd, clap_posix_fd_flags_t flags) override {
  }

//------------------------------
protected: // render
//------------------------------

  bool render_has_hard_realtime_requirement() override {
    return false;
  }

  //----------

  bool render_set(clap_plugin_render_mode mode) override {
    MRenderMode = mode;
    return false;
  }

//------------------------------
protected: // state
//------------------------------

  bool state_save(const clap_ostream_t *stream) override {
    return false;
  }

  //----------

  bool state_load(const clap_istream_t *stream) override {
    return false;
  }

//------------------------------
protected: // tail
//------------------------------

  uint32_t tail_get() override {
    return 0;
  }

//------------------------------
protected: // thread_pool
//------------------------------

  void thread_pool_exec(uint32_t task_index) override {
  }

//------------------------------
protected: // timer
//------------------------------

  void timer_support_on_timer(clap_id timer_id) override {
  }

//------------------------------
protected: // voice_info
//------------------------------

  bool voice_info_get(clap_voice_info_t *info) override {
    return false;
  }

//------------------------------
protected: // draft: ambisonic
//------------------------------

  bool ambisonic_is_config_supported(const clap_ambisonic_config_t *config) override {
    return false; 
  }

  //----------

  bool ambisonic_get_config(bool is_input, uint32_t port_index, clap_ambisonic_config_t *config) override {
    return false; 
  }

//------------------------------
protected: // draft: audio_ports_activation
//------------------------------

  bool audio_ports_activation_can_activate_while_processing() override {
    return false; 
  }

  //----------

  bool audio_ports_activation_set_active(bool is_input, uint32_t port_index, bool is_active, uint32_t sample_size) override {
    return false; 
  }

//------------------------------
protected: // draft: check_for_update
//------------------------------

  void check_for_update_check(bool include_preview) override {
  }

//------------------------------
protected: // draft: configurable_audio_ports
//------------------------------

  bool configurable_audio_ports_can_apply_configuration(const struct clap_audio_port_configuration_request *requests, uint32_t request_count) override {
    return false; 
  }

  //----------

  bool configurable_audio_ports_apply_configuration(const struct clap_audio_port_configuration_request *requests, uint32_t request_count) override {
    return false; 
  }

//------------------------------
protected: // draft: context_menu
//------------------------------

  bool context_menu_populate(const clap_context_menu_target_t  *target, const clap_context_menu_builder_t *builder) override {
    return false; 
  }

  //----------

  bool context_menu_perform(const clap_context_menu_target_t *target, clap_id action_id) override {
    return false; 
  }

//------------------------------
protected: // draft: cv
//------------------------------

  bool cv_get_channel_type(bool is_input, uint32_t port_index, uint32_t channel_index, uint32_t *channel_type) override {
    return false; 
  }

//------------------------------
protected: //
//------------------------------

  bool extensible_audio_ports_add_port(bool is_input, uint32_t channel_count, const char *port_type, const void *port_details) override {
    return false; 
  }

  //----------

  bool extensible_audio_ports_remove_port(bool is_input, uint32_t index) override {
    return false; 
  }

//------------------------------
protected: // draft: midi_mappings
//------------------------------

  uint32_t midi_mappings_count() override {
    return 0;
  }

  //----------

  bool midi_mappings_get(uint32_t index, clap_midi_mapping_t *mapping) override {
    //mapping->channel = 0;
    //mapping->number = 0;
    //apping->param_id = 0;
    return true;
  }
  
//------------------------------
protected: // draft: param_indication
//------------------------------

  void param_indication_set_mapping(clap_id param_id, bool has_mapping, const clap_color_t *color, const char *label, const char *description) override {
    //MParameters[param_id]->setMappingIndication(has_mapping,color,label,description);
  }
  
  //----------

  void param_indication_set_automation(clap_id param_id, uint32_t automation_state, const clap_color_t *color) override {
    //MParameters[param_id]->setAutomationIndication(automation_state,color);
  }
  
//------------------------------
protected: // draft: preset_load
//------------------------------

  bool preset_load_from_location(uint32_t location_kind, const char *location, const char *load_key) override {
    // loadPreset(location,load_key);
    return false;
  }
  
//------------------------------
protected: // draft: remote_controls
//------------------------------

  uint32_t remote_controls_count() override {
    return 0;
  }
  
  //----------

  bool remote_controls_get(uint32_t page_index, clap_remote_controls_page_t *page) override {
    // SAT_Strlcpy(page->section_name,"",CLAP_NAME_SIZE);
    // page->page_id = 0;
    // SAT_Strlcpy(page->name,"",CLAP_NAME_SIZE);
    // page->param_ids[0] = 0;// CLAP_INVALID_ID;
    // page->param_ids[1] = 1;//CLAP_INVALID_ID;
    // page->param_ids[2] = 2;//CLAP_INVALID_ID;
    // page->param_ids[3] = 3;//CLAP_INVALID_ID;
    // page->param_ids[4] = 4;//CLAP_INVALID_ID;
    // page->param_ids[5] = 5;//CLAP_INVALID_ID;
    // page->param_ids[6] = 6;//CLAP_INVALID_ID;
    // page->param_ids[7] = 7;//CLAP_INVALID_ID;
    // page->is_for_preset = false;
    return false;
  }
  
//------------------------------
protected: // draft: resource_directory
//------------------------------

  void resource_directory_set_directory(const char *path, bool is_shared) override {
    // MResourceDirectory = path;
    // MResourceDirectoryIsShared = is_shared;
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
protected: // draft: state_context
//------------------------------

  bool state_context_save(const clap_ostream_t *stream, uint32_t context_type) override {
    switch (context_type) {
      case CLAP_STATE_CONTEXT_FOR_DUPLICATE:  return state_save(stream);
      case CLAP_STATE_CONTEXT_FOR_PRESET:     return state_save(stream);
    }
    return true;
  }
  
  //----------

  bool state_context_load(const clap_istream_t *stream, uint32_t context_type) override {
    switch (context_type) {
      case CLAP_STATE_CONTEXT_FOR_DUPLICATE:  return state_load(stream);
      case CLAP_STATE_CONTEXT_FOR_PRESET:     return state_load(stream);
    }
    return true;
  }

//------------------------------
protected: // draft: surround
//------------------------------

  bool surround_is_channel_mask_supported(uint64_t channel_mask) override {
    return false;
  }

  //----------

  uint32_t surround_get_channel_map(bool is_input, uint32_t port_index, uint8_t *channel_map, uint32_t channel_map_capacity) override {
    return 0;
  }

//------------------------------
protected: // draft: track_info
//------------------------------

  // uint64_t      flags;                 // see the flags above
  // char          name[CLAP_NAME_SIZE];  // track name, available if flags contain CLAP_TRACK_INFO_HAS_TRACK_NAME
  // clap_color_t  color;                 // track color, available if flags contain CLAP_TRACK_INFO_HAS_TRACK_COLOR
  // int32_t       audio_channel_count;   // available if flags contain CLAP_TRACK_INFO_HAS_AUDIO_CHANNEL. see audio-ports.h, struct clap_audio_port_info to learn how to use channel count and port type
  // const char*   audio_port_type;

  void track_info_changed() override {
    // const clap_host_t* host = getClapHost();
    // const clap_host_track_info_t* track_info = (const clap_host_track_info_t*)host->get_extension(host,CLAP_EXT_TRACK_INFO);
    // clap_track_info_t info;
    // if (track_info->get(host,&info)) {
    // }
  }

//------------------------------
protected: // draft: triggers
//------------------------------

  uint32_t triggers_count() override {
    return 0;
  }

  //----------

  bool triggers_get_info(uint32_t index, clap_trigger_info_t *trigger_info) override {
    return false;
  }

//------------------------------
protected: // draft: tunung
//------------------------------

  void tuning_changed() override {
  }

};

//----------------------------------------------------------------------
#endif
