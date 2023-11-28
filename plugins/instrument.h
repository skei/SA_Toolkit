

#include "plugin/sat_plugin.h"
#include "plugin/sat_editor.h"
#include "gui/sat_widgets.h"
#include "audio/sat_voice_manager.h"

//----------

#define MAX_VOICES    32
#define EDITOR_WIDTH  256
#define EDITOR_HEIGHT 256
#define EDITOR_SCALE  1.0

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

public:

  sat_sample_t getVoiceLevel() {
    return 0.0;
  }

  uint32_t process(uint32_t state, uint32_t offset, uint32_t length) {
    return 0;
  }

  uint32_t processSlice(uint32_t state, uint32_t offset) {
    return 0;
  }

  uint32_t noteOn(uint32_t AIndex, double AValue) {
    return 0;
  }

  uint32_t noteOff(uint32_t AIndex, double AValue) {
    return 0;
  }

  void noteChoke(uint32_t AIndex, double AValue) {
  }

  void noteExpression(uint32_t AIndex, double AValue) {
  }

  void parameter(uint32_t AIndex, double AValue) {
  }

  void modulation(uint32_t AIndex, double AValue) {
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
    registerDefaultExtensions();
    appendStereoAudioInputPort("Input");
    appendStereoAudioOutputPort("Output");
    appendParameter( new SAT_Parameter("Gain",0.5) );
    setInitialEditorSize(EDITOR_WIDTH,EDITOR_HEIGHT,EDITOR_SCALE);
    MVoiceManager.init(getClapPlugin(),getClapHost());
    return SAT_Plugin::init();
  }

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    return SAT_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

  void thread_pool_exec(uint32_t task_index) final {
    MVoiceManager.threadPoolExec(task_index);
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

  void processAudio(SAT_ProcessContext* AContext) final {
    MVoiceManager.processAudio(AContext);
    MVoiceManager.mixActiveVoices();
  }

//------------------------------
public: // gui
//------------------------------

  void setupEditorWindow(SAT_Editor* AEditor, SAT_Window* AWindow) final {
    SAT_RootWidget* root = new SAT_RootWidget(0,AWindow);
    AWindow->setRootWidget(root);
    //SAT_KnobWidget* knob = new SAT_KnobWidget(SAT_Rect(50,50,100,100),"Gain",0.1);
    //root->appendChildWidget(knob);
    //knob->setTextSize(15);
    //knob->setValueSize(25);
    //AEditor->connect(knob,getParameter(0));
  }

};

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

#include "plugin/sat_entry.h"
SAT_PLUGIN_ENTRY(myDescriptor,mySynth);


