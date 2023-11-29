#ifndef sat_plugin_included
#define sat_plugin_included
//----------------------------------------------------------------------
/*
  TODO:

  * split in two:
    - SAT_NoGuiPlugin : public SAT_Plugin
    - SAT_GuiPlugin : public SAT_NoGuiPlugin

  
*/
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/clap/sat_clap_plugin.h"
#include "plugin/sat_audio_port.h"
#include "plugin/sat_host.h"
#include "plugin/sat_note_port.h"
#include "plugin/sat_parameter.h"
#include "plugin/sat_note.h"
#include "plugin/sat_process_context.h"

#if !defined (SAT_GUI_NOGUI)
  #include "plugin/sat_editor.h"
  #include "plugin/sat_editor_listener.h"
  #if defined (SAT_GUI_DEFAULT_EDITOR)
    #include "gui/sat_widgets.h"
  #endif
#endif

//#include "plugin/sat_audio_processor.h"
//#include "plugin/sat_event_processor.h"
//#include "plugin/sat_parameter_manager.h"

//----------------------------------------------------------------------

#if !defined (SAT_GUI_NOGUI)

// 128 bit
struct SAT_ParamQueueItem {
  uint32_t    type;
  uint32_t    index;
  union {
    double    value;
    SAT_Note  note;   // 64bit
  };
};

typedef SAT_Queue<SAT_ParamQueueItem,SAT_PLUGIN_MAX_MOD_EVENTS_PER_BLOCK>   SAT_ModFromHostToGuiQueue;
typedef SAT_Queue<SAT_ParamQueueItem,SAT_PLUGIN_MAX_PARAM_EVENTS_PER_BLOCK> SAT_ParamFromHostToGuiQueue;
typedef SAT_Queue<SAT_ParamQueueItem,SAT_PLUGIN_MAX_GUI_EVENTS_PER_BLOCK>   SAT_ParamFromGuiToAudioQueue;
typedef SAT_Queue<SAT_ParamQueueItem,SAT_PLUGIN_MAX_GUI_EVENTS_PER_BLOCK>   SAT_ParamFromGuiToHostQueue;

#endif // nogui

//----------

#define SAT_DEFAULT_PLUGIN_CONSTRUCTOR(PLUGIN)                                  \
  PLUGIN(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) \
  : SAT_Plugin(ADescriptor,AHost) {                                             \
  };

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Plugin
: public SAT_ClapPlugin

#if !defined (SAT_GUI_NOGUI)
, public SAT_EditorListener
#endif

