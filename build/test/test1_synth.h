//#define SAT_PLUGIN_VST3

//

#include "base/sat.h"
#include "audio/sat_audio_math.h"
#include "audio/sat_audio_utils.h"
#include "audio/sat_voice_manager.h"
#include "gui/sat_widgets.h"
#include "plugin/sat_plugin.h"

#include "test1_synth_widgets.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifdef SAT_DEBUG
  #define PLUGIN_NAME "test1_synth (debug)"
#else
  #define PLUGIN_NAME "test1_synth"
#endif

#define EDITOR_WIDTH  720
#define EDITOR_HEIGHT 550
#define EDITOR_SCALE  1.5

const char* buttontext[5] = { "1", "2", "3", "IV", "five" };

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t test1_synth_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/" PLUGIN_NAME "/" SAT_VERSION,
  .name         = PLUGIN_NAME,
  .vendor       = SAT_VENDOR,
  .url          = "",
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
  .description  = "...",
  .features     = (const char* []) {
    //CLAP_PLUGIN_FEATURE_AUDIO_EFFECT    
    CLAP_PLUGIN_FEATURE_INSTRUMENT,
    nullptr
  }
};

//----------------------------------------------------------------------
//
// voice
//
//----------------------------------------------------------------------

#define NUM_VOICES 32

class test1_synth_voice {

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
    MIndex = AIndex;
    MContext = AContext;
    srate = AContext->sample_rate;
    
  }

  //----------

  sat_sample_t getEnvLevel() {
    return 0.0;
  }

  //----------

  uint32_t process(uint32_t AState, uint32_t AOffset, uint32_t ALength) {
    float* buffer = MContext->voice_buffer;
    buffer += (MIndex * SAT_PLUGIN_MAX_BLOCK_SIZE);
    buffer += AOffset;
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
    
    SAT_GLOBAL.DEBUG.observe(SAT_OBSERVE_FLOAT,&ph,"ph");
    
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

  //----------

  uint32_t noteOff(uint32_t AKey, double AVelocity) {
    //SAT_Print("\n");
    return SAT_VOICE_FINISHED;
  }

  //----------

  void noteChoke(uint32_t AKey, double AVelocity) {
    //SAT_Print("\n");
  }

  //----------

  void noteExpression(uint32_t AExpression, double AValue) {
    //SAT_Print("\n");
  }

  //----------

  void parameter(uint32_t AIndex, double AValue) {
    //SAT_Print("\n");
  }

  //----------

  void modulation(uint32_t AIndex, double AValue) {
    //SAT_Print("\n");
  }

};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class test1_synth_plugin
: public SAT_Plugin {

//------------------------------
private:
//------------------------------

  SAT_VoiceManager<test1_synth_voice,NUM_VOICES>  MVoiceManager = {};
  //SAT_PanelWidget*    MRootPanel      = nullptr;
  SAT_VoicesWidget*   MVoicesWidget   = nullptr;
  SAT_WaveformWidget* MWaveformWidget = nullptr;
  
  double  MTestValue  = 0.0;
  double  qwe2        = 1.0;

//------------------------------
public:
//------------------------------

  //SAT_PLUGIN_DEFAULT_CONSTRUCTOR(myPlugin);

  test1_synth_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_Plugin(ADescriptor,AHost) {
    //SAT_Print("sizeof(myPlugin) : %i\n",sizeof(myPlugin));
  }

  //----------

  //virtual ~myPlugin() {
  //  SAT_Print("yepp, we are being deleted.. ptr: %p\n",this);
  //  //SAT_PRINT;
  //}

//------------------------------
public:
//------------------------------

  bool init() final {
    
    SAT_Observe(SAT_OBSERVE_DOUBLE,&MTestValue,"MTestValue");
    SAT_Observe(SAT_OBSERVE_DOUBLE,&qwe2,"qwe2");
    
    //SAT_GLOBAL.DEBUG.print_callstack();
    //int* ptr = nullptr;
    //int a = *ptr;
    
    SAT_PRINT;
    SAT_Print("Hello world!\n");
    SAT_DPrint( SAT_TERM_RESET
                SAT_TERM_NORMAL "normal " SAT_TERM_RESET
                SAT_TERM_BOLD "bold " SAT_TERM_RESET
                SAT_TERM_FAINT "faint " SAT_TERM_RESET
                SAT_TERM_ITALICS "italics " SAT_TERM_RESET
                SAT_TERM_UNDERLINE "underline\n" SAT_TERM_RESET );
    SAT_DPrint( SAT_TERM_FG_RED "hello" SAT_TERM_FG_YELLOW " world2\n" SAT_TERM_RESET);
   
    registerDefaultSynthExtensions();
    appendClapNoteInputPort();
    appendStereoOutputPort();

    /*SAT_Parameter* par1 =*/ appendParameter( new SAT_Parameter("Param1",0.1) );
    /*SAT_Parameter* par2 =*/ appendParameter( new SAT_Parameter("Param2",0.4) );
      SAT_Parameter* par3 =   appendParameter( new SAT_Parameter("Param3",0.7) );
    par3->setFlag(CLAP_PARAM_IS_MODULATABLE);
    /*SAT_Parameter* par4 =*/ appendParameter( new SAT_Parameter("Param4",0.1) );

    setInitialEditorSize(EDITOR_WIDTH,EDITOR_HEIGHT,EDITOR_SCALE);

    //setProcessThreaded(false);
    //setEventMode(SAT_PLUGIN_EVENT_MODE_BLOCK);
    
    SAT_Host* host = getHost();
    const clap_plugin_t*  clapplugin = getPlugin();
    const clap_host_t* claphost = host->getHost();
    MVoiceManager.init(clapplugin,claphost);
    MVoiceManager.setProcessThreaded(false);
    MVoiceManager.setEventMode(SAT_PLUGIN_EVENT_MODE_INTERLEAVED);
    
    // test crash..
    //double qwe = *(double*)nullptr;
    
    return SAT_Plugin::init();
    
  }

  //----------

  //void destroy() final {
  //  // root widget is not automatically deleted
  //  if (MRootPanel) delete MRootPanel;
  //  SAT_Plugin::destroy();
  //}

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

  #include "test1_synth_gui.h"
  
//------------------------------
public:
//------------------------------

  void do_editorListener_timer() final {
    SAT_Plugin::do_editorListener_timer();
    //#ifndef SAT_EXE
    for (uint32_t voice=0; voice<NUM_VOICES; voice++) {
      uint32_t state = MVoiceManager.getVoiceState(voice);
      MVoicesWidget->setVoiceState(voice,state);
    }
    MVoicesWidget->parentRedraw();
    //#endif
  }

//------------------------------
public:
//------------------------------

  void postProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) final {
    MVoiceManager.postProcessEvents(in_events,out_events);
  }

  //----------

  bool handleNoteOn(const clap_event_note_t* event) final {
    MVoiceManager.handleNoteOn(event);
    return true;
  }

  //----------

  bool handleNoteOff(const clap_event_note_t* event) final {
    MVoiceManager.handleNoteOff(event);
    return true;
  }

  //----------

  bool handleNoteChoke(const clap_event_note_t* event) final {
    MVoiceManager.handleNoteChoke(event);
    return true;
  }

  //----------

  bool handleNoteExpression(const clap_event_note_expression_t* event) final {
    MVoiceManager.handleNoteExpression(event);
    return true;
  }

  //----------

  bool handleParamValue(const clap_event_param_value_t* event) final {
    MVoiceManager.handleParamValue(event);
    return true;
  }

  //----------

  bool handleParamMod(const clap_event_param_mod_t* event) final {
    MVoiceManager.handleParamMod(event);
    return true;
  }

  //----------

  bool handleTransport(const clap_event_transport_t* event) final {
    return false;
  }

  //----------

  bool handleMidi(const clap_event_midi_t* event) final {
    MVoiceManager.handleMidi(event);
    return true;
  }

  //----------

  bool handleMidiSysex(const clap_event_midi_sysex_t* event) final {
    MVoiceManager.handleMidiSysex(event);
    return true;
  }

  //----------

  bool handleMidi2(const clap_event_midi2_t* event) final {
    MVoiceManager.handleMidi2(event);
    return true;
  }

