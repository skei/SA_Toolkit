#ifndef sa_synth_included
#define sa_synth_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "audio/sat_audio_math.h"
#include "audio/sat_voice_context.h"
#include "audio/sat_voice_manager.h"
#include "plugin/sat_plugin.h"
#include "gui/sat_widgets.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#define PLUGIN_NAME   "sa_synth"
#define EDITOR_WIDTH  400
#define EDITOR_HEIGHT 300
#define EDITOR_SCALE  1.0
#define NUM_VOICES    64

#define VOICE_SCALE   0.1

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_synth_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/" PLUGIN_NAME,
  .name         = PLUGIN_NAME,
  .vendor       = SAT_VENDOR,
  .url          = "",
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
  .description  = "...",
  .features     = (const char* []) {
    CLAP_PLUGIN_FEATURE_INSTRUMENT,
    nullptr
  }
};

//----------------------------------------------------------------------
//
// voice
//
//----------------------------------------------------------------------

class sa_synth_voice {

//------------------------------
private:
//------------------------------

  uint32_t              MIndex      = 0;
  SAT_VoiceContext*     MContext    = nullptr;
  //SAT_ParameterArray*   MParameters = nullptr;
  
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

//------------------------------
public:
//------------------------------

  void init(uint32_t AIndex, SAT_VoiceContext* AContext) {
    MIndex = AIndex;
    MContext = AContext;
    MSampleRate = AContext->sample_rate;
    
//    SAT_Assert(MContext);
//    SAT_Assert(MContext->process_context);
//    SAT_Assert(MContext->process_context->parameters);
//    MParameters = MContext->process_context->parameters;
    
  }

  //----------
  
  sat_sample_t getVoiceLevel() {
    return 1.0;
  }

  //----------

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

  //----------

  uint32_t processSlice(uint32_t AState, uint32_t AOffset) {
    return process(AState,AOffset,SAT_AUDIO_QUANTIZED_SIZE);
  }

  //----------

  uint32_t noteOn(uint32_t AKey, double AVelocity) {

    SAT_ParameterArray* parameters = MContext->process_context->parameters;
    SAT_Parameter* par_tuning = parameters->getItem(1);

    MKey      = AKey;
    MVelocity = AVelocity;
    MPhase    = 0.0;
    MPTuning  = par_tuning->getValue();
    MMTuning  = 0.0;
    METuning  = 0.0;
    return SAT_VOICE_PLAYING;
  }

  //----------

  uint32_t noteOff(uint32_t AKey, double AVelocity) {
    return SAT_VOICE_FINISHED;
    //return SAT_VOICE_RELEASED;
  }

  //----------

  void noteChoke(uint32_t AKey, double AVelocity) {
  }

  //----------

  void noteExpression(uint32_t AExpression, double AValue) {
    switch (AExpression) {
      case CLAP_NOTE_EXPRESSION_PRESSURE:   MEPress  = AValue; break;
      case CLAP_NOTE_EXPRESSION_TUNING:     METuning = AValue; break;
      case CLAP_NOTE_EXPRESSION_BRIGHTNESS: MEBright = AValue; break;
    }
  }

  //----------