{

  friend class SAT_LadspaPlugin;
  friend class SAT_Vst3Plugin;
  friend class SAT_Vst2Plugin;

  friend class SAT_LadspaEntry;
  friend class SAT_Vst2Entry;

//------------------------------
private:
//------------------------------

  const char*                       MPluginFormat               = "";

  const clap_plugin_descriptor_t*   MDescriptor                 = nullptr;
  SAT_Host*                         MHost                       = nullptr;

  SAT_ParameterArray                MParameters                 = {};
  SAT_AudioPortArray                MAudioInputPorts            = {};
  SAT_NotePortArray                 MNoteInputPorts             = {};
  SAT_AudioPortArray                MAudioOutputPorts           = {};
  SAT_NotePortArray                 MNoteOutputPorts            = {};

  SAT_ProcessContext                MProcessContext             = {};
  SAT_Dictionary<const void*>       MExtensions                 = {};

  uint32_t                          MEventMode                  = SAT_PLUGIN_EVENT_MODE_BLOCK;

  bool                              MIsInitialized              = false;
  bool                              MIsActivated                = false;
  bool                              MIsProcessing               = false;
  double                            MSampleRate                 = 0.0;
  uint32_t                          MMinBufferSize              = 0;
  uint32_t                          MMaxBufferSize              = 0;
  int32_t                           MRenderMode                 = CLAP_RENDER_REALTIME;
  const char*                       MResourceDirectory          = "";
  bool                              MResourceDirectoryShared    = false;

  uint64_t                          MTrackFlags                 = 0;
  char                              MTrackName[CLAP_NAME_SIZE]  = {0};
  SAT_Color                         MTrackColor                 = SAT_Black;
  int32_t                           MTrackChannelCount          = 0;
  const char*                       MTrackPortType              = nullptr;
  bool                              MTrackIsReturnTrack         = false;
  bool                              MTrackIsBus                 = false;
  bool                              MTrackIsMaster              = false;

  #if !defined (SAT_GUI_NOGUI)

    SAT_Editor*                     MEditor                     = nullptr;
    uint32_t                        MInitialEditorWidth         = 0;//512;
    uint32_t                        MInitialEditorHeight        = 0;//512;
    double                          MInitialEditorScale         = 1.0;

    SAT_ParamFromHostToGuiQueue     MParamFromHostToGuiQueue    = {};   // when the host changes a parameter, we need to redraw it
    SAT_ModFromHostToGuiQueue       MModFromHostToGuiQueue      = {};   // --"-- modulation
    SAT_ParamFromGuiToAudioQueue    MParamFromGuiToAudioQueue   = {};   // twweak knob, send parameter value to audio process
    SAT_ParamFromGuiToHostQueue     MParamFromGuiToHostQueue    = {};   // tell host about parameter change

    // set in gui_create (false), gui_destroy (true)
    // checked in handleParamValueEvent, handleParamModEvent
    std::atomic<bool>               MIsEditorClosing            {false};

  #endif // nogui


//------------------------------
public:
//------------------------------

  SAT_Plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_ClapPlugin(ADescriptor,AHost) {
    MProcessContext.plugin = this;
    MHost = new SAT_Host(AHost);
  }

  //----------

  virtual ~SAT_Plugin() {
    SAT_PRINT;
    #ifndef SAT_NO_AUTODELETE
      SAT_PRINT;
      deleteAudioPorts();
      deleteNotePorts();
      deleteParameters();
      SAT_PRINT;
    #endif
    delete MHost;
    SAT_PRINT;
  }

//------------------------------
public:
//------------------------------

  void setEventMode(uint32_t AMode) { MEventMode = AMode; }
  void setPluginFormat(const char* AFormat) { MPluginFormat = AFormat; }
  //void setProcessThreaded(bool AThreaded=true)  { MProcessThreaded = AThreaded; }

  SAT_Host* getHost() { return MHost; }

//------------------------------
public: // extensions
//------------------------------

  virtual void registerExtension(const char* AId, const void* APtr) {
    if (!MExtensions.hasItem(AId)) {
      MExtensions.addItem(AId,APtr);
    }
  }

  virtual void unregisterExtension(const char* AId) {
    if (MExtensions.hasItem(AId)) {
      MExtensions.removeItem(AId);
    }
  }

  //----------

  virtual void registerDefaultExtensions() {
    registerExtension(CLAP_EXT_AUDIO_PORTS,               &MExtAudioPorts);
    #if !defined (SAT_GUI_NOGUI)
      registerExtension(CLAP_EXT_GUI,                     &MExtGui);
    #endif
    registerExtension(CLAP_EXT_PARAMS,                    &MExtParams);
    registerExtension(CLAP_EXT_STATE,                     &MExtState);
  }

  //----------

  virtual void registerDefaultSynthExtensions() {
    registerDefaultExtensions();
    registerExtension(CLAP_EXT_NOTE_PORTS,                &MExtNotePorts);
    registerExtension(CLAP_EXT_THREAD_POOL,               &MExtThreadPool);
    registerExtension(CLAP_EXT_VOICE_INFO,                &MExtVoiceInfo);
  }

  //----------

  virtual void registerAllExtension() {
    registerExtension(CLAP_EXT_AUDIO_PORTS,               &MExtAudioPorts);
    registerExtension(CLAP_EXT_AUDIO_PORTS_CONFIG,        &MExtAudioPortsConfig);
    #ifndef SAT_GUI_NOGUI
      registerExtension(CLAP_EXT_GUI,                     &MExtGui);
    #endif
    registerExtension(CLAP_EXT_LATENCY,                   &MExtLatency);
    registerExtension(CLAP_EXT_NOTE_NAME,                 &MExtNoteName);
    registerExtension(CLAP_EXT_NOTE_PORTS,                &MExtNotePorts);
    registerExtension(CLAP_EXT_PARAMS,                    &MExtParams);
    registerExtension(CLAP_EXT_POSIX_FD_SUPPORT,          &MExtPosixFdSupport);
    registerExtension(CLAP_EXT_RENDER,                    &MExtRender);
    registerExtension(CLAP_EXT_STATE,                     &MExtState);
    registerExtension(CLAP_EXT_TAIL,                      &MExtTail);
    registerExtension(CLAP_EXT_THREAD_POOL,               &MExtThreadPool);
    registerExtension(CLAP_EXT_TIMER_SUPPORT,             &MExtTimerSupport);
    registerExtension(CLAP_EXT_VOICE_INFO,                &MExtVoiceInfo);
  }

  //----------

  virtual void registerDraftExtension() {
    registerExtension(CLAP_EXT_AMBISONIC,                 &MExtAmbisonic);
    registerExtension(CLAP_EXT_AUDIO_PORTS_ACTIVATION,    &MExtAudioPortsActivation);
    registerExtension(CLAP_EXT_CHECK_FOR_UPDATE,          &MExtCheckForUpdate);
    registerExtension(CLAP_EXT_CONFIGURABLE_AUDIO_PORTS,  &MExtConfigurableAudioPorts);
    registerExtension(CLAP_EXT_CONTEXT_MENU,              &MExtContextMenu);
    registerExtension(CLAP_EXT_CV,                        &MExtCv);
    registerExtension(CLAP_EXT_EXTENSIBLE_AUDIO_PORTS,    &MExtExtensibleAudioPorts);
    registerExtension(CLAP_EXT_MIDI_MAPPINGS,             &MExtMidiMappings);
    registerExtension(CLAP_EXT_PARAM_INDICATION,          &MExtParamIndication);
    registerExtension(CLAP_EXT_PRESET_LOAD,               &MExtPresetLoad);
    registerExtension(CLAP_EXT_REMOTE_CONTROLS,           &MExtRemoteControls);
    registerExtension(CLAP_EXT_RESOURCE_DIRECTORY,        &MExtResourceDirectory);
    registerExtension(CLAP_EXT_STATE_CONTEXT,             &MExtStateContext);
    registerExtension(CLAP_EXT_SURROUND,                  &MExtSurround);
    registerExtension(CLAP_EXT_TRACK_INFO,                &MExtTrackInfo);
    registerExtension(CLAP_EXT_TRIGGERS,                  &MExtTriggers);
    registerExtension(CLAP_EXT_TUNING,                    &MExtTuning);
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
public: // parameters
//------------------------------

  virtual SAT_Parameter* appendParameter(SAT_Parameter* AParameter) {
    int32_t index = MParameters.size();
    AParameter->setIndex(index);
    MParameters.append(AParameter);
    return AParameter;
  }

  //----------

  virtual SAT_Parameter* appendParameter(const clap_param_info_t* AInfo) {
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

  //----------

  virtual uint32_t getNumParameters() {
    return MParameters.size();
  }

  //----------

  virtual SAT_Parameter* getParameter(uint32_t AIndex) {
    return MParameters[AIndex];
  }

  virtual sat_param_t getParameterValue(uint32_t AIndex) {
    return MParameters[AIndex]->getValue();
  }

  //----------

  // virtual void updateParameterFromGui(SAT_Parameter* AParameter) {
  //   uint32_t index = AParameter->getIndex();
  //   sat_param_t value = AParameter->getValue();
  //   SAT_Print("index %i value %f\n",index,value);
  // }

//------------------------------
//
//------------------------------

  void setParameterValue(uint32_t AIndex, sat_param_t AValue) {
    //MParameterValues[AIndex] = AValue;
    MParameters[AIndex]->setValue(AValue);
  }

  //----------

  void setDefaultParameterValues() {
    uint32_t num = MParameters.size();
    for (uint32_t i=0; i<num; i++) {
      double value = MParameters[i]->getDefaultValue();
      MParameters[i]->setValue(value);
    }
  }
  
  //----------
  
  #if !defined (SAT_GUI_NOGUI)

  void initEditorParameterValues() {
    uint32_t num = MParameters.size();
    for (uint32_t i=0; i<num; i++) {
      SAT_Parameter* param = MParameters[i];
      double value = MParameters[i]->getValue();//getDefaultValue();
      // parameters are in clap-space
      // widgets are 0..1
      uint32_t sub = 0;//param->getWidgetIndex();
      //SAT_Print("sub %i\n",sub);
      MEditor->initParameterValue(param,i,sub,value); // (arg value  = clap space)
    }
  }

  #endif // nogui

//------------------------------
public: // modulation
//------------------------------

  virtual sat_param_t getParameterModulation(uint32_t AIndex) {
    return MParameters[AIndex]->getModulation();
  }

  // void resetAllModulations() {
  //   uint32_t num_params = getNumParameters();
  //   for (uint32_t i=0; i<num_params; i++) {
  //     SAT_Print("%i : %f\n",i);
  //     //setModulation(0);
  //   }
  // }

//------------------------------
public: // editor
//------------------------------

  #if !defined (SAT_GUI_NOGUI)

  virtual void setInitialEditorSize(uint32_t AWidth, uint32_t AHeight, double AScale) {
    MInitialEditorWidth = AWidth;
    MInitialEditorHeight = AHeight;
    MInitialEditorScale = AScale;
  }

  //----------

  // called from gui_create()

  virtual SAT_Editor* createEditor(SAT_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight, double AScale) {
    return new SAT_Editor(AListener,AWidth,AHeight,AScale,0);
    //return nullptr;
  }

  //----------

  virtual void deleteEditor(SAT_Editor* AEditor) {
    delete AEditor;
  }

  //----------

  // called from gui_set_parent()

  virtual void setupEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) {
    //SAT_PRINT;
    setupDefaultEditor(AEditor,AWindow);
  }

  //----------

  //virtual void cleanupEditor(SAT_Editor* AEditor) {
  //}

  //----------

  virtual SAT_Point getDefaultEditorSize() {
    SAT_Point p;
    uint32_t numparams = getNumParameters();
    p.w = 200;
    p.h = 30 + 10 + (numparams * 25) + 10 + 20;
    return p;
  }

  //----------

  
  virtual void setupDefaultEditor(SAT_Editor* AEditor, SAT_Window* AWindow) {
    #if defined (SAT_GUI_DEFAULT_EDITOR)
      uint32_t numparams = getNumParameters();
      SAT_Point size = getDefaultEditorSize();
      SAT_RootWidget* root = new SAT_RootWidget(0,AWindow);
      AWindow->setRootWidget(root);
      const clap_plugin_descriptor_t* descriptor = getClapDescriptor();
      SAT_Print("descriptor %p\n",descriptor);
      SAT_PluginHeaderWidget* header = new SAT_PluginHeaderWidget(30,descriptor->name);
      root->appendChildWidget(header);
      SAT_PluginFooterWidget* footer = new SAT_PluginFooterWidget(20,"...");
      root->appendChildWidget(footer);
      SAT_Widget* middle = new SAT_Widget(0);
      root->appendChildWidget(middle);
      middle->addLayoutFlag(SAT_WIDGET_LAYOUT_STRETCH_ALL);
      middle->setLayoutInnerBorder(SAT_Rect(10,10,10,10));
      for (uint32_t i=0; i<numparams; i++) {
        SAT_Parameter* param = getParameter(i);
        double x = 10;
        double y = 10 + (i * 25);
        double w = 180;
        double h = 20;
        SAT_SliderWidget* slider = new SAT_SliderWidget(SAT_Rect(x,y,w,h),param->getName(),param->getValue());
        slider->setTextOffset(SAT_Rect(5,0,0,0));
        slider->setValueOffset(SAT_Rect(0,0,5,0));
        middle->appendChildWidget(slider);
        AEditor->connect(slider,param);
      }
    #endif // default editor
  }


  #endif // nogui

//------------------------------
public: // presets
//------------------------------

  virtual bool loadPresetFromFile(const char* ALocation, const char* AKey) {
    //return false;
    char line_buffer[256] = {0};
    SAT_File file = {};
    if (!file.exists(ALocation)) {
      SAT_Print("Error! '%s' does not exist\n",ALocation);
      return false;
    }
    
    if (file.open(ALocation,SAT_FILE_READ_TEXT)) {
      for (uint32_t i=0; i<5; i++) file.readLine(line_buffer,256); // skip metadata
      
      // hex

      //sat_param_t param_buffer[SAT_PLUGIN_MAX_PARAMETERS] = {0};
      //void* ptr = param_buffer;
      //while (file.readLine(line_buffer,256)) {
      //  if (line_buffer[strlen(line_buffer)-1] == '\n') line_buffer[strlen(line_buffer)-1] = 0;
      //  if (line_buffer[0] != 0) {
      //    SAT_Print("%s\n",line_buffer);
      //    ptr = SAT_HexDecode(ptr,line_buffer,32); // num bytes
      //  }
      //}
      //sat_param_t* param_ptr = (sat_param_t*)ptr;
      //uint32_t num_params = getNumParameters();
      //for (uint32_t i=0; i<num_params; i++) {
      //  sat_param_t value = *param_ptr++;
      //  SAT_Print("%i : %f\n",i,value);
      //}
      //setAllParameters(param_buffer);
      
      // ascii
      
      uint32_t i = 0;
      while (file.readLine(line_buffer,256)) {
        if (line_buffer[strlen(line_buffer)-1] == '\n') line_buffer[strlen(line_buffer)-1] = 0;
        if (line_buffer[0] != 0) {
          
          SAT_Print("line %i: '%s'\n",i,line_buffer);
          
          //ptr = SAT_HexDecode(ptr,line_buffer,32); // num bytes
          //setParameterValue(i,v);

          //double v = atof(line_buffer);
          //SAT_Print("%i = %f\n",i,v);
          
          i += 1;
        }
      }
      file.close();
    }
    else {
      SAT_Print("Error opening file '%s'\n",ALocation);
      return false;
    }
    
    return true;

  }

//------------------------------
public: // events
//------------------------------

  virtual bool on_plugin_noteOn(const clap_event_note_t* event) { return false; }
  virtual bool on_plugin_noteOff(const clap_event_note_t* event) { return false; }
  virtual bool on_plugin_noteChoke(const clap_event_note_t* event) { return false; }
  virtual bool on_plugin_noteExpression(const clap_event_note_expression_t* event) { return false; }
  virtual bool on_plugin_paramValue(const clap_event_param_value_t* event) { return false; }
  virtual bool on_plugin_paramMod(const clap_event_param_mod_t* event) { return false; }
  virtual bool on_plugin_transport(const clap_event_transport_t* event) { return false; }
  virtual bool on_plugin_midi(const clap_event_midi_t* event) { return false; }
  virtual bool on_plugin_midiSysex(const clap_event_midi_sysex_t* event) { return false; }
  virtual bool on_plugin_midi2(const clap_event_midi2_t* event) { return false; }

//------------------------------
//
//------------------------------

  virtual void preProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
  }

  //----------

  virtual void postProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
  }

