#ifndef sat_plugin_included
#define sat_plugin_included
//----------------------------------------------------------------------

#include "sat.h"

#include "plugin/sat_audio_port.h"
#include "plugin/sat_editor.h"
#include "plugin/sat_host.h"
#include "plugin/sat_note_port.h"
#include "plugin/sat_parameter.h"
#include "plugin/sat_plugin_queues.h"
#include "plugin/sat_preset.h"
#include "plugin/sat_processor.h"

#include "plugin/editor/sat_editor_listener.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/clap/sat_clap_plugin.h"

#include "plugin/processor/sat_process_context.h"
#include "plugin/processor/sat_processor_owner.h"

// #include "plugin/processor/sat_block_processor.h"
// #include "plugin/processor/sat_interleaved_processor.h"
// #include "plugin/processor/sat_quantized_processor.h"
// #include "plugin/processor/sat_voice_processor.h"

#ifndef SAT_NO_GUI
#ifdef SAT_EDITOR_EMBEDDED
  #include "gui/sat_window.h"
  #include "gui/sat_widgets.h"
#endif
#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Plugin
: public SAT_ClapPlugin
, public SAT_EditorListener
, public SAT_ProcessorOwner {

//------------------------------
private:
//------------------------------

  bool                    MIsInitialized        = false;
  bool                    MIsActivated          = false;
  bool                    MIsProcessing         = false;
  double                  MSampleRate           = 0.0;
  uint32_t                MMinFramesCount       = 0;
  uint32_t                MMaxFramesCount       = 0;
  clap_plugin_render_mode MRenderMode           = CLAP_RENDER_REALTIME;

  SAT_AudioPortArray      MAudioInputPorts      = {};
  SAT_AudioPortArray      MAudioOutputPorts     = {};
  SAT_NotePortArray       MNoteInputPorts       = {};
  SAT_NotePortArray       MNoteOutputPorts      = {};
  SAT_ParameterArray      MParameters           = {};
  SAT_ClapExtensions      MSupportedExtensions  = {};

  SAT_Host*               MHost                 = {};
  SAT_Processor*          MProcessor            = nullptr;
  SAT_ProcessContext      MProcessContext       = {};
  SAT_PluginQueues        MQueues               = {};

  SAT_Editor*             MEditor               = nullptr;
  uint32_t                MInitialEditorWidth   = 400;
  uint32_t                MInitialEditorHeight  = 300;
  double                  MInitialEditorScale   = 1.0;
  bool                    MProportionalEditor   = false;
  
//double                  MEditorAspectRatio    = 1.0;
//sat_atomic_bool_t       MIsEditorClosing      { false };

//------------------------------
protected:
//------------------------------

//------------------------------
public:
//------------------------------

  SAT_Plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_ClapPlugin(ADescriptor,AHost) {
    // MHost = new SAT_Host(AHost); // -> .init()
  }

  //----------

  virtual ~SAT_Plugin() {
    //SAT_PRINT("MProcessor %p\n",MProcessor);
    if (MProcessor) delete MProcessor;
    //MProcessor = nullptr;
    #ifndef SAT_NO_AUTODELETE
      deleteAudioInputPorts();
      deleteAudioOutputPorts();
      deleteNoteInputPorts();
      deleteNoteOutputPorts();
      deleteParameters();
    #endif
    //if (MHost) delete MHost; // -> .destroy()
  }

//------------------------------
public:
//------------------------------

  SAT_Host* getHost() {
    return MHost;
  }

//------------------------------
public: // extensions
//------------------------------

  void registerExtension(const char* AId) {
    int32_t index = findExtension(AId);
    if (index >=0) {
      const void* ptr = getExtensionPtr(index);
      if (!MSupportedExtensions.hasItem(AId)) MSupportedExtensions.addItem(AId,ptr);
    }
  }

  //----------

  void unregisterExtension(const char* AId) {
    if (MSupportedExtensions.hasItem(AId)) {
      MSupportedExtensions.removeItem(AId);
    }
  }

  //----------

  void registerAllExtensions() {
    for (uint32_t i=0; i<getNumExtensions(); i++) {
      const char* id = getExtensionId(i);
      const void* ptr = getExtensionPtr(i);
      MSupportedExtensions.addItem(id,ptr);
    }
  }

  //----------

  void registerDefaultExtensions() {
    registerExtension(CLAP_EXT_AUDIO_PORTS);
    #ifndef SAT_NO_GUI
      registerExtension(CLAP_EXT_GUI);
    #endif
    registerExtension(CLAP_EXT_PARAMS);
    registerExtension(CLAP_EXT_STATE);
  }

  //----------

  void registerDefaultSynthExtensions() {
    registerDefaultExtensions();
    registerExtension(CLAP_EXT_NOTE_PORTS);
    registerExtension(CLAP_EXT_THREAD_POOL);
    registerExtension(CLAP_EXT_VOICE_INFO);
  }

//------------------------------
public: // audio input ports
//------------------------------

  void appendAudioInputPort(const clap_audio_port_info_t* APort) {
  }

  //----------

  void appendAudioInputPort(SAT_AudioPort* APort) {
    MAudioInputPorts.append(APort);
  }

  //----------

  void appendStereoAudioInputPort(const char* AName="Audio In", uint32_t AId=0) {
    //SAT_PRINT("AName %s AId %i\n",AName,AId);
    SAT_AudioPort* port = new SAT_AudioPort(AName,AId);
    MAudioInputPorts.append(port);
  }

  //----------

  void deleteAudioInputPorts() {
    for (uint32_t i=0; i<MAudioInputPorts.size(); i++) {
      delete MAudioInputPorts[i];
    }
  }

//------------------------------
public: // audio output ports
//------------------------------

  void appendAudioOutputPort(const clap_audio_port_info_t* APort) {
  }

  //----------

  void appendAudioOutputPort(SAT_AudioPort* APort) {
    MAudioOutputPorts.append(APort);
  }

  //----------

  void appendStereoAudioOutputPort(const char* AName="Audio Out", uint32_t AId=0) {
    //SAT_PRINT("AName %s AId %i\n",AName,AId);
    SAT_AudioPort* port = new SAT_AudioPort(AName,AId);
    MAudioOutputPorts.append(port);
  }

  //----------

  void deleteAudioOutputPorts() {
    for (uint32_t i=0; i<MAudioOutputPorts.size(); i++) {
      delete MAudioOutputPorts[i];
    }
  }

//------------------------------
public: // note input ports
//------------------------------

  void appendNoteInputPort(const clap_audio_port_info_t* APort) {
  }

  //----------

  void appendNoteInputPort(SAT_AudioPort* APort) {
  }

  //----------

  void appendClapNoteInputPort(const char* AName="Note In", uint32_t AId=0) {
    SAT_NotePort* port = new SAT_NotePort(AName,AId);
    MNoteInputPorts.append(port);
  }

  //----------

  void appendMidiNoteInputPort(const char* AName="Note In", uint32_t AId=0) {
  }

  //----------

  void deleteNoteInputPorts() {
    for (uint32_t i=0; i<MNoteInputPorts.size(); i++) {
      delete MNoteInputPorts[i];
    }
  }

//------------------------------
public: // note output ports
//------------------------------

  void appendNoteOutputPort(const clap_audio_port_info_t* APort) {
  }

  //----------

  void appendNoteOutputPort(SAT_AudioPort* APort) {
  }

  //----------

  void appendClapNoteOutputPort(const char* AName="Note Out", uint32_t AId=0) {
    SAT_NotePort* port = new SAT_NotePort(AName,AId);
    MNoteOutputPorts.append(port);
  }

  //----------

  // void appendMidiNoteOutputPort(const char* AName="Note Out", uint32_t AId=0) {
  // }

  //----------

  void deleteNoteOutputPorts() {
    for (uint32_t i=0; i<MNoteOutputPorts.size(); i++) {
      delete MNoteOutputPorts[i];
    }
  }

//------------------------------
public: // parameters
//------------------------------

  SAT_Parameter* appendParameter(clap_param_info_t* AInfo) {
    SAT_Parameter* parameter = new SAT_Parameter(AInfo);
    return appendParameter(parameter);
  }

  //----------

  SAT_Parameter* appendParameter(SAT_Parameter* AParameter) {
    uint32_t index = MParameters.size();
    AParameter->setIndex(index);
    MParameters.append(AParameter);
    return AParameter;
  }

  //----------

  SAT_Parameter* appendParameter(/*uint32_t AId=0,*/ const char* AName, const char* AModule="", sat_param_t AValue=0.0, sat_param_t AMinValue=0.0, sat_param_t AMaxValue=1.0, uint32_t AFlags=CLAP_PARAM_IS_AUTOMATABLE) {
    SAT_Parameter* param = new SAT_Parameter(/*AId,*/AName,AModule,AValue,AMinValue,AMaxValue,AFlags);
    uint32_t index = MParameters.size();
    param->setIndex(index);
    MParameters.append(param);
    return param;
  }

  //----------

  void appendParameters(SAT_ParameterArray* AParameters) {
    for (uint32_t i=0; i<AParameters->size(); i++) {
      SAT_Parameter* param = AParameters->getItem(i);
      appendParameter(param);
    }
  }

  //----------

  void appendParameters(clap_param_info_t* AInfo, uint32_t ACount) {
    for (uint32_t i=0; i<ACount; i++) {
      appendParameter( &AInfo[i] );
    }
  }

  //----------

  void deleteParameters() {
    for (uint32_t i=0; i<MParameters.size(); i++) {
      delete MParameters[i];
    }
  }

  //----------

  uint32_t getNumParameters() {
    return MParameters.size();
  }

  //----------

  SAT_Parameter* findParameter(const char* AName) {
    for (uint32_t i=0; i<getNumParameters(); i++) {
      SAT_Parameter* param = getParameter(i);
      if (param) {
        clap_param_info_t* info = param->getInfo();
        const char* name = info->name;
        if (name) {
          if (strcmp(AName,name) == 0) {
            return param;
          }
        }
      }
    }
    return nullptr;
  }

  //----------

  SAT_Parameter* getParameter(uint32_t AIndex) {
    return MParameters[AIndex];;
  }

  //----------

  SAT_ParameterArray* getParameters() {
    return &MParameters;
  }

  //----------

  virtual void setDefaultParameterValues() {
    if (MProcessor) {
      uint32_t num = MParameters.size();
      for (uint32_t i=0; i<num; i++) {
        double value = MParameters[i]->getDefaultValue();
        MParameters[i]->setValue(value);
        //bool processParamValueEvent(clap_event_param_value_t* event);
        //return paramValueEvent(event);
        clap_event_param_value_t event;
        event.header.size     = sizeof(clap_event_param_value_t);
        event.header.time     = 0;
        event.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
        event.header.type     = CLAP_EVENT_PARAM_VALUE;
        event.header.flags    = 0; // CLAP_EVENT_IS_LIVE, CLAP_EVENT_DONT_RECORD
        event.param_id        = i;
        event.cookie          = nullptr; // set?
        event.note_id         = -1;
        event.port_index      = -1;
        event.channel         = -1;
        event.key             = -1;
        event.value           = value;
        MProcessor->processParamValueEvent(&event);
      }
    }
  }

  //----------

  #ifndef SAT_NO_GUI

  virtual void setEditorParameterValues() {
    if (MEditor) {
      uint32_t num = MParameters.size();
      for (uint32_t i=0; i<num; i++) {
        SAT_Parameter* param = MParameters[i];
        MEditor->setParameterValue(param);
      }
    }
  }

  #endif

//------------------------------
public: // presets
//------------------------------

  virtual bool savePresetToFile(const char* ALocation, const char* AKey) {
    return false;
  }

  //----------

  virtual bool loadPresetFromFile(const char* ALocation, const char* AKey) {
    SAT_PRINT("location: %s, key: %s\n",ALocation,AKey);
    
    //return false;
    char line_buffer[256] = {0};
    SAT_File file = {};
    if (!file.exists(ALocation)) {
      SAT_PRINT("Error! '%s' does not exist\n",ALocation);
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
      //    SAT_PRINT("%s\n",line_buffer);
      //    ptr = SAT_HexDecode(ptr,line_buffer,32); // num bytes
      //  }
      //}
      //sat_param_t* param_ptr = (sat_param_t*)ptr;
      //uint32_t num_params = getNumParameters();
      //for (uint32_t i=0; i<num_params; i++) {
      //  sat_param_t value = *param_ptr++;
      //  SAT_PRINT("%i : %f\n",i,value);
      //}
      //setAllParameters(param_buffer);
      // on_plugin_loadPreset(metadata,param_buffer);
    
      // ascii
    
      uint32_t i = 0;
      while (file.readLine(line_buffer,256)) {
        if (line_buffer[strlen(line_buffer)-1] == '\n') line_buffer[strlen(line_buffer)-1] = 0;
        if (line_buffer[0] != 0) {
    
          SAT_PRINT("line %i: '%s'\n",i,line_buffer);
    
          //ptr = SAT_HexDecode(ptr,line_buffer,32); // num bytes
          //setParameterValue(i,v);
    
          //double v = atof(line_buffer);
          //SAT_PRINT("%i = %f\n",i,v);
    
          i += 1;
        }
      }
      file.close();
    }
    else {
      SAT_PRINT("Error opening file '%s'\n",ALocation);
      return false;
    }
    return true;
  }

//------------------------------
public: // editor
//------------------------------

  void setInitialEditorSize(uint32_t AWidth, uint32_t AHeight, double AScale=1.0, bool AProportional=false) {
    //SAT_PRINT("AWidth %i AHeight %i AScale %.3f\n",AWidth,AHeight,AScale);
    MInitialEditorWidth = AWidth;
    MInitialEditorHeight = AHeight;
    MInitialEditorScale = AScale;
    MProportionalEditor = AProportional;
    // if ((AWidth > 0) && (AHeight > 0)) {
    //   MEditorAspectRatio = AWidth / AHeight;
    // }
  }

  //----------

  // called by on_EditorListener_createWindow by SAT_EmbeddedEditor.create()
  // override this to create your own window

  #ifndef SAT_NO_GUI
    #ifdef SAT_EDITOR_EMBEDDED

      virtual SAT_Window* createWindow(uint32_t AWidth, uint32_t AHeight) {
        return new SAT_Window(AWidth,AHeight);
      }

      // called by on_EditorListener_createWindow by SAT_EmbeddedEditor.destroy()

      virtual void deleteWindow(SAT_Window* AWindow) {
        delete AWindow;
      }

    #endif // embedded
  #endif // no gui

  //----------

  #ifndef SAT_NO_GUI

    // called by SAT_Plugin.init()
    // override this to create your own editor

    virtual SAT_Editor* createEditor(SAT_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight, double AScale=1.0, bool AProportional=false) {
      return new SAT_Editor(AListener,AWidth,AHeight,AScale,AProportional);
    }

    //----------

    // called by SAT_Plugin.destroy()

    virtual void deleteEditor(SAT_Editor* AEditor) {
      if (AEditor) delete AEditor;
    }

    //----------

    // called by SAT_Plugin.gui_create()
    // called when the editor (and its window, if embedded) has been created..

    virtual bool setupEditor(SAT_Editor* AEditor) {
      // //SAT_TRACE;
      // SAT_Window* window = AEditor->getWindow();
      // // root
      // SAT_RootWidget* root = new SAT_RootWidget( window, SAT_Rect() );
      // window->setRootWidget(root);
      // // default editor
      // for (uint32_t i=0; i<getNumParameters(); i++) {
      //   double x = 10;
      //   double y = (10 + (i * 35));
      //   double w = 300;
      //   double h = 30;
      //   SAT_DragValueWidget* widget = new SAT_DragValueWidget(SAT_Rect(x,y,w,h));
      //   root->appendChild(widget);
      //   AEditor->connect(widget,getParameter(i));
      // }
      return true;
    }

    //----------

    // called by SAT_Plugin.gui_create()
    // after setupEditor()

    virtual bool setupOverlay(SAT_Editor* AEditor) {
      SAT_TRACE;
      return false;
    }


    //----------

    // called by SAT_Plugin.gui_destroy()
    // just before editor is being destroyed

    virtual void cleanupEditor(SAT_Editor* AEditor) {
      //SAT_TRACE;
    }

  #endif // no gui

//------------------------------
public: // editor listener
//------------------------------

  #ifndef SAT_NO_GUI

    // audio queue is flushed at the start of next process
    // host queue is flushed at the end of next process
    //
    // value is in widget-space
    // todo: convert to parameter-space (denormalize)

    void on_EditorListener_update(uint32_t AIndex, sat_param_t AValue) override {
      //SAT_PRINT("AIndex %i AValue %.3f\n",AIndex,AValue);
      SAT_Parameter* param = getParameter(AIndex);
      if (param) {
        sat_param_t value = param->denormalize(AValue);
        //SAT_PRINT("AValue %.3f denormalized value %.3f\n",AValue,value);
        MQueues.queueParamFromGuiToHost(AIndex,value);
        MQueues.queueParamFromGuiToAudio(AIndex,value);
        // prematurely?
        // to keep everything in sync, should we set this when flushing (to audio) ?
        MParameters[AIndex]->setValue(value);
      }

    }

    //----------

    void on_EditorListener_timer(SAT_Timer* ATimer, double ADelta) override {
      //SAT_TRACE;
      MQueues.flushParamFromHostToGui(&MParameters,MEditor);
      MQueues.flushModFromHostToGui(&MParameters,MEditor);
    }

    //----------

    #ifdef SAT_EDITOR_EMBEDDED

    SAT_Window* on_EditorListener_createWindow(SAT_WindowListener* AListener, uint32_t AWidth, uint32_t AHeight) override {
      //SAT_PRINT("AWidth %i AHeight %i\n",AWidth,AHeight);
      SAT_Window* window = createWindow(AWidth,AHeight);
      window->setInitialSize(MInitialEditorWidth,MInitialEditorHeight,MInitialEditorScale,MProportionalEditor);
      return window;
    }

    //----------

    void on_EditorListener_deleteWindow(SAT_Window* AWindow) override {
      deleteWindow(AWindow);
    }

    #endif

  #endif // gui

//------------------------------
public: // processor
//------------------------------

  virtual void setProcessor(SAT_Processor* AProcessor) {
    //SAT_PRINT("AProcessor %p\n",AProcessor);
    MProcessor = AProcessor;
  }

  //----------

  // virtual void deleteProcessor(SAT_Processor* AProcessor) {
  //   if (AProcessor) delete AProcessor;
  // }

  //----------

  // void createProcessor(uint32_t AProcessor) {
  //   SAT_Processor* processor = nullptr;
  //   switch (AProcessor) {
  //     case SAT_PLUGIN_BLOCK_PROCESSOR:        processor = new SAT_BlockProcessor(this); break;
  //     case SAT_PLUGIN_INTERLEAVED_PROCESSOR:  processor = new SAT_InterleavedProcessor(this); break;
  //     case SAT_PLUGIN_QUANTIZED_PROCESSOR:    processor = new SAT_QuantizedProcessor(this); break;
  //     case SAT_PLUGIN_VOICE_PROCESSOR:        processor = new SAT_VoiceProcessor(this); break;
  //   }
  //   if (!processor) processor = new SAT_Processor(this);
  //   SAT_Assert(processor);
  //   MProcessor = processor;
  // }

//------------------------------
public: // processor owner
//------------------------------

  SAT_AudioPortArray* on_processorOwner_getAudioInputPorts()  final { return &MAudioInputPorts; }
  SAT_AudioPortArray* on_processorOwner_getAudioOutputPorts() final { return &MAudioOutputPorts; }
  SAT_NotePortArray*  on_processorOwner_getNoteInputPorts()   final { return &MNoteInputPorts; }
  SAT_NotePortArray*  on_processorOwner_getNoteOutputPorts()  final { return &MNoteOutputPorts; }
  SAT_ParameterArray* on_processorOwner_getParameters()       final { return &MParameters; }

  // a parameter has changed in process()
  // tell the editor about it
  //
  // value is in clap-space
  // todo: convert to widget-space (normalize)

  void on_processorOwner_updateParamFromHostToGui(uint32_t AIndex, sat_param_t AValue) final {
    //SAT_PRINT("%i = %.3f\n",AIndex,AValue);
    #ifndef SAT_NO_GUI
      if (MEditor) {
        SAT_Parameter* param = getParameter(AIndex);
        if (param) {
          #ifdef SAT_WINDOW_TIMER_REFRESH_WIDGETS
            uint32_t index = param->getIndex();
            sat_param_t value = param->normalize(AValue);
            MQueues.queueParamFromHostToGui(index,value);
          #else
            MEditor->updateParameterFromHost(param, AValue);
          #endif

        }
      }
    #endif
  }

  // modulation has changed i process()
  // tell the editor about it

  void on_processorOwner_updateModFromHostToGui(uint32_t AIndex, sat_param_t AValue) final {
    //SAT_PRINT("%i = %.3f\n",AIndex,AValue);
    #ifndef SAT_NO_GUI
      if (MEditor) {
        SAT_Parameter* param = getParameter(AIndex);
        if (param) {
          #ifdef SAT_WINDOW_TIMER_REFRESH_WIDGETS
            uint32_t index = param->getIndex();
            sat_param_t value = param->normalize(AValue);
            MQueues.queueModFromHostToGui(index,value);
          #else
            MEditor->updateModulationFromHost(param, AValue);
          #endif
        }
      }
    #endif
  }

//----------------------------------------------------------------------
public: // clap plugin
//----------------------------------------------------------------------

  bool init() override {
    //SAT_TRACE;
    MHost = new SAT_Host(getClapHost());
    setDefaultParameterValues();
    #ifndef SAT_NO_GUI
      MEditor = createEditor(this,MInitialEditorWidth,MInitialEditorHeight,MInitialEditorScale,MProportionalEditor);
      SAT_Assert(MEditor);
    #endif
    MIsInitialized = true;    
    return true;
  }

  //----------

  void destroy() override {
    //SAT_TRACE;
    if (MHost) delete MHost;
    #ifndef SAT_NO_GUI
      deleteEditor(MEditor);
      MEditor = nullptr;
    #endif
    MIsInitialized = false;
    delete this;
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) override {
    //SAT_PRINT("sample_rate %.2f min_frames %i max_frames %i\n",sample_rate,min_frames_count,max_frames_count);
    MIsActivated                    = true;
    MSampleRate                     = sample_rate;
    MMinFramesCount                 = min_frames_count;
    MMaxFramesCount                 = max_frames_count;
    MProcessContext.samplerate      = sample_rate;
    MProcessContext.minframescount  = min_frames_count;
    MProcessContext.maxframescount  = max_frames_count;

    // setDefaultParameterValues();
    // #ifndef SAT_NO_GUI
    //   MEditor = createEditor(this,MInitialEditorWidth,MInitialEditorHeight,MInitialEditorScale,MProportionalEditor);
    //   SAT_Assert(MEditor);
    // #endif

    return true;
  }

  //----------

  void deactivate() override {
    //SAT_TRACE;
    MIsActivated = false;    
    // #ifndef SAT_NO_GUI
    //   deleteEditor(MEditor);
    //   MEditor = nullptr;
    // #endif
  }

  //----------

  bool start_processing() override {
    //SAT_TRACE;
    MIsProcessing = true;
    return true;
  }

  //----------

  void stop_processing() override {
    //SAT_TRACE;
    MIsProcessing = false;    
  }

  //----------

  void reset() override {
    //SAT_TRACE;
    MProcessContext.process_counter = 0;
    MProcessContext.sample_counter = 0;
  }

  //----------

  clap_process_status process(const clap_process_t *process) override {
    //SAT_TRACE;
    MProcessContext.process = process;
    MProcessContext.parameters = &MParameters;
    MProcessContext.samplerate = MSampleRate;
    MProcessContext.minframescount = MMinFramesCount;
    MProcessContext.maxframescount = MMaxFramesCount;

    if (process->transport) MProcessor->processTransportEvent(process->transport);

    #ifndef SAT_NO_GUI
      MQueues.flushParamFromGuiToAudio(MProcessor);
    #endif
    
    MProcessor->preProcessEvents(process->in_events,process->out_events);
    MProcessor->process(&MProcessContext);
    MProcessor->postProcessEvents(process->in_events,process->out_events);

    MQueues.flushNoteEndsFromAudioToHost(&MProcessContext);

    #if !defined (SAT_NO_GUI)
      MQueues.flushParamFromGuiToHost(&MProcessContext);
    #endif

    MProcessContext.process_counter += 1;
    MProcessContext.sample_counter += process->frames_count;
    return CLAP_PROCESS_CONTINUE;
  }

  //----------

  const void* get_extension(const char *id) override {
    
    if (MSupportedExtensions.hasItem(id)) {
      //SAT_PRINT("supported extension '%s'\n",id);
      return MSupportedExtensions.getItem(id);
    }
    const char* compat_id = findCompatExtension(id);
    if (compat_id) {
      if (MSupportedExtensions.hasItem(compat_id)) {
        //SAT_PRINT("unsupported '%s', compatible '%s'\n",id, compat_id);
        return MSupportedExtensions.getItem(compat_id);
      }
      else {
        //SAT_PRINT("unsupported '%s', compatible '%s' (unsupported)\n",id, compat_id);
        return nullptr;
      }
    }
    //SAT_PRINT("unsupported '%s'\n",id);
    return nullptr;
  }

  //----------

  void on_main_thread() override {
    //SAT_TRACE;
  }

//----------------------------------------------------------------------
public: // clap extensions
//----------------------------------------------------------------------

  // ambisonic
  //------------------------------

  bool ambisonic_is_config_supported(const clap_ambisonic_config_t *config) override {
    return false;
  }

  //----------

  bool ambisonic_get_config(bool is_input, uint32_t port_index, clap_ambisonic_config_t *config) override {
    return false;
  }

  // audio ports
  //------------------------------

  uint32_t audio_ports_count(bool is_input) override {
    //SAT_PRINT("is_input %i\n",is_input);
    uint32_t num;
    if (is_input) num = MAudioInputPorts.size();
    else num = MAudioOutputPorts.size();
    return num;
  }

  //----------

  bool audio_ports_get(uint32_t index, bool is_input, clap_audio_port_info_t *info) override {
    //SAT_PRINT("is_input %i index %i info %p\n",is_input,index,info);
    void* dst = info;
    uint32_t size = sizeof(clap_audio_port_info_t);
    void* src;
    if (is_input) src = MAudioInputPorts[index]->getInfo();
    else src = MAudioOutputPorts[index]->getInfo();
    // SAT_PRINT("dst %p src %p size %p\n",dst,src,size);
    memcpy(dst,src,size);
    return true;
  }

  // audio ports activation
  //------------------------------

  bool audio_ports_activation_can_activate_while_processing() override {
    return false;
  }

  //----------

  bool audio_ports_activation_set_active(bool is_input, uint32_t port_index, bool is_active, uint32_t sample_size) override {
    return false;
  }

  // audio ports config
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

  // configurable audio ports
  //------------------------------

  bool configurable_audio_ports_can_apply_configuration(const struct clap_audio_port_configuration_request *requests, uint32_t request_count) override {
    return false;
  }

  //----------

  bool configurable_audio_ports_apply_configuration(const struct clap_audio_port_configuration_request *requests, uint32_t request_count) override {
    return false;
  }

  // context menu
  //------------------------------

  bool context_menu_populate(const clap_context_menu_target_t  *target, const clap_context_menu_builder_t *builder) override {
    return false;
  }

  //----------

  bool context_menu_perform(const clap_context_menu_target_t *target, clap_id action_id) override {
    return false;
  }

  // gui
  //------------------------------

  #ifndef SAT_NO_GUI

  bool gui_is_api_supported(const char *api, bool is_floating) override {
    //SAT_PRINT("api %s floating %i\n",api,is_floating);
    if (MEditor) return MEditor->isApiSupported(api,is_floating);
    return false;
  }

  //----------

  bool gui_get_preferred_api(const char **api, bool *is_floating) override {
    //SAT_PRINT("\n");
    if (MEditor) return MEditor->getPreferredApi(api,is_floating);
    return false;
  }

  //----------

  bool gui_create(const char *api, bool is_floating) override {
    //SAT_PRINT("api %s floating %i\n",api,is_floating);
    // MIsEditorClosing = false;
    if (MEditor) {
      if (MEditor->create(api,is_floating)) {
        if (setupEditor(MEditor)) {
          MEditor->setupOverlay();
          bool result = setupOverlay(MEditor);
          setEditorParameterValues();
          return true;
        }
      }
    }
    return false;
  }

  //----------

  void gui_destroy() override {
    //SAT_PRINT("\n");
    // MIsEditorClosing = true;
    cleanupEditor(MEditor);
    if (MEditor) MEditor->destroy();
  }

  //----------

  bool gui_set_scale(double scale) override {
    //SAT_PRINT("scale %.3f\n",scale);
    if (MEditor) return MEditor->setScale(scale);
    return false;
  }

  //----------

  bool gui_get_size(uint32_t *width, uint32_t *height) override {
    //SAT_PRINT("\n");
    if (MEditor) return MEditor->getSize(width,height);
    return false;
  }

  //----------

  bool gui_can_resize() override {
    //SAT_PRINT("\n");
    if (MEditor) return MEditor->canResize();
    return false;
  }

  //----------

  bool gui_get_resize_hints(clap_gui_resize_hints_t *hints) override {
    //SAT_PRINT("\n");
    if (MEditor) return MEditor->getResizeHints(hints);
    return false;
  }

  //----------

  bool gui_adjust_size(uint32_t *width, uint32_t *height) override {
    //SAT_PRINT("*width %i *height %i\n",*width,*height);
    if (MEditor) return MEditor->adjustSize(width,height);
    return false;
  }

  //----------

  bool gui_set_size(uint32_t width, uint32_t height) override {
    //SAT_PRINT("width %i height %i\n",width,height);
    if (MEditor) return MEditor->setSize(width,height);
    return false;
  }

  //----------

  bool gui_set_parent(const clap_window_t *window) override {
    //SAT_PRINT("api %s ptr %p\n",window->api,window->ptr);
    if (MEditor) {
      if (MEditor->setParent(window)) {
        // SAT_TRACE;
        // // should this be in _create?
        // SAT_Window* window = MEditor->getWindow();
        // if (window) setupEditor(MEditor,window);
        // setEditorParameterValues();
        return true;
      }
    }
    return false;
  }

  //----------

  bool gui_set_transient(const clap_window_t *window) override {
    //SAT_PRINT("api %s ptr %p\n",window->api,window->ptr);
    if (MEditor) return MEditor->setTransient(window);
    return false;
  }

  //----------

  void gui_suggest_title(const char *title) override {
    //SAT_PRINT("title %s\n",title);
    if (MEditor) MEditor->suggestTitle(title);
  }

  //----------

  bool gui_show() override {
    //SAT_PRINT("\n");
    if (MEditor) return MEditor->show();
    return false;
  }

  //----------

  bool gui_hide() override {
    //SAT_PRINT("\n");
    if (MEditor) return MEditor->hide();
    return false;
  }

  #endif // no gui

  // latency
  //------------------------------

  uint32_t latency_get() override {
    return 0;
  }

  // note name
  //------------------------------

  uint32_t note_name_count() override {
    return 0;
  }

  //----------

  bool note_name_get(uint32_t index, clap_note_name_t *note_name) override {
    // note_name->port = -1;
    // note_name->channel = -1;
    // note_name->key = -1;
    // SAT_Strlcpy(note_name->name,"",CLAP_NAME_SIZE);
    return false;
  }

  // note ports
  //------------------------------

  uint32_t note_ports_count(bool is_input) override {
    if (is_input) return MNoteInputPorts.size();
    else return MNoteOutputPorts.size();
  }

  //----------

  bool note_ports_get(uint32_t index, bool is_input, clap_note_port_info_t *info) override {
    uint32_t size = sizeof(clap_note_port_info_t);
    void* ptr;
    if (is_input) ptr = MNoteInputPorts[index]->getInfo();
    else ptr = MNoteOutputPorts[index]->getInfo();
    memcpy(info,ptr,size);
    return true;
  }

  // param indication
  //------------------------------

  void param_indication_set_mapping(clap_id param_id, bool has_mapping, const clap_color_t *color, const char *label, const char *description) override {
    //SAT_TRACE;
    SAT_Parameter* param = MParameters[param_id];
    // param->setMappingIndication(has_mapping,color,label,description);
    param->setIndicateMapped(has_mapping);
    SAT_Color C = SAT_Color(
      (double)color->red    * SAT_INV255,
      (double)color->green  * SAT_INV255,
      (double)color->blue   * SAT_INV255,
      (double)color->alpha  * SAT_INV255
    );
    param->setIndicateMappedColor(C);
    #ifndef SAT_NO_GUI
      SAT_Widget* widget = (SAT_Widget*)param->getWidget();
      if (widget) widget->do_widget_redraw(widget);
    #endif
  }

  //----------

  void param_indication_set_automation(clap_id param_id, uint32_t automation_state, const clap_color_t *color) override {
    //SAT_TRACE;
    SAT_Parameter* param = MParameters[param_id];
    // param->setAutomationIndication(automation_state,color);
    //param->setAutomationIndication(automation_state,color);
    param->setIndicateAutomationState(automation_state);
    SAT_Color C = SAT_Color(
      (double)color->red    * SAT_INV255,
      (double)color->green  * SAT_INV255,
      (double)color->blue   * SAT_INV255,
      (double)color->alpha  * SAT_INV255
    );
    param->setIndicateAutomationColor(C);
    #ifndef SAT_NO_GUI
      SAT_Widget* widget = (SAT_Widget*)param->getWidget();
      if (widget) widget->do_widget_redraw(widget);
    #endif
  }

  // params
  //------------------------------

  uint32_t params_count() override {
    return MParameters.size();
  }

  //----------

  bool params_get_info(uint32_t param_index, clap_param_info_t *param_info) override {
    clap_param_info_t* src = MParameters[param_index]->getInfo();
    uint32_t size = sizeof(clap_param_info_t);
    memcpy(param_info,src,size);
    return true;
  }

  //----------

  bool params_get_value(clap_id param_id, double *out_value) override {
    double value = MParameters[param_id]->getValue();
    *out_value = value;
    return true;
  }

  //----------

  bool params_value_to_text(clap_id param_id, double value, char *out_buffer, uint32_t out_buffer_capacity) override {
    const char* txt = MParameters[param_id]->valueToText(value);
    SAT_Strlcpy(out_buffer,txt,out_buffer_capacity);
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
      MProcessor->processEvent(header);
    }
  }

  // posix fd support
  //------------------------------

  void posix_fd_support_on_fd(int fd, clap_posix_fd_flags_t flags) override {
  }

  // preset load
  //------------------------------

  bool preset_load_from_location(uint32_t location_kind, const char *location, const char *load_key) override {
    const clap_host_t* host = getClapHost();
    const clap_host_preset_load_t* host_preset_load = (const clap_host_preset_load_t*)host->get_extension(host,CLAP_EXT_PRESET_LOAD);
    switch (location_kind) {
      case CLAP_PRESET_DISCOVERY_LOCATION_FILE: {
        SAT_PRINT("CLAP_PRESET_DISCOVERY_LOCATION_FILE location '%s', load_key '%s'\n",location,load_key);
        bool loaded = loadPresetFromFile(location,load_key);
        if (loaded && host_preset_load) host_preset_load->loaded(host,location_kind,location,load_key);
        return true;
      }
      case CLAP_PRESET_DISCOVERY_LOCATION_PLUGIN: {
        SAT_PRINT("CLAP_PRESET_DISCOVERY_LOCATION_PLUGIN location '%s', load_key '%s'\n",location,load_key);
        bool loaded = false;//loadPresetFromPlugin(location,load_key);
        if (loaded && host_preset_load) host_preset_load->loaded(host,location_kind,location,load_key);
        return true;
      }
      default: {
        SAT_PRINT("unknown location kind (%i : '%s','%s')\n",location_kind,location,load_key);
        return false;
      }
    }
    return false;
  }

  // remote controls
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

  // render
  //------------------------------

  bool render_has_hard_realtime_requirement() override {
    return false;
  }

  //----------

  bool render_set(clap_plugin_render_mode mode) override {
    MRenderMode = mode;
    return false;
  }

  // state
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
    return false;
  }

  // state context
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

  // surround
  //------------------------------

  bool surround_is_channel_mask_supported(uint64_t channel_mask) override {
    return false;
  }

  //----------

  uint32_t surround_get_channel_map(bool is_input, uint32_t port_index, uint8_t *channel_map, uint32_t channel_map_capacity) override {
    return 0;
  }

  // tail
  //------------------------------

  uint32_t tail_get() override {
    return 0;
  }

  // thread pool
  //------------------------------

  void thread_pool_exec(uint32_t task_index) override {
  }

  // timer support
  //------------------------------

  void timer_support_on_timer(clap_id timer_id) override {
  }

  // track info
  //------------------------------

  void track_info_changed() override {
    // const clap_host_t* host = getClapHost();
    // //const clap_host_track_info_t* track_info = (const clap_host_track_info_t*)host->get_extension(host,CLAP_EXT_TRACK_INFO);
    // clap_track_info_t info;
    // //if (track_info->get(host,&info)) {
    // if (MHost->ext.track_info) {
    //   if (MHost->ext.track_info->get(host,&info)) {
    //     MTrackFlags = info.flags;
    //     if (info.flags & CLAP_TRACK_INFO_HAS_TRACK_NAME) {
    //       SAT_Strlcpy(MTrackName,info.name,CLAP_NAME_SIZE);
    //       SAT_Print("%s\n",MTrackName);
    //     }
    //     if (info.flags & CLAP_TRACK_INFO_HAS_TRACK_COLOR) {
    //       MTrackColor = SAT_Color(info.color.red,info.color.green,info.color.blue,info.color.alpha);
    //     }
    //     if (info.flags & CLAP_TRACK_INFO_HAS_AUDIO_CHANNEL) {
    //       MTrackChannelCount = info.audio_channel_count;
    //       MTrackPortType = info.audio_port_type;
    //     }
    //     MTrackIsReturnTrack = ((info.flags & CLAP_TRACK_INFO_IS_FOR_RETURN_TRACK)  != 0);
    //     MTrackIsBus         = ((info.flags & CLAP_TRACK_INFO_IS_FOR_BUS)           != 0);
    //     MTrackIsMaster      = ((info.flags & CLAP_TRACK_INFO_IS_FOR_MASTER)        != 0);
    //     //SAT_Widget* widget = (SAT_Widget*)param->getWidget();
    //     //if (widget && MEditor && MEditor->isOpen()) widget->do_widget_redraw(widget);
    //   }
    // }
  }

  // voice info
  //------------------------------

  bool voice_info_get(clap_voice_info_t *info) override {
    // info->voice_count     = 16;
    // info->voice_capacity  = 16;
    // info->flags           = CLAP_VOICE_INFO_SUPPORTS_OVERLAPPING_NOTES;
    // return true;
    return false;
  }

