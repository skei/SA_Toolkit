
#include "base/sat.h"
#include "audio/sat_audio_utils.h"
#include "gui/sat_widgets.h"
#include "plugin/sat_plugin.h"

#include "plugin/sat_voice_manager.h"
#include "audio/processor/sat_audio_processor.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

#define EDITOR_WIDTH  (50 + 200 + 50)
#define EDITOR_HEIGHT (50 + 200 + (4 * (10 + 20)) + 50)

//----------

const clap_plugin_descriptor_t myDescriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "me/myPlugin/0",
  .name         = "myPlugin",
  .vendor       = "me",
  .url          = "",
  .manual_url   = "",
  .support_url  = "",
  .version      = "0",
  .description  = "...",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr }
};

//----------------------------------------------------------------------
//
// voice
//
//----------------------------------------------------------------------

/*
#define NUM_VOICES 32

class myVoice {

//------------------------------
private:
//------------------------------

  uint32_t          MIndex    = 0;
  SAT_VoiceContext* MContext  = nullptr;

//------------------------------
public:
//------------------------------

  void init(uint32_t AIndex, SAT_VoiceContext* AContext) {
    MIndex    = AIndex;
    MContext  = AContext;
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
        //ph = MIP_Fract(ph);
        //float v = (ph * 2.0) - 1.0;  // 0..1 -> -1..1
        //*buffer++ = v * 0.1;
        *buffer++ = 0.0;
        //ph += phadd;
      }
    } // playing
    else {
      memset(buffer,0,ALength * sizeof(float));
    }
    return AState;
  }

  //----------

  uint32_t processSlice(uint32_t AState, uint32_t AOffset) {
    return AState;
  }

  //----------

  uint32_t noteOn(uint32_t AKey, double AVelocity) { return SAT_VOICE_PLAYING; }
  uint32_t noteOff(uint32_t AKey, double AVelocity) { return SAT_VOICE_FINISHED; }
  void noteChoke(uint32_t AKey, double AVelocity) {}
  void noteExpression(uint32_t AExpression, double AValue) {}
  void parameter(uint32_t AIndex, double AValue) {}
  void modulation(uint32_t AIndex, double AValue) {}

};
*/

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

  SAT_PanelWidget*                      MRootPanel    = nullptr;
  //SAT_VoiceManager<myVoice,NUM_VOICES>  MVoiceManager = {};

//------------------------------
public:
//------------------------------

  SAT_PLUGIN_DEFAULT_CONSTRUCTOR(myPlugin);

  //----------

  bool init() final {
    registerDefaultExtensions();
    //registerExtension(CLAP_EXT_THREAD_POOL,&MThreadPoolExt);
    //registerExtension(CLAP_EXT_VOICE_INFO,&MVoiceInfoExt);
    //MVoiceManager.setProcessThreaded(false);
    //MVoiceManager.setEventMode(SAT_VOICE_EVENT_MODE_INTERLEAVED);
    appendClapNoteInputPort();
    appendStereoInputPort();
    appendStereoOutputPort();
    SAT_Parameter* par1 = appendParameter( new SAT_Parameter("Param1",0.0) );
    SAT_Parameter* par2 = appendParameter( new SAT_Parameter("Param2",0.5) );
    SAT_Parameter* par3 = appendParameter( new SAT_Parameter("Param3",1.5) );
    par3->setFlag(CLAP_PARAM_IS_MODULATABLE);
    setInitialEditorSize(EDITOR_WIDTH,EDITOR_HEIGHT);
    //SAT_Host* host = getHost();
    //const clap_plugin_t*  clapplugin = getPlugin();
    //const clap_host_t* claphost = host->getHost();
    //MVoiceManager.init(clapplugin,claphost);

//    SAT_AudioProcessor* processor = new SAT_AudioProcessor(getParameters(),getProcessContext());
//    setAudioProcessor(processor);

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
    //MVoiceManager.activate(sample_rate,min_frames_count,max_frames_count);
    return SAT_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

  //----------

  //void thread_pool_exec(uint32_t task_index) final {
  //  MVoiceManager.thread_pool_exec(task_index);
  //}

  //----------

  //bool voice_info_get(clap_voice_info_t *info) final {
  //  info->voice_count     = NUM_VOICES;
  //  info->voice_capacity  = NUM_VOICES;
  //  info->flags           = CLAP_VOICE_INFO_SUPPORTS_OVERLAPPING_NOTES;
  //  return true;
  //}


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

    MRootPanel = new SAT_PanelWidget( SAT_Rect(0,0,EDITOR_WIDTH,EDITOR_HEIGHT) );
    AWindow->setRootWidget(MRootPanel);
    MRootPanel->setFillBackground(true);

    SAT_LogoWidget* logo = new SAT_LogoWidget(SAT_Rect(50,50,200,200));
    MRootPanel->appendChildWidget(logo);
    logo->setLogoColor(SAT_White);

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
    dragvalue->setSnapPos(0.75);
    dragvalue->setAutoHideCursor(false);
    dragvalue->setAutoLockCursor(true);

    SAT_SliderWidget* slider = new SAT_SliderWidget(SAT_Rect(50,360,200,20),"Param 3", 0.0);
    MRootPanel->appendChildWidget(slider);
    slider->setTextSize(12);
    slider->setBipolar(true);
    slider->setBipolarCenter(0.3);
    slider->setSnap(true);
    slider->setSnapPos(0.3);
    slider->setAutoHideCursor(true);
    slider->setAutoLockCursor(true);
    //slider->setModulation(0.25);
    slider->setDrawModulation(true);
    slider->setModulationColor( SAT_Color(1,1,1,0.25) );

    AEditor->connect(value,     getParameter(0));
    AEditor->connect(dragvalue, getParameter(1));
    AEditor->connect(slider,    getParameter(2));

    return true;
  }