//------------------------------
//
//------------------------------

  // virtual
  bool handleEvent(const clap_event_header_t* header) {
    if (header->space_id == CLAP_CORE_EVENT_SPACE_ID) {
      switch (header->type) {
        case CLAP_EVENT_NOTE_ON:          return handleNoteOnEvent((clap_event_note_t*)header);
        case CLAP_EVENT_NOTE_OFF:         return handleNoteOffEvent((clap_event_note_t*)header);
        case CLAP_EVENT_NOTE_CHOKE:       return handleNoteChokeEvent((clap_event_note_t*)header);
        case CLAP_EVENT_NOTE_EXPRESSION:  return handleNoteExpressionEvent((clap_event_note_expression_t*)header);
        case CLAP_EVENT_PARAM_VALUE:      return handleParamValueEvent((clap_event_param_value_t*)header);
        case CLAP_EVENT_PARAM_MOD:        return handleParamModEvent((clap_event_param_mod_t*)header);
        case CLAP_EVENT_TRANSPORT:        return handleTransportEvent((clap_event_transport_t*)header);
        case CLAP_EVENT_MIDI:             return handleMidiEvent((clap_event_midi_t*)header);
        case CLAP_EVENT_MIDI_SYSEX:       return handleMidiSysexEvent((clap_event_midi_sysex_t*)header);
        case CLAP_EVENT_MIDI2:            return handleMidi2Event((clap_event_midi2_t*)header);
      }
    }
    return false;
  }

  //----------

  bool handleNoteOnEvent(clap_event_note_t* event) {
    bool result = on_plugin_noteOn(event);
    if (result) return true;
    return false;
  }

  //----------

  bool handleNoteOffEvent(clap_event_note_t* event) {
    bool result = on_plugin_noteOff(event);
    if (result) return true;
    return false;
  }

  //----------

  bool handleNoteChokeEvent(clap_event_note_t* event) {
    bool result = on_plugin_noteChoke(event);
    if (result) return true;
    return false;
  }

  //----------

  bool handleNoteExpressionEvent(clap_event_note_expression_t* event) {
    bool result = on_plugin_noteExpression(event);
    if (result) return true;
    else {
      // uint32_t expression = event->expression_id;
      // double value = event->value;
      // switch (expression) {
      //   case CLAP_NOTE_EXPRESSION_VOLUME:     SAT_Print("NOTE VOLUME EXPRESSION %.3f\n",value);     break; // with 0 < x <= 4, plain = 20 * log(x)
      //   case CLAP_NOTE_EXPRESSION_PAN:        SAT_Print("NOTE PAN EXPRESSION %.3f\n",value);        break; // pan, 0 left, 0.5 center, 1 right
      //   case CLAP_NOTE_EXPRESSION_TUNING:     SAT_Print("NOTE TUNING EXPRESSION %.3f\n",value);     break; // relative tuning in semitone, from -120 to +120
      //   case CLAP_NOTE_EXPRESSION_VIBRATO:    SAT_Print("NOTE VIBRATO EXPRESSION %.3f\n",value);    break; // 0..1
      //   case CLAP_NOTE_EXPRESSION_EXPRESSION: SAT_Print("NOTE EXPRESSION EXPRESSION %.3f\n",value); break; // 0..1
      //   case CLAP_NOTE_EXPRESSION_BRIGHTNESS: SAT_Print("NOTE BRIGHTNESS EXPRESSION %.3f\n",value); break; // 0..1
      //   case CLAP_NOTE_EXPRESSION_PRESSURE:   SAT_Print("NOTE PRESSURE EXPRESSION %.3f\n",value);   break; // 0..1
      // }
    }
    return false;

  }

  //----------

  bool handleParamValueEvent(clap_event_param_value_t* event) {
    uint32_t index = event->param_id; // !!!
    double value = event->value;
    //SAT_Print("from host (event).. index %i value %.3f\n",index,value);
    MParameters[index]->setValue(value);

    #if !defined (SAT_GUI_NOGUI)

    if (!MIsEditorClosing) {
      if (MEditor && MEditor->isOpen()) {
        //queueParamFromHostToGui(index,value);
        MParameters[index]->setGuiAutomationDirty(true);
        MParameters[index]->setLastAutomatedValue(value);
      }
    }

    #endif

    return on_plugin_paramValue(event);
  }

  //----------

  bool handleParamModEvent(clap_event_param_mod_t* event) {
    uint32_t index = event->param_id; // !!!
    double amount = event->amount;
    //SAT_Print("from host (event).. index %i amount %.3f\n",index,amount);
    MParameters[index]->setModulation(amount);

    #if !defined (SAT_GUI_NOGUI)
    
    if (!MIsEditorClosing) {
      if (MEditor && MEditor->isOpen()) {
        //queueModFromHostToGui(index,value);
        MParameters[index]->setGuiModulationDirty(true);
        MParameters[index]->setLastModulatedValue(amount);
      }
    }

    #endif

    return on_plugin_paramMod(event);
  }

  //----------

  bool handleTransportEvent(const clap_event_transport_t* event) {
    bool result = on_plugin_transport(event);
    if (result) return true;

    if (event->flags & CLAP_TRANSPORT_HAS_TEMPO) {
      //MProcessContext.has_tempo = true
      //MProcessContext.tempo     = event->tempo;             // in bpm
      //MProcessContext.tempo_inc = event->tempo_inc;         // tempo increment for each samples and until the next time info event
    }
    if (event->flags & CLAP_TRANSPORT_HAS_BEATS_TIMELINE) {
      //MProcessContext.has_beats = true
      //MProcessContext. = clap_beattime song_pos_beats;      // position in beats
      //clap_beattime loop_start_beats;
      //clap_beattime loop_end_beats;
      //clap_beattime bar_start;                              // start pos of the current bar
      //int32_t       bar_number;                             // bar at song pos 0 has the number 0
    }
    if (event->flags & CLAP_TRANSPORT_HAS_SECONDS_TIMELINE) {
      //MProcessContext.has_seconds = true
      //clap_sectime  song_pos_seconds;                       // position in seconds
      //clap_sectime  loop_start_seconds;
      //clap_sectime  loop_end_seconds;
    }
    if (event->flags & CLAP_TRANSPORT_HAS_TIME_SIGNATURE) {
      //MProcessContext.has_timesig = true
      //uint16_t      tsig_num;                               // time signature numerator
      //uint16_t      tsig_denom;                             // time signature denominator
    }
    if (event->flags & CLAP_TRANSPORT_IS_PLAYING) {}
    if (event->flags & CLAP_TRANSPORT_IS_RECORDING) {}
    if (event->flags & CLAP_TRANSPORT_IS_LOOP_ACTIVE) {}
    if (event->flags & CLAP_TRANSPORT_IS_WITHIN_PRE_ROLL) {}

    return false;

  }

  //----------

  bool handleMidiEvent(clap_event_midi_t* event) {
    bool result = on_plugin_midi(event); // data1,data2,data3
    if (result) return true;
    else {

      #ifdef SAT_PLUGIN_CONVERT_MIDI

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
        } // switch msg

        return true;

      #else

        return false;

      #endif //convert midi

    } // result

    return false;

  }

  //----------

  bool handleMidiSysexEvent(const clap_event_midi_sysex_t* event) {
    bool result = on_plugin_midiSysex(event);
    if (result) return true;
    return false;
  }

  //----------

  bool handleMidi2Event(clap_event_midi2_t* event) {
    bool result = on_plugin_midi2(event);
    if (result) return true;
    return false;
  }

