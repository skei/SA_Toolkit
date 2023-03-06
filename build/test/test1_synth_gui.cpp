
#include "base/sat.h"
#include "audio/sat_audio_math.h"
#include "audio/sat_audio_utils.h"
//#include "audio/sat_modulation_matrix.h"
#include "audio/sat_voice_manager.h"
#include "gui/sat_widgets.h"
#include "plugin/sat_plugin.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#define PLUGIN_NAME   "myPlugin"
#define EDITOR_WIDTH  (50 + 200                   + 10 + 200 + 10 + 200 + 50)
#define EDITOR_HEIGHT (50 + 200 + (5 * (10 + 20))                       + 50)

const char* buttontext[5] = { "1", "2", "3", "IV", "five" };

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t myDescriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/" PLUGIN_NAME "/" SAT_VERSION,
  .name         = PLUGIN_NAME,
  .vendor       = SAT_VENDOR,
  .url          = "",
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
  .description  = "...",
  //.features     = (const char*[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr }
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_INSTRUMENT, nullptr }
};

//----------------------------------------------------------------------
//
// voice
//
//----------------------------------------------------------------------

#define NUM_VOICES 32

class myVoice {

//------------------------------
private:
//------------------------------

  uint32_t          MIndex    = 0;
  SAT_VoiceContext* MContext  = nullptr;

  float             srate     = 0.0;
  float             ph        = 0.0;
  float             phadd     = 0.0;

//------------------------------
public:
//------------------------------

  void init(uint32_t AIndex, SAT_VoiceContext* AContext) {
    MIndex    = AIndex;
    MContext  = AContext;
    srate = AContext->sample_rate;
  }

  //----------

  sat_sample_t getEnvLevel() {
    return 0.0;
  }

  //----------

  uint32_t process(uint32_t AState, uint32_t AOffset, uint32_t ALength) {
    //float* buffer = MContext->buffer;
    float* buffer = MContext->voice_buffer;
    buffer += AOffset;
    buffer += (MIndex * SAT_PLUGIN_MAX_BLOCK_SIZE);
    if ((AState == SAT_VOICE_PLAYING) || (AState == SAT_VOICE_RELEASED)) {
      for (uint32_t i=0; i<ALength; i++) {
        ph = SAT_Fract(ph);
        float v = (ph * 2.0) - 1.0;  // 0..1 -> -1..1
        //v = sin(v * SAT_PI2);
        *buffer++ = v * 0.1;
        ph += phadd;
      }
    } // playing
    else {
      memset(buffer,0,ALength * sizeof(float));
    }
    return AState;
  }

  //----------

  uint32_t processSlice(uint32_t AState, uint32_t AOffset) {
    return process(AState,AOffset,SAT_AUDIO_QUANTIZED_SIZE);
  }

  //----------

  uint32_t noteOn(uint32_t AKey, double AVelocity) {
    //SAT_Print("\n");
    ph = 0.0;
    float hz = SAT_NoteToHz(AKey);
    phadd = 1.0 / SAT_HzToSamples(hz,srate);
    return SAT_VOICE_PLAYING;
  }

  uint32_t noteOff(uint32_t AKey, double AVelocity) {
    //SAT_Print("\n");
    return SAT_VOICE_FINISHED;
  }

  void noteChoke(uint32_t AKey, double AVelocity) {
    //SAT_Print("\n");
  }

  void noteExpression(uint32_t AExpression, double AValue) {
    //SAT_Print("\n");
  }

  void parameter(uint32_t AIndex, double AValue) {
    //SAT_Print("\n");
  }

