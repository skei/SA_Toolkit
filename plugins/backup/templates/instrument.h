
#include "plugin/sat_plugin.h"

#include "audio/sat_audio_math.h"
#include "audio/sat_voice_manager.h"
#include "audio/modulation/sat_exp_envelope.h"

#if !defined (SAT_GUI_NOGUI)
  #include "plugin/sat_editor.h"
  #include "gui/sat_widgets.h"
#endif

//----------

#define MAX_VOICES    32
#define VOICE_SCALE   0.1

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t instrument_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "sat_instrument",
  .name         = "instrument",
  .vendor       = "skei.audio",
  .url          = "https://github.com/skei/SA_Toolkit",
  .manual_url   = "",
  .support_url  = "",
  .version      = "0.0.1",
  .description  = "",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_INSTRUMENT, nullptr }
};

//----------------------------------------------------------------------
//
// voice
//
//----------------------------------------------------------------------

class myVoice {

//------------------------------
private:
//------------------------------

  uint32_t                        MIndex      = 0;
  SAT_VoiceContext*               MContext    = nullptr;
  float                           MSampleRate = 0.0;

  SAT_ExpEnvelope<sat_sample_t>   MEnvelope     = {};  

  // note on
  double                          MKey        = 0.0;
  double                          MVelocity   = 1.0;

  // synth
  float                           MPhase      = 0.0;
  float                           MPhaseAdd   = 0.0;
  
  // parameters
  double                          MPGain      = 1.0;
  double                          MPTuning    = 0.0;
  double                          MPFilter    = 1.0;

  // modulation
  double                          MMGain      = 0.0;
  double                          MMTuning    = 0.0;
  double                          MMFilter    = 0.0;

  // note expressions
  double                          MEPress     = 0.0;
  double                          METuning    = 0.0;
  double                          MEBright    = 0.0;

//------------------------------
public:
//------------------------------

  void init(uint32_t AIndex, SAT_VoiceContext* AContext) {
    MIndex = AIndex;
    MContext = AContext;
    MSampleRate = AContext->sample_rate;
    MEnvelope.setSampleRate(MSampleRate);
    MEnvelope.setADSR( 0.0, 0.0, 1.0, 2.5 );

  }

  //----------

  sat_sample_t getVoiceLevel() {
    return MEnvelope.getValue();
  }

  //----------

  uint32_t process(uint32_t AState, uint32_t AOffset, uint32_t ALength) {
    // calc position of this voice in (big) buffer
    float* buffer = MContext->voice_buffer;
    buffer += (MIndex * SAT_PLUGIN_MAX_BLOCK_SIZE);
    buffer += AOffset;
    // calc samples
    if ((AState == SAT_VOICE_PLAYING) || (AState == SAT_VOICE_RELEASED)) {
      for (uint32_t i=0; i<ALength; i++) {
        // naive phase (0..1)-> saw waveform (-1..1)
        MPhase = SAT_Fract(MPhase);
        float v = (MPhase * 2.0) - 1.0;
        // todo: polyblep
        sat_sample_t env = MEnvelope.process();
        *buffer++ = v * env * VOICE_SCALE;
        // calc tuning (key + tuning param + modulation + expression)
        double tu = MPTuning + MMTuning;
        tu = SAT_Clamp(tu,-1,1);
        tu +=  METuning;
        float hz = SAT_NoteToHz(MKey + tu);
        // update synth phase
        MPhaseAdd = 1.0 / SAT_HzToSamples(hz,MSampleRate);
        MPhase += MPhaseAdd;
      }
    }
    else {
      // no voices -> silent output
      memset(buffer,0,ALength * sizeof(float));
    }
    // continue..
    if (MEnvelope.getStage() == SAT_ENVELOPE_FINISHED) return SAT_VOICE_FINISHED;
    else return AState;

  }

  //----------

  uint32_t processSlice(uint32_t AState, uint32_t AOffset) {
    return process(AState,AOffset,SAT_AUDIO_QUANTIZED_SIZE);
  }

  //----------

  uint32_t noteOn(uint32_t AIndex, double AValue) {
    SAT_Plugin*     plugin      = MContext->process_context->plugin;
    SAT_Parameter*  par_tuning  = plugin->getParameter(1);
    // reset voice
    MKey      = AIndex;
    MVelocity = AValue;
    MPhase    = 0.0;
    MPTuning  = par_tuning->getValue();
    MMTuning  = 0.0;
    METuning  = 0.0;
    MEnvelope.reset();
    MEnvelope.setADSR(0,0,1,2.5);
    MEnvelope.noteOn();    
    return SAT_VOICE_PLAYING;
  }

  //----------

  uint32_t noteOff(uint32_t AIndex, double AValue) {
    MEnvelope.noteOff();
    return SAT_VOICE_RELEASED;
    //return SAT_VOICE_FINISHED;
  }

  //----------

  void noteChoke(uint32_t AIndex, double AValue) {
    // todo
  }

  //----------

  void noteExpression(uint32_t AIndex, double AValue) {
    switch (AIndex) {
      case CLAP_NOTE_EXPRESSION_PRESSURE:   MEPress  = AValue; break;
      case CLAP_NOTE_EXPRESSION_TUNING:     METuning = AValue; break;
      case CLAP_NOTE_EXPRESSION_BRIGHTNESS: MEBright = AValue; break;
    }
  }

  //----------