//------------------------------
private: // queues
//------------------------------

  // parameter, host -> gui
  // (automation, host-provided generic plugin interface)
  //
  // TODO: check for duplicates
  // pop updates to array, check array for duplicates..
  // called from:
  // - SAT_Plugin.handleParamValueEvent

  #if !defined (SAT_GUI_NOGUI)

  void queueParamFromHostToGui(uint32_t AIndex, sat_param_t AValue) {
    //SAT_Print("%i = %f\n",AIndex,AValue);
    SAT_ParamQueueItem item;
    item.type   = CLAP_EVENT_PARAM_VALUE;
    item.index  = AIndex;
    item.value  = AValue;
    if (!MParamFromHostToGuiQueue.write(item)) {
      //SAT_Log("queueParamFromHostToGui: couldn't write to queue\n");
      SAT_Print("queueParamFromHostToGui: couldn't write to queue\n");
    }
  }

  //----------

  // TODO: check for duplicated (param/mod)
  // called from:
  // - SAT_Plugin.do_editorListener_timer

  void flushParamFromHostToGui() {
    uint32_t count = 0;
    //SAT_Print("\n");
    SAT_ParamQueueItem item;
    while (MParamFromHostToGuiQueue.read(&item)) {
      count += 1;
      SAT_Parameter* parameter = MParameters[item.index];

      // parameters are in clap-space
      // widgets are 0..1

      if (MEditor && MEditor->isOpen()) {
        MEditor->updateParameterFromHost(parameter,item.value);
      }
    }
    //if (count > 0) { SAT_Print("flushParamFromHostToGui: %i events\n",count); }
  }

  #endif // nogui