  void modulation(uint32_t AIndex, double AValue) {
    //SAT_Print("\n");
  }

};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class myPlugin
: public SAT_Plugin {

//------------------------------
private:
//------------------------------

  SAT_PanelWidget* MRootPanel = nullptr;
  SAT_VoiceManager<myVoice,NUM_VOICES>  MVoiceManager = {};
  //SAT_ModulationMatrix MModMatrix = {};

//------------------------------
public:
//------------------------------

  //SAT_PLUGIN_DEFAULT_CONSTRUCTOR(myPlugin);

  myPlugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_Plugin(ADescriptor,AHost) {
    //SAT_Print("sizeof(myPlugin) : %i\n",sizeof(myPlugin));
  }

  virtual ~myPlugin() {
    //SAT_Print("yepp, we are being deleted..\n");
  }

//------------------------------
public:
//------------------------------

  bool init() final {
    SAT_Print("Hello world!\n");
    registerDefaultExtensions();
    registerExtension(CLAP_EXT_THREAD_POOL,&MThreadPoolExt);
    registerExtension(CLAP_EXT_VOICE_INFO,&MVoiceInfoExt);
    appendClapNoteInputPort();
    //appendStereoInputPort();
    appendStereoOutputPort();
    SAT_Parameter* par;
    par = appendParameter( new SAT_Parameter("Param1",0.0) );
    par = appendParameter( new SAT_Parameter("Param2",0.5) );
    par = appendParameter( new SAT_Parameter("Param3",1.5) );
    par->setFlag(CLAP_PARAM_IS_MODULATABLE);
    setInitialEditorSize(EDITOR_WIDTH,EDITOR_HEIGHT);

    //setProcessThreaded(false);
    //setEventMode(SAT_PLUGIN_EVENT_MODE_INTERLEAVED);

    SAT_Host* host = getHost();
    const clap_plugin_t*  clapplugin = getPlugin();
    const clap_host_t* claphost = host->getHost();
    MVoiceManager.init(clapplugin,claphost);

    MVoiceManager.setProcessThreaded(true);
    MVoiceManager.setEventMode(SAT_PLUGIN_EVENT_MODE_INTERLEAVED);

    //setProcessThreaded(false);
    //setEventMode(SAT_PLUGIN_EVENT_MODE_BLOCK);

    return SAT_Plugin::init();
  }

  //----------

  void destroy() final {
    // root widget is not automatically deleted
    if (MRootPanel) delete MRootPanel;
    SAT_Plugin::destroy();
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MVoiceManager.activate(sample_rate,min_frames_count,max_frames_count);
    return SAT_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

  //----------

  void thread_pool_exec(uint32_t task_index) final {
    MVoiceManager.thread_pool_exec(task_index);
  }

  //----------

  bool voice_info_get(clap_voice_info_t *info) final {
    info->voice_count     = NUM_VOICES;
    info->voice_capacity  = NUM_VOICES;
    info->flags           = CLAP_VOICE_INFO_SUPPORTS_OVERLAPPING_NOTES;
    return true;
  }


//------------------------------
public:
//------------------------------

  //SAT_Editor* createEditor(SAT_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight) final {
  //  //SAT_PRINT;
  //  SAT_Editor* editor = new SAT_Editor(AListener,AWidth,AHeight);
  //  return editor;
  //}

  //----------

  bool initEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) final {
    //SAT_PRINT;

    // root

    MRootPanel = new SAT_PanelWidget( SAT_Rect(0,0,EDITOR_WIDTH,EDITOR_HEIGHT) );
    AWindow->setRootWidget(MRootPanel);
    MRootPanel->setFillBackground(true);

    // menu

    SAT_MenuWidget* menu = new SAT_MenuWidget(SAT_Rect(260,70,150,150));
    menu->setRightClickClose(true);

      SAT_MenuItemWidget* i1 = new SAT_MenuItemWidget(SAT_Rect(5,5,140,15),"Item 1");
      menu->appendChildWidget(i1);
      i1->setDrawBorder(false);

      SAT_MenuItemWidget* i2 = new SAT_MenuItemWidget(SAT_Rect(5,20,140,15),"Item 2");
      menu->appendChildWidget(i2);
      i2->setDrawBorder(false);

      SAT_MenuItemWidget* i3 = new SAT_MenuItemWidget(SAT_Rect(5,35,140,15),"Item 3");
      menu->appendChildWidget(i3);
      i3->setDrawBorder(false);


    // widgets

    SAT_LogoWidget* logo = new SAT_LogoWidget(SAT_Rect(50,50,200,200));
    MRootPanel->appendChildWidget(logo);
    logo->setLogoColor(SAT_DarkMagenta);

    SAT_TextWidget* text = new SAT_TextWidget(SAT_Rect(50,270,200,20),"Hello world!");
    MRootPanel->appendChildWidget(text);
    text->setTextSize(12);

    SAT_ValueWidget* value = new SAT_ValueWidget(SAT_Rect(50,300,200,20),"Param 1", 0.0);
    MRootPanel->appendChildWidget(value);
    value->setTextSize(12);

    SAT_DragValueWidget* dragvalue = new SAT_DragValueWidget(SAT_Rect(50,330,200,20),"Param 2", 0.0);
    MRootPanel->appendChildWidget(dragvalue);
    dragvalue->setTextSize(12);
    dragvalue->setSnap(true);
    dragvalue->setSnapPos(0.5);
    dragvalue->setAutoHideCursor(false);
    dragvalue->setAutoLockCursor(true);

    SAT_SliderWidget* slider = new SAT_SliderWidget(SAT_Rect(50,360,200,20),"Param 3", 0.0);
    MRootPanel->appendChildWidget(slider);
    slider->setTextSize(12);
    slider->setBipolar(true);
    slider->setBipolarCenter(0.5);
    slider->setSnap(true);
    slider->setSnapPos(0.5);
    slider->setAutoHideCursor(true);
    slider->setAutoLockCursor(true);
    //slider->setModulation(0.25);
    slider->setDrawModulation(true);
    slider->setModulationColor( SAT_Color(1,1,1,0.25) );

    SAT_ButtonWidget* button1 = new SAT_ButtonWidget(SAT_Rect(50,390,95,20));
    MRootPanel->appendChildWidget(button1);

    SAT_ButtonWidget* button2 = new SAT_ButtonWidget(SAT_Rect(155,390,95,20));
    MRootPanel->appendChildWidget(button2);
    button2->setIsToggle(true);

    // column 2

    SAT_SelectorWidget* selector = new SAT_SelectorWidget(SAT_Rect(260,50,200,20),"Select",menu);
    MRootPanel->appendChildWidget(selector);

    SAT_GridWidget* grid = new SAT_GridWidget(SAT_Rect(260,80,200,200),16,16);
    MRootPanel->appendChildWidget(grid);

    SAT_ButtonRowWidget* buttonrow = new SAT_ButtonRowWidget(SAT_Rect(260,290,200,20),5,buttontext,SAT_BUTTON_ROW_MULTI);
    MRootPanel->appendChildWidget(buttonrow);
    buttonrow->setValueIsBits(true,8);

    SAT_KnobWidget* knob = new SAT_KnobWidget(SAT_Rect(260,320,50,50),"%",0.0);
    MRootPanel->appendChildWidget(knob);
    knob->setBipolar(true);
    knob->setBipolarCenter(0.5);
    knob->setSnap(true);
    knob->setSnapPos(0.5);

    // column 3

    SAT_GraphWidget* graph = new SAT_GraphWidget(SAT_Rect(470,50,200,230));
    MRootPanel->appendChildWidget(graph);
    graph->setFillBackground(true);

    for (uint32_t i=0; i<5; i++) {
      SAT_GraphModule* module = new SAT_GraphModule();
      module->numInputs = 2;
      module->inputs[0] = SAT_PIN_SIGNAL;
      module->numOutputs = 2;
      graph->addModule(module,i*10,i*10,"module");
    }

    //

    SAT_MenuItemWidget* menuitem = new SAT_MenuItemWidget(SAT_Rect(320,320,140,20),"MenuItem");
    MRootPanel->appendChildWidget(menuitem);

    MRootPanel->appendChildWidget(menu);

    AEditor->connect(value,     getParameter(0));
    AEditor->connect(dragvalue, getParameter(1));
    AEditor->connect(slider,    getParameter(2));

    return true;
  }

//------------------------------
public:
//------------------------------

// TODO

  void preProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) final {
    MVoiceManager.preProcessEvents(in_events,out_events);
  }

  //----------

  void postProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) final {
    MVoiceManager.postProcessEvents(in_events,out_events);
  }

  //----------

  bool handleNoteOn(const clap_event_note_t* event) final {
    MVoiceManager.handleNoteOn(event);
    return true;
  }

  bool handleNoteOff(const clap_event_note_t* event) final {
    MVoiceManager.handleNoteOff(event);
    return true;
  }

  bool handleNoteChoke(const clap_event_note_t* event) final {
    MVoiceManager.handleNoteChoke(event);
    return true;
  }

  bool handleNoteExpression(const clap_event_note_expression_t* event) final {
    MVoiceManager.handleNoteExpression(event);
    return true;
  }

  bool handleParamValue(const clap_event_param_value_t* event) final {
    MVoiceManager.handleParamValue(event);
    return true;
  }

  bool handleParamMod(const clap_event_param_mod_t* event) final {
    MVoiceManager.handleParamMod(event);
    return true;
  }

  bool handleTransport(const clap_event_transport_t* event) final {
    return false;
  }

  bool handleMidi(const clap_event_midi_t* event) final {
    MVoiceManager.handleMidi(event);
    return true;
  }

  bool handleMidiSysex(const clap_event_midi_sysex_t* event) final {
    MVoiceManager.handleMidiSysex(event);
    return true;
  }

  bool handleMidi2(const clap_event_midi2_t* event) final {
    MVoiceManager.handleMidi2(event);
    return true;
  }

