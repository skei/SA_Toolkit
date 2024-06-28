#ifndef sa_tyr_included
#define sa_tyr_included
//----------------------------------------------------------------------

#include "audio/sat_voice_manager.h"
#include "plugin/sat_plugin.h"

#if !defined (SAT_GUI_NOGUI)
  #include "plugin/sat_editor.h"
  #include "gui/sat_widgets.h"
#endif

#include "sa_tyr/sa_tyr_voice.h"

//----------

#define MAX_VOICES    32

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_tyr_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "skei.audio/sa_tyr",
  .name         = "sa_tyr",
  .vendor       = "skei.audio",
  .url          = "https://github.com/skei/SA_Toolkit",
  .manual_url   = "",
  .support_url  = "",
  .version      = "0.0.0",
  .description  = "",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_INSTRUMENT, nullptr }
};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class sa_tyr_plugin
: public SAT_Plugin {

//------------------------------
private:
//------------------------------

  SAT_VoiceManager<sa_tyr_voice,MAX_VOICES>  MVoiceManager = {};

//------------------------------
public:
//------------------------------

  SAT_DEFAULT_PLUGIN_CONSTRUCTOR(sa_tyr_plugin);

//------------------------------
public:
//------------------------------

  bool init() final {

    registerDefaultSynthExtensions();

    appendClapNoteInputPort("Note Input");
    appendStereoAudioInputPort("Audio Input");
    appendStereoAudioOutputPort("Audio Output");

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
  SAT_PLUGIN_ENTRY(sa_tyr_descriptor,sa_tyr_plugin);
#endif

//----------

#undef MAX_VOICES
#undef VOICE_SCALE

//----------------------------------------------------------------------
#endif
