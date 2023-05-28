#ifndef sa_synth_included
#define sa_synth_included
//----------------------------------------------------------------------

//#define SAT_PLUGIN_CLAP
//#define SAT_PLUGIN_VST2
//#define SAT_PLUGIN_VST3

//----------

#include "base/sat.h"
#include "audio/sat_audio_math.h"
#include "audio/sat_audio_utils.h"
#include "audio/sat_voice_manager.h"
#include "plugin/sat_plugin.h"
#include "gui/sat_widgets.h"

#include "sa_synth/sa_synth_voice.h"
//#include "sa_synth/sa_synth_widgets.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#define PLUGIN_NAME   "sa_synth"
#define EDITOR_WIDTH  225
#define EDITOR_HEIGHT 145
#define EDITOR_SCALE  3.0
#define NUM_VOICES    32

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
// plugin
//
//----------------------------------------------------------------------

class sa_synth_plugin
: public SAT_Plugin {

//------------------------------
private:
//------------------------------

  SAT_VoiceManager<sa_synth_voice,NUM_VOICES> MVoiceManager   = {};
//  SAT_VoicesWidget*                             MVoicesWidget   = nullptr;
//  SAT_WaveformWidget*                           MWaveformWidget = nullptr;
  
//------------------------------
public:
//------------------------------

  SAT_PLUGIN_DEFAULT_CONSTRUCTOR(sa_synth_plugin);

//------------------------------
public:
//------------------------------

  bool init() final {
    
    SAT_Print("id: '%s'\n",sa_synth_descriptor.id);
    
    // extensions
    registerAllExtensions();
    
    // ports
    appendClapNoteInputPort();
    appendStereoAudioOutputPort();
    
    // parameters
    appendParameter(new SAT_TextParameter("wf",   0, 0, 4, sa_synth_osc_type_text ));     // 0
  //appendParameter(new SAT_IntParameter( "tu",   0, -12, 12 ));                          // 1
    appendParameter(new SAT_Parameter(    "tu",   0, -2, 2 ));                            // 1
    appendParameter(new SAT_Parameter(    "A",    0   ));                                 // 2
    appendParameter(new SAT_Parameter(    "D",    0   ));                                 // 3
    appendParameter(new SAT_Parameter(    "S",    1   ));                                 // 4
    appendParameter(new SAT_Parameter(    "R",    0   ));                                 // 5
    appendParameter(new SAT_TextParameter("flt",  0, 0, 4, sa_synth_flt_type_text ));     // 6
    appendParameter(new SAT_Parameter(    "fr",   1   ));                                 // 7
    appendParameter(new SAT_Parameter(    "bw",   0.5 ));                                 // 8
    appendParameter(new SAT_Parameter(    "A",    0   ));                                 // 9
    appendParameter(new SAT_Parameter(    "D",    0   ));                                 // 10
    appendParameter(new SAT_Parameter(    "S",    1   ));                                 // 11
    appendParameter(new SAT_Parameter(    "R",    0   ));                                 // 12
    appendParameter(new SAT_Parameter(    "vol",  1   ));                                 // 13
    
    setAllParameterFlags(CLAP_PARAM_IS_MODULATABLE);
    //setAllParameterFlags(CLAP_PARAM_IS_MODULATABLE_PER_PORT);
    //setAllParameterFlags(CLAP_PARAM_IS_MODULATABLE_PER_CHANNEL);
    //setAllParameterFlags(CLAP_PARAM_IS_MODULATABLE_PER_KEY);
    setAllParameterFlags(CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID);
    
    // voice manager
    SAT_Host* host = getHost();
    const clap_plugin_t* clapplugin = getPlugin();
    const clap_host_t* claphost = host->getHost();
    MVoiceManager.init(clapplugin,claphost);
    MVoiceManager.setProcessThreaded(true);
    MVoiceManager.setEventMode(SAT_PLUGIN_EVENT_MODE_INTERLEAVED);
    
    // editor
    setInitialEditorSize(EDITOR_WIDTH,EDITOR_HEIGHT,EDITOR_SCALE);
    
    return SAT_Plugin::init();
  }

  //----------
  
  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    SAT_Print("sample_rate %.2f min_frames_count %i max_frames_count %i\n",sample_rate,min_frames_count,max_frames_count);
    MVoiceManager.activate(sample_rate,min_frames_count,max_frames_count);
    return SAT_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

  //----------

  void thread_pool_exec(uint32_t task_index) final {
    MVoiceManager.thread_pool_exec(task_index);
  }

  //----------

  bool voice_info_get(clap_voice_info_t *info) final {
    SAT_Print("-> %i (CLAP_VOICE_INFO_SUPPORTS_OVERLAPPING_NOTES)\n",NUM_VOICES);
    info->voice_count     = NUM_VOICES;
    info->voice_capacity  = NUM_VOICES;
    info->flags           = CLAP_VOICE_INFO_SUPPORTS_OVERLAPPING_NOTES;
    return true;
  }

  //----------
  
  uint32_t remote_controls_count() override {
    SAT_PRINT;
    return 1;
  }  
  
  //----------
  
  bool remote_controls_get(uint32_t page_index, clap_remote_controls_page_t *page) override {
    SAT_PRINT;
    switch (page_index) {
      case 0: {
        strcpy(page->section_name,"Section");
        page->page_id = 0;
        strcpy(page->page_name,"Parameters");
        page->param_ids[0] = 0;
        page->param_ids[1] = 1;
        page->param_ids[2] = CLAP_INVALID_ID;
        page->param_ids[3] = 13;
        page->param_ids[4] = 6;
        page->param_ids[5] = 7;
        page->param_ids[6] = 8;
        page->param_ids[7] = CLAP_INVALID_ID;
        page->is_for_preset = false;
        SAT_PRINT;
        return true;
      }
    }
    SAT_PRINT;
    return false;
  }  
  
  //----------

  bool preset_load_from_location(uint32_t location_kind, const char *location, const char *load_key) override {
    if (location_kind == CLAP_PRESET_DISCOVERY_LOCATION_FILE) {
      SAT_Print("location '%s', load_key '%s'\n",location,load_key);
      if (location_kind == CLAP_PRESET_DISCOVERY_LOCATION_FILE) {
        loadPresetFromFile(location);
        SAT_Host* host = getHost();
        if (host) host->preset_load_loaded(location_kind,location,load_key);
        return true;
      }
    }
    return false;
  }

//------------------------------
public: // gui
//------------------------------

  #include "sa_synth/sa_synth_gui.h"
  
//------------------------------
public: // timer
//------------------------------

  void do_editorListener_timer() final {
    SAT_Plugin::do_editorListener_timer();
//    //update voices widget.. (move to widget itself (+ register timer)
//    // before or after sat_plugin:: ?
//    for (uint32_t voice=0; voice<NUM_VOICES; voice++) {
//      uint32_t state = MVoiceManager.getVoiceState(voice);
//      MVoicesWidget->setVoiceState(voice,state);
//    }
//    MVoicesWidget->parentRedraw();
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
public: // audio
//------------------------------

  // block (synth)

  void processAudio(SAT_ProcessContext* AContext) final {
    const clap_process_t* process = AContext->process;
    uint32_t length = process->frames_count;
    float** outputs = process->audio_outputs[0].data32;
    
    // process voices
    AContext->voice_buffer = outputs;
    AContext->voice_length = length;
    MVoiceManager.processAudio(AContext);

    // scale outputs
    sat_param_t scale = getParameterValue(13) + getModulationValue(13);   // Gain
    scale = SAT_Clamp(scale,0,1);
    SAT_ScaleStereoBuffer(outputs,scale,length);

  }

  //----------

  // interleaved

  /*
  void processAudio(SAT_ProcessContext* AContext, uint32_t offset, uint32_t length) final {
    const clap_process_t* process = AContext->process;

    // copy inputs to outputs
    float* inputs[2];
    float* outputs[2];
    inputs[0]  = process->audio_inputs[0].data32[0] + offset;
    inputs[1]  = process->audio_inputs[0].data32[1] + offset;
    outputs[0] = process->audio_outputs[0].data32[0] + offset;
    outputs[1] = process->audio_outputs[0].data32[1] + offset;
    SAT_CopyStereoBuffer(outputs,inputs,length);

    // scale outputs
    sat_param_t scale = getParameterValue(2) + getModulationValue(2);
    scale = SAT_Clamp(scale,0,1);
    SAT_ScaleStereoBuffer(outputs,scale,length);
  }
  */

  //----------

  // quantized

  /*
  void processAudio(SAT_ProcessContext* AContext, uint32_t offset) final {
    processAudio(AContext,offset,SAT_AUDIO_QUANTIZED_SIZE);
  }
  */

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

//----------------------------------------------------------------------
#endif
