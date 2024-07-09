#ifndef sa_tyr_included
#define sa_tyr_included
//----------------------------------------------------------------------

#include "sat.h"
#include "audio/sat_audio_math.h"
#include "plugin/sat_parameters.h"
#include "plugin/sat_plugin.h"
#include "plugin/processor/sat_voice_processor.h"

#if !defined (SAT_GUI_NOGUI)
  #include "plugin/sat_editor.h"
  #include "gui/sat_widgets.h"
#endif

//----------

//#include "sa_tyr/sa_tyr_parameters.h"
#include "sa_tyr/sa_tyr_voice.h"

//----------------------------------------------------------------------

#define MAX_VOICES    32
#define EDITOR_WIDTH  430
#define EDITOR_HEIGHT (295 + 40 + 25)
#define EDITOR_SCALE  2

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_tyr_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/sa_tyr/v0",
  .name         = "sa_tyr",
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

//typedef SAT_VoiceProcessor<sa_tyr_voice,MAX_VOICES> sa_tyr_voice_processor;

//

class sa_tyr_voice_processor
: public SAT_VoiceProcessor<sa_tyr_voice,MAX_VOICES> {

//------------------------------
public:
//------------------------------

  sa_tyr_voice_processor(SAT_ProcessorOwner* AOwner)
  : SAT_VoiceProcessor(AOwner) {
    SAT_TRACE;
  }

  //----------

  virtual ~sa_tyr_voice_processor() {
    SAT_TRACE;
  }

//------------------------------
public:
//------------------------------

  void process(SAT_ProcessContext* AContext) override {
    // SAT_ClearStereoBuffer(output,length);
    // processEvents(process->in_events,process->out_events);
    // processAudio(AContext);
    // mixActiveVoices(output,length);
    SAT_VoiceProcessor::process(AContext);
    // const clap_process_t* process = AContext->process;
    // float** output = process->audio_outputs[0].data32;
    // uint32_t length = process->frames_count;
    // sat_param_t gain = AContext->parameters->getItem(SA_TYR_PARAM_GAIN)->getValue();
    // SAT_ScaleStereoBuffer(output,gain,length);
  }

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

  sa_tyr_voice_processor* MProcessor = nullptr;

//------------------------------
public:
//------------------------------

  sa_tyr_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~sa_tyr_plugin() {
  }

//------------------------------
public:
//------------------------------

  bool init() final {
    registerDefaultSynthExtensions();
    appendClapNoteInputPort("In");
    appendStereoAudioOutputPort("Out");
    MProcessor = new sa_tyr_voice_processor(this);
    setProcessor(MProcessor);
    MProcessor->init(getClapPlugin(),getClapHost());
    MProcessor->setProcessThreaded(true);
    MProcessor->setEventMode(SAT_VOICE_EVENT_MODE_QUANTIZED);
    //sa_tyr_SetupParameters(this);
    clap_param_info_flags flags = CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_MODULATABLE | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID;
    appendParameter(new SAT_Parameter("Gain",   "", 1,  0, 1, flags));  // 0
    appendParameter(new SAT_Parameter("Tuning", "", 0, -1, 1, flags));  // 1
    appendParameter(new SAT_Parameter("Filter", "", 1,  0, 1, flags));  // 2
    #if !defined (SAT_GUI_NOGUI)
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
public:
//------------------------------

  bool voice_info_get(clap_voice_info_t *info) override {
    //SAT_TRACE;
    info->voice_count     = MAX_VOICES;
    info->voice_capacity  = MAX_VOICES;
    info->flags           = CLAP_VOICE_INFO_SUPPORTS_OVERLAPPING_NOTES;
    return true;
  }

  //----------

  void thread_pool_exec(uint32_t task_index) override {
    MProcessor->threadPoolExec(task_index);
  }

  //----------

  // uint32_t remote_controls_count() final {
  //   return 1;
  // }  
  
  //----------

  // bool remote_controls_get(uint32_t page_index, clap_remote_controls_page_t *page) final {
  //   switch (page_index) {
  //     case 0: {
  //       strcpy(page->section_name,"Section");
  //       page->page_id = 0;
  //       strcpy(page->page_name,"SA_Synth parameters");
  //       page->param_ids[0] = SA_MAEL_PARAM_OSC1_SQU;
  //       page->param_ids[1] = SA_MAEL_PARAM_OSC1_TRI;
  //       page->param_ids[2] = SA_MAEL_PARAM_OSC1_SIN;
  //       page->param_ids[3] = SA_MAEL_PARAM_OSC1_WIDTH;
  //       page->param_ids[4] = SA_MAEL_PARAM_OSC2_SQU;
  //       page->param_ids[5] = SA_MAEL_PARAM_OSC2_TRI;
  //       page->param_ids[6] = SA_MAEL_PARAM_OSC2_SIN;
  //       page->param_ids[7] = SA_MAEL_PARAM_OSC2_WIDTH;
  //       page->is_for_preset = false;
  //       return true;
  //     }
  //   }
  //   return false;
  // }  
  
  //----------

  // bool preset_load_from_location(uint32_t location_kind, const char *location, const char *load_key) final {
  //   if (location_kind == CLAP_PRESET_DISCOVERY_LOCATION_FILE) {
  //     loadPresetFromFile(location,load_key);
  //     SAT_Host* host = getHost();
  //     if (host && host->ext.preset_load) host->ext.preset_load->loaded(getClapHost(),location_kind,location,load_key);
  //     return true;
  //   }
  //   return false;
  // }

  //----------

  #ifndef SAT_NO_GUI
    #include "sa_tyr/sa_tyr_gui.h"
  #endif

  //----------

  // void on_editorListener_timer(SAT_Timer* ATimer, double ADelta) override {
  //   SAT_Plugin::on_editorListener_timer(ATimer,ADelta);
  //   for (uint32_t i=0; i<MAX_VOICES; i++) {
  //     uint32_t state = MProcessor->getVoiceState(i);
  //     voices->setVoiceState(i,state);
  //   }
  //   voices->do_widget_redraw(voices);
  // }  

};

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
  SAT_PLUGIN_ENTRY(sa_tyr_descriptor,sa_tyr_plugin)
#endif

#undef MAX_VOICES

//----------------------------------------------------------------------
#endif