//------------------------------
public:
//------------------------------

  // block

  void processAudio(SAT_ProcessContext* AContext) final {
    const clap_process_t* process = AContext->process;
    uint32_t length = process->frames_count;
    float** outputs = process->audio_outputs[0].data32;
    AContext->voice_buffer = outputs;
    AContext->voice_length = length;
    MVoiceManager.processAudio(AContext);
    
    sat_param_t scale = getParameterValue(2) + getModulationValue(2);
    scale = SAT_Clamp(scale,0,1);
    
    SAT_ScaleStereoBuffer(outputs,scale,length);
  }

  //----------

  // interleaved

  /*
  void processAudio(SAT_ProcessContext* AContext, uint32_t offset, uint32_t length) final {
    const clap_process_t* process = AContext->process;
    //uint32_t length = process->frames_count;
    float* inputs[2];
    float* outputs[2];
    inputs[0]  = process->audio_inputs[0].data32[0] + offset;
    inputs[1]  = process->audio_inputs[0].data32[1] + offset;
    outputs[0] = process->audio_outputs[0].data32[0] + offset;
    outputs[1] = process->audio_outputs[0].data32[1] + offset;
    SAT_CopyStereoBuffer(outputs,inputs,length);
    sat_param_t scale = getParameterValue(2) + getModulationValue(2);
    scale = SAT_Clamp(scale,0,1);
    SAT_ScaleStereoBuffer(outputs,scale,length);
  }
  */

  //----------

  // quantized

  /*
  void processAudio(SAT_ProcessContext* AContext, uint32_t offset) final {
  }
  */

  };

//----------------------------------------------------------------------
//
// register
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY

  #include "plugin/sat_entry.h"

  SAT_PLUGIN_ENTRY(test1_synth_descriptor,test1_synth_plugin);

  /*
  
  void SAT_Register(SAT_Registry* ARegistry) {
    //SAT_PRINT;
    uint32_t index = ARegistry->getNumDescriptors();
    SAT_Log("SAT_Register -> id %s index %i\n",myDescriptor.id,index);
    ARegistry->registerDescriptor(&myDescriptor);
  }

  const clap_plugin_t* SAT_CreatePlugin(uint32_t AIndex, const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost) {
    SAT_Log("SAT_CreatePlugin (index %i)\n",AIndex);
    if (AIndex == 0) {
      myPlugin* plugin = new myPlugin(ADescriptor,AHost); // deleted in SAT_Plugin.destroy
      return plugin->getPlugin();
    }
    return nullptr;
  }
  
  */

#endif // SAT_NO_ENTRY

//----------------------------------------------------------------------
