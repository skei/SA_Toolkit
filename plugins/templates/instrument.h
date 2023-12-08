

#include "plugin/sat_plugin.h"
#include "audio/sat_audio_math.h"
#include "audio/sat_voice_manager.h"

#if !defined (SAT_GUI_NOGUI)
  #include "plugin/sat_editor.h"
  #include "gui/sat_widgets.h"
#endif

//----------

  #define EDITOR_WIDTH  256
  #define EDITOR_HEIGHT 256
  #define EDITOR_SCALE  1.0

  #define MAX_VOICES    32
  #define VOICE_SCALE   0.1

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t myDescriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "me/mySynth",
  .name         = "my synth",
  .vendor       = "me",
  .url          = "https://website.com/mySynth",
  .manual_url   = "https://website.com/mySynth/manual.pdf",
  .support_url  = "https://website.com/support",
  .version      = "0.0.0",
  .description  = "my precious synth",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_INSTRUMENT, nullptr }
};

//----------------------------------------------------------------------
//
// voice
//
//----------------------------------------------------------------------

class myVoice {

private:

  uint32_t              MIndex      = 0;
  SAT_VoiceContext*     MContext    = nullptr;
  
  float                 MSampleRate = 0.0;
  double                MKey        = 0.0;
  double                MVelocity   = 1.0;
  float                 MPhase      = 0.0;
  float                 MPhaseAdd   = 0.0;
  
  double                MPGain      = 1.0;
  double                MPTuning    = 0.0;
  double                MPFilter    = 1.0;

  double                MMGain      = 0.0;
  double                MMTuning    = 0.0;
  double                MMFilter    = 0.0;

  double                MEPress     = 0.0;
  double                METuning    = 0.0;
  double                MEBright    = 0.0;

public:

  void init(uint32_t AIndex, SAT_VoiceContext* AContext) {
    MIndex = AIndex;
    MContext = AContext;
    MSampleRate = AContext->sample_rate;
  }

  sat_sample_t getVoiceLevel() {
    return 1.0;
  }

  uint32_t process(uint32_t AState, uint32_t AOffset, uint32_t ALength) {
    float* buffer = MContext->voice_buffer;
    buffer += (MIndex * SAT_PLUGIN_MAX_BLOCK_SIZE);
    buffer += AOffset;
    if ((AState == SAT_VOICE_PLAYING) || (AState == SAT_VOICE_RELEASED)) {
      for (uint32_t i=0; i<ALength; i++) {
        MPhase = SAT_Fract(MPhase);
        float v = (MPhase * 2.0) - 1.0;  // 0..1 -> -1..1
        double env = 1.0;
        *buffer++ = v * env * VOICE_SCALE;
        // update
        double tu = MPTuning + MMTuning;
        tu = SAT_Clamp(tu,-1,1);
        tu +=  METuning;
        float hz = SAT_NoteToHz(MKey + tu);
        MPhaseAdd = 1.0 / SAT_HzToSamples(hz,MSampleRate);
        MPhase += MPhaseAdd;
      }
    }
    else {
      memset(buffer,0,ALength * sizeof(float));
    }
    return AState;
  }

  uint32_t processSlice(uint32_t AState, uint32_t AOffset) {
    return process(AState,AOffset,SAT_AUDIO_QUANTIZED_SIZE);
  }

  uint32_t noteOn(uint32_t AIndex, double AValue) {
    SAT_Plugin*     plugin      = MContext->process_context->plugin;
    SAT_Parameter*  par_tuning  = plugin->getParameter(1);
    MKey      = AIndex;
    MVelocity = AValue;
    MPhase    = 0.0;
    MPTuning  = par_tuning->getValue();
    MMTuning  = 0.0;
    METuning  = 0.0;
    return SAT_VOICE_PLAYING;
  }

  uint32_t noteOff(uint32_t AIndex, double AValue) {
    return SAT_VOICE_FINISHED;
    //return SAT_VOICE_RELEASED;
  }

  void noteChoke(uint32_t AIndex, double AValue) {
  }