//------------------------------
//
//------------------------------

  // modulation, host -> gui
  // (modulation)
  //
  // called from:
  // - SAT_Plugin.handleParamModEvent

  #if !defined (SAT_GUI_NOGUI)

  void queueModFromHostToGui(uint32_t AIndex, sat_param_t AValue) {
    // todo: is_gui_open?
    //SAT_Print("%i = %f\n",AIndex,AValue);
    SAT_ParamQueueItem item;
    item.type   = CLAP_EVENT_PARAM_MOD;
    item.index  = AIndex;
    item.value  = AValue;
    if (!MModFromHostToGuiQueue.write(item)) {
      //SAT_Log("queueModFromHostToGui: couldn't write to queue\n");
      SAT_Print("queueModFromHostToGui: couldn't write to queue\n");
    }
  }

  //----------

  // TODO: check for duplicates
  // pop updates to array, check array for duplicates..
  // called from:
  // - SAT_Plugin.do_editorListener_timer

  void flushModFromHostToGui() {
    //SAT_Print("\n");
    SAT_ParamQueueItem item;
    uint32_t count = 0;
    while (MModFromHostToGuiQueue.read(&item)) {
      count += 1;
      SAT_Parameter* parameter = MParameters[item.index];
      if (MEditor && MEditor->isOpen()) {
        MEditor->updateModulationFromHost(parameter,item.value);
      }
    }
    //if (count > 0) { SAT_Print("flushModFromHostToGui: %i events\n",count); }
  }

  #endif // nogui

//------------------------------
//
//------------------------------

  // parameter, gui -> audio
  // (tweak knob)
  //
  // called from:
  // - SAT_Plugin.do_editorListener_parameter_update

  #if !defined (SAT_GUI_NOGUI)

  void queueParamFromGuiToAudio(uint32_t AIndex, sat_param_t AValue) {
    //SAT_Print("%i = %f\n",AIndex,AValue);
    SAT_ParamQueueItem item;
    item.type   = CLAP_EVENT_PARAM_VALUE;
    item.index  = AIndex;
    item.value  = AValue;
    if (!MParamFromGuiToAudioQueue.write(item)) {
      //SAT_Log("queueParamFromGuiToAudio: couldn't write to queue\n");
    }
  }

  //----------

  // TODO: check for duplicates? can we have duplicates from gui?
  // called from:
  // - SAT_Plugin.process

  void flushParamFromGuiToAudio() {
    uint32_t count = 0;
    //SAT_Print("\n");
    SAT_ParamQueueItem item;
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
      bool handled          = handleParamValueEvent(&event); // handleParamValueEvent ?
      if (!handled) {
        //TODO
      }
    }
    //if (count > 0) { SAT_Print("flushParamFromGuiToAudio: %i events\n",count); }
  }

  #endif // nogui