//------------------------------
public:
//------------------------------

// TODO
//
//  void preProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) final {
//    MVoiceManager.preProcessEvents(in_events,out_events);
//  }
//
//  void postProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) final {
//    MVoiceManager.postProcessEvents(in_events,out_events);
//  }

  //----------

  bool handleNoteOn(const clap_event_note_t* event) final {
    //SAT_Print("\n");
    //MVoiceManager.processNoteOn(event);
    return true;
  }

  bool handleNoteOff(const clap_event_note_t* event) final {
    //SAT_Print("\n");
    //MVoiceManager.processNoteOff(event);
    return true;
  }

  bool handleNoteChoke(const clap_event_note_t* event) final {
    //SAT_Print("\n");
    //MVoiceManager.processNoteChoke(event);
    return true;
  }

  bool handleNoteExpression(const clap_event_note_expression_t* event) final {
    //SAT_Print("\n");
    //MVoiceManager.processNoteExpression(event);
    return true;
  }

  bool handleParamValue(const clap_event_param_value_t* event) final {
    //SAT_Print("\n");
    //MVoiceManager.processParamValue(event);
    return true;
  }

  bool handleParamMod(const clap_event_param_mod_t* event) final {
    //SAT_Print("\n");
    //MVoiceManager.processParamMod(event);
    return true;
  }

  bool handleTransport(const clap_event_transport_t* event) final {
    //SAT_Print("\n");
    return false;
  }

  bool handleMidi(const clap_event_midi_t* event) final {
    //SAT_Print("\n");
    //MVoiceManager.processMidi(event);
    return true;
  }

  bool handleMidiSysex(const clap_event_midi_sysex_t* event) final {
    //SAT_Print("\n");
    //MVoiceManager.processMidiSysex(event);
    return true;
  }

  bool handleMidi2(const clap_event_midi2_t* event) final {
    //SAT_Print("\n");
    //MVoiceManager.processMidi2(event);
    return true;
  }

//------------------------------
public:
//------------------------------

//  void processAudioBlock(SAT_ProcessContext* AContext) final {
//    //MVoiceManager.processAudioBlock(AContext);
//    const clap_process_t* process = AContext->process;
//    uint32_t length = process->frames_count;
//    float** inputs = process->audio_inputs[0].data32;
//    float** outputs = process->audio_outputs[0].data32;
//    SAT_CopyStereoBuffer(outputs,inputs,length);
//    sat_param_t scale = getParameterValue(2);
//    SAT_ScaleStereoBuffer(outputs,scale,length);
//  }

  //----------

  void processAudio(SAT_ProcessContext* AContext, uint32_t offset, uint32_t length) final {
    //MVoiceManager.processAudioBlock(AContext);
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

};

//----------------------------------------------------------------------
//
// register
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY
  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(myDescriptor,myPlugin);
#endif