  void noteExpression(uint32_t AIndex, double AValue) {
    switch (AIndex) {
      case CLAP_NOTE_EXPRESSION_PRESSURE:   MEPress  = AValue; break;
      case CLAP_NOTE_EXPRESSION_TUNING:     METuning = AValue; break;
      case CLAP_NOTE_EXPRESSION_BRIGHTNESS: MEBright = AValue; break;
    }
  }

  void parameter(uint32_t AIndex, double AValue) {
    switch (AIndex) {
      case 0: MPGain    = AValue; break;
      case 1: MPTuning  = AValue; break;
      case 2: MPFilter  = AValue; break;
    }
  }

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
// synth
//
//----------------------------------------------------------------------

class mySynth
: public SAT_Plugin {

//------------------------------
private:
//------------------------------

  SAT_VoiceManager<myVoice,MAX_VOICES>  MVoiceManager = {};

//------------------------------
public:
//------------------------------

  SAT_DEFAULT_PLUGIN_CONSTRUCTOR(mySynth);

//------------------------------
public:
//------------------------------

  bool init() final {
    registerDefaultSynthExtensions();
    registerExtension(CLAP_EXT_PARAM_INDICATION,&MExtParamIndication);
    registerExtension(CLAP_EXT_PRESET_LOAD,&MExtPresetLoad);
    registerExtension(CLAP_EXT_REMOTE_CONTROLS,&MExtRemoteControls);
    registerExtension(CLAP_EXT_TRACK_INFO,&MExtTrackInfo);
    appendClapNoteInputPort("Input");
    appendStereoAudioOutputPort("Output");
    clap_param_info_flags flags = CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_MODULATABLE | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID;
    appendParameter(new SAT_Parameter("Gain",   1,  0, 1, flags));  // 0
    appendParameter(new SAT_Parameter("Tuning", 0, -1, 1, flags));  // 1
    appendParameter(new SAT_Parameter("Filter", 1,  0, 1, flags));  // 2
    #if !defined (SAT_GUI_NOGUI)
      setInitialEditorSize(EDITOR_WIDTH,EDITOR_HEIGHT,EDITOR_SCALE);
    #endif
    MVoiceManager.init(getClapPlugin(),getClapHost());
    MVoiceManager.setProcessThreaded(true);
    MVoiceManager.setEventMode(SAT_PLUGIN_EVENT_MODE_QUANTIZED);
    return SAT_Plugin::init();
  }

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MVoiceManager.activate(sample_rate,min_frames_count,max_frames_count);
    return SAT_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

  void thread_pool_exec(uint32_t task_index) final {
    MVoiceManager.threadPoolExec(task_index);
  }

  bool voice_info_get(clap_voice_info_t *info) final {
    info->voice_count     = MAX_VOICES;
    info->voice_capacity  = MAX_VOICES;
    info->flags           = CLAP_VOICE_INFO_SUPPORTS_OVERLAPPING_NOTES;
    return true;
  }

  uint32_t remote_controls_count() final {
    return 1;
  }  
  
  bool remote_controls_get(uint32_t page_index, clap_remote_controls_page_t *page) final {
    switch (page_index) {
      case 0: {
        strcpy(page->section_name,"Section");
        page->page_id = 0;
        strcpy(page->page_name,"SA_Synth parameters");
        page->param_ids[0] = CLAP_INVALID_ID;
        page->param_ids[1] = CLAP_INVALID_ID;
        page->param_ids[2] = CLAP_INVALID_ID;
        page->param_ids[3] = CLAP_INVALID_ID;
        page->param_ids[4] = CLAP_INVALID_ID;
        page->param_ids[5] = CLAP_INVALID_ID;
        page->param_ids[6] = CLAP_INVALID_ID;
        page->param_ids[7] = CLAP_INVALID_ID;
        page->is_for_preset = false;
        return true;
      }
    }
    return false;
  }  
  
  bool preset_load_from_location(uint32_t location_kind, const char *location, const char *load_key) final {
    if (location_kind == CLAP_PRESET_DISCOVERY_LOCATION_FILE) {
      loadPresetFromFile(location,load_key);
      SAT_Host* host = getHost();
      if (host && host->ext.preset_load) host->ext.preset_load->loaded(getClapHost(),location_kind,location,load_key);
      return true;
    }
    return false;
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

#include "plugin/sat_entry.h"
SAT_PLUGIN_ENTRY(myDescriptor,mySynth);