//------------------------------
//
//------------------------------

  // parameter, gui -> host
  // (tweak knob)
  //
  // called from:
  // - SAT_Plugin.do_editorListener_parameter_update

  #if !defined (SAT_GUI_NOGUI)

  void queueParamFromGuiToHost(uint32_t AIndex, sat_param_t AValue) {
    //SAT_Print("%i = %f\n",AIndex,AValue);
    SAT_ParamQueueItem item;
    item.type   = CLAP_EVENT_PARAM_VALUE;
    item.index  = AIndex;
    item.value  = AValue;
    if (!MParamFromGuiToHostQueue.write(item)) {
      //SAT_Log("queueParamFromGuiToHost: couldn't write to queue\n");
    }
  }

  //----------

  // TODO: check for duplicates? can we have duplicates from gui?
  // called from:
  // - SAT_Plugin.process()

  void flushParamFromGuiToHost(const clap_output_events_t *out_events) {
    uint32_t count = 0;
    //SAT_Print("\n");
    SAT_ParamQueueItem item;
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

  #endif // nogui

//------------------------------
private:
//------------------------------

  #if !defined (SAT_GUI_NOGUI)

  // don't send ALL param value/mods to gui.. only last one in block
  // set flag, and check at end of process
  //
  // call before processing all events
  
  void clearAutomationToGui() {
    uint32_t num = MParameters.size();
    for (uint32_t i=0; i<num; i++) {
      MParameters[i]->setGuiAutomationDirty(false);
    }
  }
  
  //----------

  void clearModulationToGui() {
    uint32_t num = MParameters.size();
    for (uint32_t i=0; i<num; i++) {
      MParameters[i]->setGuiModulationDirty(false);
    }
  }
  
  //----------
    
  // call after processing all events
  
  void queueAutomationToGui() {
    uint32_t num = MParameters.size();
    for (uint32_t i=0; i<num; i++) {
      if (MParameters[i]->isGuiAutomationDirty()) {
        double value = MParameters[i]->getLastAutomatedValue();
        queueParamFromHostToGui(i,value);
      }
    }
  }

  //----------
    
  void queueModulationToGui() {
    uint32_t num = MParameters.size();
    for (uint32_t i=0; i<num; i++) {
      if (MParameters[i]->isGuiModulationDirty()) {
        double value = MParameters[i]->getLastModulatedValue();
        queueModFromHostToGui(i,value);
      }
    }
  }

  #endif // nogui

//------------------------------
private:
//------------------------------

//------------------------------
public: // process
//------------------------------

  virtual void processEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
    //SAT_PRINT;
    if (!in_events) return;
    //if (!out_events) return;
    #if !defined (SAT_GUI_NOGUI)
    clearAutomationToGui();
    clearModulationToGui();
    #endif
    uint32_t prev_time = 0;
    uint32_t size = in_events->size(in_events);
    for (uint32_t i=0; i<size; i++) {
      const clap_event_header_t* header = in_events->get(in_events,i);
      if (header->space_id == CLAP_CORE_EVENT_SPACE_ID) {
        if (header->time < prev_time) {
          SAT_Print("huh? not sorted? prev_time %i header->time %i header->type %i\n",prev_time,header->time,header->type);
        }
        handleEvent(header);
        prev_time = header->time;
      }
    }
    #if !defined (SAT_GUI_NOGUI)
    queueAutomationToGui();
    queueModulationToGui();
    #endif
  }

  //----------

  virtual void processStereoSample(float* spl0, float* spl1) {
  }

  //----------

  virtual void processAudio(SAT_ProcessContext* AContext, uint32_t AOffset, uint32_t ALength) {
    const clap_process_t* process = AContext->process;
    //float* inputs[2];
    //float* outputs[2];
    //inputs[0]  = process->audio_inputs[0].data32[0] + AOffset;
    //inputs[1]  = process->audio_inputs[0].data32[1] + AOffset;
    //outputs[0] = process->audio_outputs[0].data32[0] + AOffset;
    //outputs[1] = process->audio_outputs[0].data32[1] + AOffset;
    //SAT_CopyStereoBuffer(outputs,inputs,ALength);
    float* input0  = process->audio_inputs[0].data32[0]  + AOffset;
    float* input1  = process->audio_inputs[0].data32[1]  + AOffset;
    float* output0 = process->audio_outputs[0].data32[0] + AOffset;
    float* output1 = process->audio_outputs[0].data32[1] + AOffset;
    for (uint32_t i=0; i<ALength; i++) {
      float spl0 = *input0++;
      float spl1 = *input1++;
      processStereoSample(&spl0,&spl1);
      *output0++ = spl0;
      *output1++ = spl1;
    }
  }

  //----------

  virtual void processAudio(SAT_ProcessContext* AContext, uint32_t AOffset) {
    processAudio(AContext,AOffset,SAT_AUDIO_QUANTIZED_SIZE);
  }

  //----------

  virtual void processAudio(SAT_ProcessContext* AContext) {
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
      //*output0++ = *input0++;
      //*output1++ = *input1++;
      float spl0 = *input0++;
      float spl1 = *input1++;
      processStereoSample(&spl0,&spl1);
      *output0++ = spl0;
      *output1++ = spl1;
    }
  }

  //----------

  // processes events at their sample accurate place, and audio inbetween

  virtual void processAudioInterleaved(SAT_ProcessContext* AContext) {
    const clap_input_events_t* in_events = AContext->process->in_events;
    uint32_t remaining = AContext->process->frames_count;
    uint32_t num_events = in_events->size(in_events);
    uint32_t current_time = 0;
    uint32_t current_event = 0;
    #if !defined (SAT_GUI_NOGUI)
    clearAutomationToGui();
    clearModulationToGui();
    #endif
    while (remaining > 0) {
      if (current_event < num_events) {
        const clap_event_header_t* header = in_events->get(in_events,current_event);
        current_event += 1;
        int32_t length = header->time - current_time;
        // if length > remaining ...
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
    #if !defined (SAT_GUI_NOGUI)
    queueAutomationToGui();
    queueModulationToGui();
    #endif
  }

  //----------

  // split audio block in smaller, regular sizes, and quantize events
  // (process all events 'belonging' to the slice, at the atart ot the slice,
  // and then the audio)..
  // events could be processed up to (slicesize - 1) samples 'early'..

  virtual void processAudioQuantized(SAT_ProcessContext* AContext) {
    uint32_t buffer_length = AContext->process->frames_count;
    uint32_t remaining = buffer_length;
    uint32_t current_time = 0;
    uint32_t current_event = 0;
    uint32_t next_event_time = 0;
    #if !defined (SAT_GUI_NOGUI)
    clearAutomationToGui();
    clearModulationToGui();
    #endif
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
    #if !defined (SAT_GUI_NOGUI)
    queueAutomationToGui();
    queueModulationToGui();
    #endif
  }


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------


//------------------------------
protected: // SAT_EditorListener
//------------------------------

  #if !defined (SAT_GUI_NOGUI)

  void on_editorListener_timer(SAT_Timer* ATimer, double AElapsed) override {
    //SAT_PRINT;
    flushParamFromHostToGui();
    flushModFromHostToGui();
  }

  //----------

  void on_editorListener_update(SAT_Parameter* AParameter, sat_param_t AValue) override {
    uint32_t index = AParameter->getIndex();
    //SAT_Print("from gui.. AParameter %p index %i AValue %f\n",AParameter,index,AValue);
    MParameters[index]->setValue(AValue);
    queueParamFromGuiToAudio(index,AValue);
    queueParamFromGuiToHost(index,AValue);
  }

#endif // nogui


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------


//------------------------------
protected: // clap_plugin
//------------------------------

  bool init() override {

    // #ifdef SAT_DEBUG_WINDOW
    //   MDebugWindow = new SAT_DebugWindow(640,480);
    //   MDebugWindow->setTitle("SAT_DebugWindow");
    //   MDebugWindow->show();
    //   MDebugWindow->startEventThread();
    // #endif

    setDefaultParameterValues();
    MIsInitialized = true;
    return true;
  }

  //----------

  void destroy() override {
    SAT_PRINT;

    // #ifdef SAT_DEBUG_WINDOW
    //   if (MDebugWindow) {
    //     MDebugWindow->stopEventThread();
    //     delete MDebugWindow;
    //   }
    // #endif

    MIsInitialized = false;

    #ifdef SAT_PLUGIN_DELETE_IN_DESTROY
      delete this;
    #endif
    
    SAT_PRINT;

  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) override {
    MProcessContext.plugin    = this;
    MIsActivated              = true;
    MSampleRate               = sample_rate;
    MMinBufferSize            = min_frames_count;
    MMaxBufferSize            = max_frames_count;
    MProcessContext.samplerate = sample_rate;
    MProcessContext.minbufsize = min_frames_count;
    MProcessContext.maxbufsize = max_frames_count;
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
    //MProcessContext.parameters = &MParameters;
    MProcessContext.samplerate = MSampleRate;
    MProcessContext.minbufsize = MMinBufferSize;
    MProcessContext.maxbufsize = MMaxBufferSize;
    MProcessContext.process_counter += 1;
    //MProcessContext.voice_buffer
    //MProcessContext.voice_length

    #if !defined (SAT_GUI_NOGUI)
      flushParamFromGuiToAudio();
    #endif

    //preProcessEvents(process->in_events,process->out_events);

    if (process->transport) handleTransportEvent(process->transport);
    switch (MEventMode) {
      case SAT_PLUGIN_EVENT_MODE_BLOCK:
        processEvents(process->in_events,process->out_events);
        processAudio(&MProcessContext);
        break;
      case SAT_PLUGIN_EVENT_MODE_INTERLEAVED:
        processAudioInterleaved(&MProcessContext);
        break;
      case SAT_PLUGIN_EVENT_MODE_QUANTIZED:
        processAudioQuantized(&MProcessContext);
        break;
    }

    postProcessEvents(process->in_events,process->out_events);

    #if !defined (SAT_GUI_NOGUI)
      flushParamFromGuiToHost(process->out_events);
    #endif

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

  //TODO: fix this

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

  #if !defined (SAT_GUI_NOGUI)

  bool gui_is_api_supported(const char *api, bool is_floating) override {
    //SAT_Print("api %s is_floating %i",api,is_floating);
    #if defined(SAT_GUI_WAYLAND)
      if ((strcmp(api,CLAP_WINDOW_API_WAYLAND) == 0) && (is_floating)) {
        //SAT_DPrint(" -> true\n");
        return true;
      }
    #elif defined(SAT_GUI_WIN32)
      if ((strcmp(api,CLAP_WINDOW_API_WIN32) == 0) && (!is_floating)) {
        //SAT_DPrint(" -> true\n");
        return true;
      }
    #elif defined(SAT_GUI_X11)
      if ((strcmp(api,CLAP_WINDOW_API_X11) == 0) && (!is_floating)) {
        //SAT_DPrint(" -> true\n");
        return true;
      }
    #endif
    //SAT_DPrint(" -> false\n");
    return false;
  }

  //----------

  bool gui_get_preferred_api(const char **api, bool *is_floating) override {
    //SAT_Print("");
    #if defined(SAT_GUI_WAYLAND)
      *api = CLAP_WINDOW_API_WAYLAND;
      *is_floating = true;
      //SAT_DPrint(" -> true (*api %s is_floating %i)\n",*api,*is_floating);
      return true;
    #elif defined(SAT_GUI_WIN32)
      *api = CLAP_WINDOW_API_WIN32;
      *is_floating = false;
      //SAT_DPrint(" -> true (*api %s is_floating %i)\n",*api,*is_floating);
      return true;
    #elif defined(SAT_GUI_X11)
      *api = CLAP_WINDOW_API_X11;
      *is_floating = false;
      //SAT_DPrint(" -> true (*api %s is_floating %i)\n",*api,*is_floating);
      return true;
    #endif
    //SAT_DPrint(" -> false\n");
    return false;
  }

  //----------

  bool gui_create(const char *api, bool is_floating) override {
    MIsEditorClosing = false;

    if (is_floating == true) return false;

    #ifdef SAT_LINUX
      if (strcmp(api,CLAP_WINDOW_API_X11) != 0) return false;
    #endif
    #ifdef SAT_WIN32
      if (strcmp(api,CLAP_WINDOW_API_WIN32) != 0) return false;
    #endif

    #if defined (SAT_GUI_DEFAULT_EDITOR)
    
      // if we haven't set/called setInitialEditorSize, use calculated, generic editor size
      if ((MInitialEditorWidth == 0) || (MInitialEditorHeight == 0)) {
        SAT_Point size = getDefaultEditorSize();
        double scale = 1.0;
        setInitialEditorSize(size.w,size.h,scale);
      }

    #else

      setInitialEditorSize(512,512,1.0);

    #endif

    //uint32_t w = (double)MInitialEditorWidth;// * MInitialEditorScale;
    //uint32_t h = (double)MInitialEditorHeight;// * MInitialEditorScale;

    //SAT_Print("api %s is_floating %i",api,is_floating);
    //MEditor = createEditor(this,w,h,MInitialEditorScale);
    MEditor = createEditor(this,MInitialEditorWidth,MInitialEditorHeight,MInitialEditorScale);
    MEditor->create(api,is_floating);
    if (MEditor) {
      //setupEditor(MEditor);
      //SAT_DPrint(" -> true\n");
      return true;
    }
    //SAT_DPrint(" -> false\n");
    //SAT_Print("MEditor = null\n");
    return false;
  }

  //----------

  void gui_destroy() override {
    MIsEditorClosing = true;
    //SAT_Print("\n");
    if (MEditor) {
      //cleanupEditor(MEditor);
      MEditor->destroy();
      deleteEditor(MEditor);
      MEditor = nullptr;
    }
    //else { SAT_Print("MEditor = null\n"); }
  }

  //----------

  bool gui_set_scale(double scale) override {
    //SAT_Print("scale %f\n",scale);
    if (MEditor) return MEditor->set_scale(scale);
    //SAT_Print("MEditor = null\n");
    return false;
  }

  //----------

  bool gui_get_size(uint32_t *width, uint32_t *height) override {
    bool result = false;
    if (MEditor) result = MEditor->get_size(width,height);
    else {
      *width = MInitialEditorWidth;// * MInitialEditorScale;
      *height = MInitialEditorHeight;// * MInitialEditorScale;
    }
    //SAT_Print("(*width %i *height %i)\n",*width,*height);
    return result;
  }

  //----------

  bool gui_can_resize() override {
    //SAT_Print("\n");
    if (MEditor) return MEditor->can_resize();
    //SAT_Print("MEditor = null\n");
    return false;
  }

  //----------

  bool gui_get_resize_hints(clap_gui_resize_hints_t *hints) override {
    //SAT_Print("\n");
    if (MEditor) return MEditor->get_resize_hints(hints);
    //SAT_Print("MEditor = null\n");
    return false;
  }

  //----------

  bool gui_adjust_size(uint32_t *width, uint32_t *height) override {
    //SAT_Print("(*width %i *height %i)\n",*width,*height);
    if (MEditor) return MEditor->adjust_size(width,height);
    //SAT_Print("MEditor = null\n");
    return false;
  }

  //----------

  bool gui_set_size(uint32_t width, uint32_t height) override {
    //SAT_Print("width %i height %i\n",width,height);
    if (MEditor) return MEditor->set_size(width,height);
    //SAT_Print("MEditor = null\n");
    return false;
  }

  //----------

  bool gui_set_parent(const clap_window_t *window) override {
    //SAT_Print("\n");
    if (MEditor) {
      bool result = MEditor->set_parent(window);
      SAT_Window* window = MEditor->getWindow();
      if (window) setupEditorWindow(MEditor,window);

      initEditorParameterValues();
      
      return result;
    }
    //SAT_Print("MEditor = null\n");
    return false;
  }

  //----------

  bool gui_set_transient(const clap_window_t *window) override {
    //SAT_Print("\n");
    if (MEditor) return MEditor->set_transient(window);
    //SAT_Print("MEditor = null\n");
    return false;
  }

  //----------

  void gui_suggest_title(const char *title) override {
    //SAT_Print("title %s\n",title);
    if (MEditor) MEditor->suggest_title(title);
    //else { SAT_Print("MEditor = null\n"); }
  }

  //----------

  bool gui_show() override {
    //SAT_Print("\n");
    if (MEditor) return MEditor->show();
    //SAT_Print("MEditor = null\n");
    return false;
  }

  //----------

  bool gui_hide() override {
    //SAT_Print("\n");
    if (MEditor) return MEditor->hide();
    //SAT_Print("MEditor = null\n");
    return false;
  }

  #endif // nogui

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
    //note_name->port = -1;
    //note_name->channel = -1;
    //note_name->key = -1;
    //SAT_Strlcpy(note_name->name,"",CLAP_NAME_SIZE);
    return false;
  }

//------------------------------
protected: // note_ports
//------------------------------

  uint32_t note_ports_count(bool is_input) override {
    if (is_input) return MNoteInputPorts.size();
    else return MNoteOutputPorts.size();
  }

  //----------

  bool note_ports_get(uint32_t index, bool is_input, clap_note_port_info_t *info) override {
    const clap_note_port_info_t* port_info = nullptr;
    if (is_input) port_info = MNoteInputPorts[index]->getInfo();
    else port_info = MNoteOutputPorts[index]->getInfo();
    memcpy(info,port_info,sizeof(clap_note_port_info_t));
    return true;
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

  // on main/gui thread?

  void params_flush(const clap_input_events_t *in, const clap_output_events_t *out) override {
    //processEvents(in,out);
    for (uint32_t i=0; i<in->size(in); i++) {
      const clap_event_header_t* header;
      header = in->get(in,i);
      handleEvent(header);
    }
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
    return true;
  }

//------------------------------
protected: // state
//------------------------------

  bool state_save(const clap_ostream_t *stream) override {
    //uint32_t total = 0;
    uint32_t written = 0;
    uint32_t version = 0;
    uint32_t num_params = MParameters.size();
    // version
    written = stream->write(stream,&version,sizeof(uint32_t));
    if (written != sizeof(uint32_t)) {
      //SAT_Print("state_save: error writing version\n");
      return false;
    }
    //total += sizeof(uint32_t);
    // num params
    written = stream->write(stream,&num_params,sizeof(uint32_t));
    if (written != sizeof(uint32_t)) {
      //SAT_Print("state_save: error writing parameter count\n");
      return false;
    }
    //total += sizeof(uint32_t);
    // param values
    for (uint32_t i=0; i<num_params; i++) {
      double value = MParameters[i]->getValue();
      //double value = MParameterValues[i];
      written = stream->write(stream,&value,sizeof(double));
      if (written != sizeof(double)) {
        //SAT_Print("state_load: error writing parameter %i\n",i);
        return false;
      }
      //total += sizeof(double);
    }
    //SAT_Print("total: %i\n",total);
    return true;
  }

  //----------

  bool state_load(const clap_istream_t *stream) override {
    //uint32_t total = 0;
    uint32_t read = 0;
    uint32_t version = 0;
    uint32_t num_params = 0;
    // version
    read = stream->read(stream,&version,sizeof(uint32_t));
    if (read != sizeof(uint32_t)) {
      //SAT_Print("state_load: error reading version\n");
      return false;
    }
    //total += sizeof(uint32_t);
    //TODO: check version
    // num params
    read = stream->read(stream,&num_params,sizeof(uint32_t));
    if (read != sizeof(uint32_t)) {
      //SAT_Print("state_load: error reading parameter count\n");
      return false;
    }
    //total += sizeof(uint32_t);
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
      //total += sizeof(double);
      
      MParameters[i]->setValue(value);    // !!!!!!!!!!!!!!!!!!!!
      
      //MParameterValues[i] = value;
    }
    //SAT_Print("total: %i\n",total);
    //updateParameterValues();
    //updateEditorParameterValues();
    return true;
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
    //info->voice_count     = 16;
    //info->voice_capacity  = 16;
    //info->flags           = CLAP_VOICE_INFO_SUPPORTS_OVERLAPPING_NOTES;
    //return true;
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
    //return true;
    return false;
  }
  
//------------------------------
protected: // draft: param_indication
//------------------------------

  void param_indication_set_mapping(clap_id param_id, bool has_mapping, const clap_color_t *color, const char *label, const char *description) override {
    SAT_Parameter* param = MParameters[param_id];
//    param->setMappingIndication(has_mapping,color,label,description);
    param->setIsMapped(has_mapping);
    param->setMappedColor(SAT_Color(color->red,color->green,color->blue,color->alpha));
    #if !defined (SAT_GUI_NOGUI)
      SAT_Widget* widget = (SAT_Widget*)param->getWidget();
      if (widget && MEditor && MEditor->isOpen()) widget->do_widget_redraw(widget);
    #endif
  }
  
  //----------



  void param_indication_set_automation(clap_id param_id, uint32_t automation_state, const clap_color_t *color) override {
    SAT_Parameter* param = MParameters[param_id];
//    param->setAutomationIndication(automation_state,color);
    //param->setAutomationIndication(automation_state,color);
    param->setAutomationState(automation_state);
    param->setAutomationColor(SAT_Color(color->red,color->green,color->blue,color->alpha));
    #if !defined (SAT_GUI_NOGUI)
      SAT_Widget* widget = (SAT_Widget*)param->getWidget();
      if (widget && MEditor && MEditor->isOpen()) widget->do_widget_redraw(widget);
    #endif
  }
  
//------------------------------
protected: // draft: preset_load
//------------------------------

  bool preset_load_from_location(uint32_t location_kind, const char *location, const char *load_key) override {

    const clap_host_t* host = MHost->getHost();
    //const clap_host_preset_load_t* preset_load = (const clap_host_preset_load_t*)host->get_extension(host,CLAP_EXT_PRESET_LOAD);

    //return loadPresetFromFile(location,load_key);
    switch (location_kind) {
      case CLAP_PRESET_DISCOVERY_LOCATION_FILE: {
        SAT_Print("CLAP_PRESET_DISCOVERY_LOCATION_FILE location '%s', load_key '%s'\n",location,load_key);
        if (MHost->ext.preset_load) MHost->ext.preset_load->loaded(host,location_kind,location,load_key);
        return true;
      }
      case CLAP_PRESET_DISCOVERY_LOCATION_PLUGIN: {
        SAT_Print("CLAP_PRESET_DISCOVERY_LOCATION_PLUGIN location '%s', load_key '%s'\n",location,load_key);
        if (MHost->ext.preset_load) MHost->ext.preset_load->loaded(host,location_kind,location,load_key);
        return true;
      }
      default: {
        SAT_Print("unknown location kind (%i : '%s','%s')\n",location_kind,location,load_key);
        return false;
      }
    }
    return false;
  }
  
//------------------------------
protected: // draft: remote_controls
//------------------------------

  uint32_t remote_controls_count() override {
    return 1;
  }
  
  //----------

  bool remote_controls_get(uint32_t page_index, clap_remote_controls_page_t *page) override {
    SAT_Strlcpy(page->page_name,"Perform",CLAP_NAME_SIZE);
    SAT_Strlcpy(page->section_name,"",CLAP_NAME_SIZE);
    page->page_id = 0;
    page->is_for_preset = false;
    uint32_t numpar = MParameters.size();
    for (uint32_t i=0; i<8; i++) {
      if (i < numpar) page->param_ids[i] = i;// CLAP_INVALID_ID;
      else page->param_ids[i] = CLAP_INVALID_ID;
    }
    return true;
  }
  
//------------------------------
protected: // draft: resource_directory
//------------------------------

  void resource_directory_set_directory(const char *path, bool is_shared) override {
    MResourceDirectory = path;
    MResourceDirectoryShared = is_shared;
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

  void track_info_changed() override {
    const clap_host_t* host = getClapHost();
    //const clap_host_track_info_t* track_info = (const clap_host_track_info_t*)host->get_extension(host,CLAP_EXT_TRACK_INFO);
    clap_track_info_t info;
    //if (track_info->get(host,&info)) {
    if (MHost->ext.track_info) {
      if (MHost->ext.track_info->get(host,&info)) {
        MTrackFlags = info.flags;
        if (info.flags & CLAP_TRACK_INFO_HAS_TRACK_NAME) {
          SAT_Strlcpy(MTrackName,info.name,CLAP_NAME_SIZE);
          SAT_Print("%s\n",MTrackName);
        }
        if (info.flags & CLAP_TRACK_INFO_HAS_TRACK_COLOR) {
          MTrackColor = SAT_Color(info.color.red,info.color.green,info.color.blue,info.color.alpha);
        }
        if (info.flags & CLAP_TRACK_INFO_HAS_AUDIO_CHANNEL) {
          MTrackChannelCount = info.audio_channel_count;
          MTrackPortType = info.audio_port_type;
        }
        MTrackIsReturnTrack = ((info.flags & CLAP_TRACK_INFO_IS_FOR_RETURN_TRACK)  != 0);
        MTrackIsBus         = ((info.flags & CLAP_TRACK_INFO_IS_FOR_BUS)           != 0);
        MTrackIsMaster      = ((info.flags & CLAP_TRACK_INFO_IS_FOR_MASTER)        != 0);

        //SAT_Widget* widget = (SAT_Widget*)param->getWidget();
        //if (widget && MEditor && MEditor->isOpen()) widget->do_widget_redraw(widget);

      }
    }
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