  void parameter(uint32_t AIndex, double AValue) {
    switch (AIndex) {
      case 0: MPGain    = AValue; break;
      case 1: MPTuning  = AValue; break;
      case 2: MPFilter  = AValue; break;
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

class sa_synth_plugin
: public SAT_Plugin {

//------------------------------
private:
//------------------------------

  SAT_VoiceManager<sa_synth_voice,NUM_VOICES> MVoiceManager = {};
  
//------------------------------
public:
//------------------------------

  SAT_PLUGIN_DEFAULT_CONSTRUCTOR(sa_synth_plugin);

//------------------------------
public: // clap
//------------------------------

  bool init() final {
    
    SAT_Print("id: '%s'\n",sa_synth_descriptor.id);
    //registerAllExtensions();
    registerDefaultSynthExtensions();
    registerExtension(CLAP_EXT_REMOTE_CONTROLS,&MRemoteControlsExt);
    registerExtension(CLAP_EXT_PARAM_INDICATION,&MParamIndicationExt);
    //registerExtension(CLAP_EXT_PRESET_LOAD,&MPresetLoadExt);
    clap_param_info_flags flags = CLAP_PARAM_IS_AUTOMATABLE
                                | CLAP_PARAM_IS_MODULATABLE
                                | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID;
    appendParameter(new SAT_Parameter("Gain",   "", 1,  0, 1, flags));  // 0
    appendParameter(new SAT_Parameter("Tuning", "", 0, -1, 1, flags));  // 1
    appendParameter(new SAT_Parameter("Filter", "", 1,  0, 1, flags));  // 2
    // ports
    appendClapNoteInputPort();
    appendStereoAudioOutputPort();
    // editor
    setInitialEditorSize(EDITOR_WIDTH,EDITOR_HEIGHT,EDITOR_SCALE);
    
    // voice manager
    SAT_Host* host = getHost();
    const clap_plugin_t* clapplugin = getClapPlugin();
    const clap_host_t* claphost = host->getHost();
    MVoiceManager.init(clapplugin,claphost);
    MVoiceManager.setProcessThreaded(true);
    MVoiceManager.setEventMode(SAT_PLUGIN_EVENT_MODE_QUANTIZED);
    
    bool result = SAT_Plugin::init();
    return result;
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
    info->voice_count     = NUM_VOICES;
    info->voice_capacity  = NUM_VOICES;
    info->flags           = CLAP_VOICE_INFO_SUPPORTS_OVERLAPPING_NOTES;
    return true;
  }

  //----------
  
  uint32_t remote_controls_count() final {
    return 1;
  }  
  
  //----------
  
  bool remote_controls_get(uint32_t page_index, clap_remote_controls_page_t *page) final {
    switch (page_index) {
      case 0: {
        strcpy(page->section_name,"Section");
        page->page_id = 0;
        strcpy(page->page_name,"SA_Synth parameters");
        page->param_ids[0] = 0;
        page->param_ids[1] = 1;
        page->param_ids[2] = 2;
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
  
  //----------

  bool preset_load_from_location(uint32_t location_kind, const char *location, const char *load_key) final {
    if (location_kind == CLAP_PRESET_DISCOVERY_LOCATION_FILE) {
      loadPresetFromFile(location);
      SAT_Host* host = getHost();
      if (host) host->preset_load_loaded(location_kind,location,load_key);
      return true;
    }
    return false;
  }

//------------------------------
public: // gui
//------------------------------

  bool initEditorWindow(SAT_Editor* AEditor) final {
    SAT_Window* window = AEditor->getWindow();
    SAT_PanelWidget* root = new SAT_PanelWidget( SAT_Rect(0,0,EDITOR_WIDTH,EDITOR_HEIGHT) );
    window->appendRootWidget(root);
    root->setFillBackground(true);
    root->setDrawBorder(false);
    return true;
  }
  
//------------------------------
public: // events
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
    //switch (AExpression) {
    //  case CLAP_NOTE_EXPRESSION_PRESSURE:   e_gain   = AValue; break;
    //  case CLAP_NOTE_EXPRESSION_TUNING:     e_tuning = AValue; break;
    //  case CLAP_NOTE_EXPRESSION_BRIGHTNESS: e_filter = AValue; break;
    //}
    MVoiceManager.handleNoteExpression(event);
    return true;
  }

  //----------

  bool handleParamValue(const clap_event_param_value_t* event) final {
    //switch (event->param_id) {
    //  case 0: p_gain    = AValue; break;
    //  case 1: p_tuning  = AValue; break;
    //  case 2: p_filter  = AValue; break;
    //}
    MVoiceManager.handleParamValue(event);
    return true;
  }

  //----------

  bool handleParamMod(const clap_event_param_mod_t* event) final {
    //switch (event->param_id) {
    //  case 0: m_gain    = AValue; break;
    //  case 1: m_tuning  = AValue; break;
    //  case 2: m_filter  = AValue; break;
    //}
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
public: // audio
//------------------------------

  void processAudio(SAT_ProcessContext* AContext) final {
    const clap_process_t* process = AContext->process;
    uint32_t length = process->frames_count;
    float** outputs = process->audio_outputs[0].data32;
    // voices
    AContext->voice_buffer = outputs;
    AContext->voice_length = length;
    MVoiceManager.processAudio(AContext);
    // we could process the voice buffers individually...
    MVoiceManager.mixActiveVoices();
    // gain
    //sat_param_t scale = getParameterValue(14) + getModulationValue(14);   // Gain
    //scale = SAT_Clamp(scale,0,1);
    //SAT_ScaleStereoBuffer(outputs,scale,length);
  }

};

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY

  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(sa_synth_descriptor,sa_synth_plugin);

#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#undef PLUGIN_NAME
#undef EDITOR_WIDTH
#undef EDITOR_HEIGHT
#undef EDITOR_SCALE
#undef NUM_VOICES
#undef VOICE_SCALE

//----------------------------------------------------------------------
#endif