  void parameter(uint32_t AIndex, double AValue) {
    //sat_param_t a5 = (AValue * 5.0);
    switch (AIndex) {
      case 0: MPGain    = AValue; break;
      case 1: MPTuning  = AValue; break;
      case 2: MPFilter  = AValue; break;
      //case SA_MAEL_PARAM_ENV1_ATT:    MEnvelope.setAttack(a5);  break;
      //case SA_MAEL_PARAM_ENV1_DEC:    MEnvelope.setDecay(a5);   break;
      //case SA_MAEL_PARAM_ENV1_SUS:    MEnvelope.setSustain(a5); break;
      //case SA_MAEL_PARAM_ENV1_REL:    MEnvelope.setRelease(a5); break;    
    }
  }

  //----------

  void modulation(uint32_t AIndex, double AValue) {
    switch (AIndex) {
      case 0: MMGain    = AValue; break;
      case 1: MMTuning  = AValue; break;
      case 2: MMFilter  = AValue; break;
    }
  }

};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class instrument
: public SAT_Plugin {

//------------------------------
private:
//------------------------------

  SAT_VoiceManager<myVoice,MAX_VOICES>  MVoiceManager = {};

//------------------------------
public:
//------------------------------

  SAT_DEFAULT_PLUGIN_CONSTRUCTOR(instrument);

//------------------------------
public:
//------------------------------

  bool init() final {

    registerDefaultSynthExtensions();

    appendClapNoteInputPort("Input");
    appendStereoAudioOutputPort("Output");

    // all params mod per note id
    clap_param_info_flags flags = CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_MODULATABLE | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID;
    appendParameter(new SAT_Parameter("Gain",   1,  0, 1, flags));  // 0
    appendParameter(new SAT_Parameter("Tuning", 0, -1, 1, flags));  // 1
    appendParameter(new SAT_Parameter("Filter", 1,  0, 1, flags));  // 2

    #if !defined (SAT_GUI_NOGUI)
      setInitialEditorSize(256,256,1.0);
    #endif

    MVoiceManager.init(getClapPlugin(),getClapHost());
    MVoiceManager.setProcessThreaded(true);
    MVoiceManager.setEventMode(SAT_PLUGIN_EVENT_MODE_QUANTIZED);

    return SAT_Plugin::init();
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MVoiceManager.activate(sample_rate,min_frames_count,max_frames_count);
    return SAT_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

  //----------

  void thread_pool_exec(uint32_t task_index) final {
    MVoiceManager.threadPoolExec(task_index);
  }

  //----------

  bool voice_info_get(clap_voice_info_t *info) final {
    info->voice_count     = MAX_VOICES;
    info->voice_capacity  = MAX_VOICES;
    info->flags           = CLAP_VOICE_INFO_SUPPORTS_OVERLAPPING_NOTES;
    return true;
  }

//------------------------------
public: // events
//------------------------------

  bool on_plugin_noteOn(const clap_event_note_t* event) final {
    MVoiceManager.handleNoteOn(event);
    return true;
  }

  bool on_plugin_noteOff(const clap_event_note_t* event) final {
    MVoiceManager.handleNoteOff(event);
    return true;
  }

  bool on_plugin_noteChoke(const clap_event_note_t* event) final {
    MVoiceManager.handleNoteChoke(event);
    return true;
  }

  bool on_plugin_noteExpression(const clap_event_note_expression_t* event) final {
    MVoiceManager.handleNoteExpression(event);
    return true;
  }

  bool on_plugin_paramValue(const clap_event_param_value_t* event) final {
    MVoiceManager.handleParamValue(event);
    return true;
  }

  bool on_plugin_paramMod(const clap_event_param_mod_t* event) final {
    MVoiceManager.handleParamMod(event);
    return true;
  }

  //bool on_plugin_transport(const clap_event_transport_t* event) final {
  //  return true;
  //}

  bool on_plugin_midi(const clap_event_midi_t* event) final {
    MVoiceManager.handleMidi(event);
    return true;
  }

  bool on_plugin_midiSysex(const clap_event_midi_sysex_t* event) final {
    MVoiceManager.handleMidiSysex(event);
    return true;
  }

  bool on_plugin_midi2(const clap_event_midi2_t* event) final {
    MVoiceManager.handleMidi2(event);
    return true;
  }

  //----------

  void postProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) final {
    MVoiceManager.postProcessEvents(in_events,out_events);
  }

//------------------------------
public: // process
//------------------------------

  // block
  void processAudio(SAT_ProcessContext* AContext) final {
    const clap_process_t* process = AContext->process;
    uint32_t length = process->frames_count;
    float** outputs = process->audio_outputs[0].data32;
    AContext->voice_buffer = outputs;
    AContext->voice_length = length;
    MVoiceManager.processAudio(AContext);
    MVoiceManager.mixActiveVoices();
  }

//------------------------------
public: // gui
//------------------------------

  // #if !defined (SAT_GUI_NOGUI)
  //  
  // void setupEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) final {
  //   SAT_RootWidget* root = new SAT_RootWidget(AWindow);
  //   AWindow->setRootWidget(root);
  //   //SAT_KnobWidget* knob = new SAT_KnobWidget(SAT_Rect(50,50,100,100),"Gain",0.1);
  //   //root->appendChildWidget(knob);
  //   //knob->setTextSize(15);
  //   //knob->setValueSize(25);
  //   //AEditor->connect(knob,getParameter(0));
  // }
  //
  // #endif // nogui

};

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY
  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(instrument_descriptor,instrument);
#endif

//----------

#undef MAX_VOICES
#undef VOICE_SCALE
