#ifndef sa_mael_included
#define sa_mael_included
//----------------------------------------------------------------------

#include "sat.h"
#include "audio/sat_audio_math.h"
#include "plugin/sat_parameters.h"
#include "plugin/sat_plugin.h"
#include "plugin/processor/sat_voice_processor.h"

#if !defined (SAT_NO_GUI)
  #include "plugin/sat_editor.h"
  #include "gui/sat_widgets.h"
#endif

//----------

#include "sa_mael/sa_mael_parameters.h"
#include "sa_mael/sa_mael_voice.h"

//----------------------------------------------------------------------

#define MAX_VOICES    1024
#define EDITOR_WIDTH  430
#define EDITOR_HEIGHT 360
#define EDITOR_SCALE  2

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_mael_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/sa_mael/v0",
  .name         = "sa_mael",
  .vendor       = SAT_VENDOR,
  .url          = SAT_URL,
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
  .description  = "",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_INSTRUMENT, nullptr }
};

//----------------------------------------------------------------------
//
// processor
//
//----------------------------------------------------------------------

//typedef SAT_VoiceProcessor<sa_mael_voice,MAX_VOICES> sa_mael_voice_processor;

//

class sa_mael_voice_processor
: public SAT_VoiceProcessor<sa_mael_voice,MAX_VOICES> {

//------------------------------
public:
//------------------------------

  sa_mael_voice_processor(SAT_ProcessorListener* AListener)
  : SAT_VoiceProcessor(AListener) {
    SAT_TRACE;
  }

  //----------

  virtual ~sa_mael_voice_processor() {
    SAT_TRACE;
  }

//------------------------------
public:
//------------------------------

  void process(SAT_ProcessContext* AContext) override {
    SAT_VoiceProcessor::process(AContext);
    const clap_process_t* process = AContext->process;
    float** output = process->audio_outputs[0].data32;
    uint32_t length = process->frames_count;
    sat_param_t gain = AContext->parameters->getItem(SA_MAEL_PARAM_GAIN)->getValue();
    SAT_ScaleStereoBuffer(output,gain,length);
  }

};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class sa_mael_plugin
: public SAT_Plugin {
  
//------------------------------
private:
//------------------------------

  sa_mael_voice_processor* MProcessor = nullptr;

//------------------------------
public:
//------------------------------

  // todo, macro:
  // SAT_VOICE_PROCESSOR_DEFAULT_CONSTRUCTOR(sa_mael_plugin)
  // don't do anything here..

  //----------

  sa_mael_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~sa_mael_plugin() {
  }

//------------------------------
public: // clap
//------------------------------

  bool init() final {
    registerDefaultSynthExtensions();
    registerExtension(CLAP_EXT_PARAM_INDICATION);
    //registerExtension(CLAP_EXT_PRESET_LOAD);
    registerExtension(CLAP_EXT_REMOTE_CONTROLS);
    registerExtension(CLAP_EXT_TRACK_INFO);

    appendClapNoteInputPort("In");
    appendStereoAudioOutputPort("Out");

    SAT_TRACE;
    MProcessor = new sa_mael_voice_processor(this);
    SAT_TRACE;
    
    setProcessor(MProcessor);
    MProcessor->init(getClapPlugin(),getClapHost());
    
    #ifdef SAT_VOICE_PROCESSOR_THREADED
      MProcessor->setProcessThreaded(true);
    #else
      MProcessor->setProcessThreaded(false);
    #endif
    MProcessor->setEventMode(SAT_VOICE_EVENT_MODE_INTERLEAVED);

    sa_mael_SetupParameters(this);

    #if !defined (SAT_NO_GUI)
      setInitialEditorSize(EDITOR_WIDTH,EDITOR_HEIGHT,EDITOR_SCALE,true);
    #endif

    return SAT_Plugin::init();
  }
  
  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MProcessor->activate(sample_rate,min_frames_count,max_frames_count);
    return SAT_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

//------------------------------
public: // voice info
//------------------------------

  bool voice_info_get(clap_voice_info_t *info) override {
    //SAT_TRACE;
    info->voice_count     = MAX_VOICES;
    info->voice_capacity  = MAX_VOICES;
    info->flags           = CLAP_VOICE_INFO_SUPPORTS_OVERLAPPING_NOTES;
    return true;
  }

//------------------------------
public: // thread pool
//------------------------------

  void thread_pool_exec(uint32_t task_index) override {
    MProcessor->threadPoolExec(task_index);
  }

//------------------------------
public: // remote controls
//------------------------------

  uint32_t remote_controls_count() final {
    return 1;
  }  
  
  //----------

  bool remote_controls_get(uint32_t page_index, clap_remote_controls_page_t *page) final {
    switch (page_index) {
      case 0: {
        strcpy(page->section_name,"Section");
        page->page_id = 0;
        strcpy(page->page_name,"sa_mael parameters");
        page->param_ids[0] = SA_MAEL_PARAM_OSC1_SQU;
        page->param_ids[1] = SA_MAEL_PARAM_OSC1_TRI;
        page->param_ids[2] = SA_MAEL_PARAM_OSC1_SIN;
        page->param_ids[3] = SA_MAEL_PARAM_OSC1_WIDTH;
        page->param_ids[4] = SA_MAEL_PARAM_OSC2_SQU;
        page->param_ids[5] = SA_MAEL_PARAM_OSC2_TRI;
        page->param_ids[6] = SA_MAEL_PARAM_OSC2_SIN;
        page->param_ids[7] = SA_MAEL_PARAM_OSC2_WIDTH;
        page->is_for_preset = false;
        return true;
      }
    }
    return false;
  }  
  
//------------------------------
public: // preset load
//------------------------------

  // bool preset_load_from_location(uint32_t location_kind, const char *location, const char *load_key) final {
  //   if (location_kind == CLAP_PRESET_DISCOVERY_LOCATION_FILE) {
  //     loadPresetFromFile(location,load_key);
  //     SAT_Host* host = getHost();
  //     if (host && host->ext.preset_load) host->ext.preset_load->loaded(getClapHost(),location_kind,location,load_key);
  //     return true;
  //   }
  //   return false;
  // }

//------------------------------
public: // gui
//------------------------------

  #ifndef SAT_NO_GUI
    #include "sa_mael/sa_mael_editor.h"
  #endif

//------------------------------
public: // timer
//------------------------------

  #ifdef SAT_EDITOR_EMBEDDED
  
    void on_EditorListener_timer(double ADelta) override {
      SAT_Plugin::on_EditorListener_timer(ADelta);
      for (uint32_t i=0; i<MAX_VOICES; i++) {
        uint32_t state = MProcessor->getVoiceState(i);
        voices->setVoiceState(i,state);
      }
      voices->setNumPlayingVoices(MProcessor->getNumPlayingVoices());
      voices->setNumReleasedVoices(MProcessor->getNumReleasedVoices());
      voices->do_Widget_redraw(voices);
    }

  #endif

};

//----------------------------------------------------------------------

#undef MAX_VOICES
#undef EDITOR_WIDTH
#undef EDITOR_HEIGHT
#undef EDITOR_SCALE

//----------------------------------------------------------------------
//
// entry point
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY
  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(sa_mael_descriptor,sa_mael_plugin)
#endif

//----------------------------------------------------------------------
#endif