//----------------------------------------------------------------------
public: // draft extensions
//----------------------------------------------------------------------

  // extensible audio ports
  //------------------------------

  bool extensible_audio_ports_add_port(bool is_input, uint32_t channel_count, const char *port_type, const void *port_details) override {
    return false;
  }

  //----------

  bool extensible_audio_ports_remove_port(bool is_input, uint32_t index) override {
    return false;
  }

  // resource directory
  //------------------------------

  void resource_directory_set_directory(const char *path, bool is_shared) override {
    // MResourceDirectory = path;
    // MResourceDirectoryShared = is_shared;
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

  // triggers
  //------------------------------

  uint32_t triggers_count() override {
    return 0;
  }

  //----------

  bool triggers_get_info(uint32_t index, clap_trigger_info_t *trigger_info) override {
    return false;
  }

  // tuning
  //------------------------------

  void tuning_changed() override {
  }

  // undo
  //------------------------------

  void undo_get_delta_properties(clap_undo_delta_properties_t *properties) override {
  }

  //----------

  bool undo_can_use_delta_format_version(clap_id format_version) override {
    return false;
  }

  //----------

  bool undo_apply_delta(clap_id format_version, const void* delta, size_t delta_size) override {
    return false;
  }

  //----------

  void undo_set_context_info(uint64_t flags, const char* undo_name, const char* redo_name) override {
  }

};

//----------------------------------------------------------------------
#endif