//------------------------------
public:
//------------------------------

  void processAudio(SAT_ProcessContext* AContext) final {

    const clap_process_t* process = AContext->process;
    uint32_t length = process->frames_count;
    //float** inputs = process->audio_inputs[0].data32; // instrument, n o input..
    float** outputs = process->audio_outputs[0].data32;

    AContext->voice_buffer = outputs;
    AContext->voice_length = length;
    MVoiceManager.processAudio(AContext);

    sat_param_t scale = getParameterValue(2);
    SAT_ScaleStereoBuffer(outputs,scale,length);

  }

  //----------

  // interleaved

//  void processAudio(SAT_ProcessContext* AContext, uint32_t offset, uint32_t length) final {
//    const clap_process_t* process = AContext->process;
//    //uint32_t length = process->frames_count;
//    float* inputs[2];
//    float* outputs[2];
//    inputs[0]  = process->audio_inputs[0].data32[0] + offset;
//    inputs[1]  = process->audio_inputs[0].data32[1] + offset;
//    outputs[0] = process->audio_outputs[0].data32[0] + offset;
//    outputs[1] = process->audio_outputs[0].data32[1] + offset;
//    SAT_CopyStereoBuffer(outputs,inputs,length);
//    sat_param_t scale = getParameterValue(2) + getModulationValue(2);
//    scale = SAT_Clamp(scale,0,1);
//    SAT_ScaleStereoBuffer(outputs,scale,length);
//  }

};

//----------------------------------------------------------------------
//
// register
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY

  #include "plugin/sat_entry.h"

  //SAT_PLUGIN_ENTRY(myDescriptor,myPlugin);

  void SAT_Register(SAT_Registry* ARegistry) {
    //SAT_PRINT;
    uint32_t index = ARegistry->getNumDescriptors();
    SAT_Log("SAT_Register -> id %s index %i\n",myDescriptor.id,index);
    ARegistry->registerDescriptor(&myDescriptor);
  }

  const clap_plugin_t* SAT_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
    SAT_Log("SAT_CreatePlugin (index %i)\n",AIndex);
    if (AIndex == 0) {
      myPlugin* plugin = new myPlugin(ADescriptor,AHost);
      return plugin->getPlugin();
    }
    return nullptr;
  }

#endif

