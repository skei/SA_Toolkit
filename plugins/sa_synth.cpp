#ifndef sa_synth_included
#define sa_synth_included
//----------------------------------------------------------------------

#include "sat.h"
//#include "base/system/sat_cpu.h"
//#include "audio/process/sat_resampler.h"
#include "plugin/sat_plugin.h"
#include "plugin/processor/sat_voice_processor.h"

#ifndef SAT_NO_GUI
  #include "plugin/sat_editor.h"
#endif

//----------

#define SA_SYNTH_MAX_VOICES    256
#define SA_SYNTH_EDITOR_WIDTH  730
#define SA_SYNTH_EDITOR_HEIGHT 430
#define SA_SYNTH_EDITOR_SCALE  1.5

#include "sa_synth/sa_synth_parameters.h"
#include "sa_synth/sa_synth_voice.h"

#ifndef SAT_NO_GUI
  #include "sa_synth/sa_synth_widgets.h"
#endif
//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_synth_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/sa_synth/v0",
  .name         = "sa_synth",
  .vendor       = SAT_VENDOR,
  .url          = SAT_URL,
  .manual_url   = "",
  .support_url  = "",
  .version      = "0.0.1",
  .description  = "",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_INSTRUMENT, nullptr }
};

//----------------------------------------------------------------------
//
// processor
//
//----------------------------------------------------------------------

class sa_synth_voice_processor
: public SAT_VoiceProcessor<sa_synth_voice,SA_SYNTH_MAX_VOICES> {

//------------------------------
public:
//------------------------------

  sa_synth_voice_processor(SAT_ProcessorListener* AListener, uint32_t AOversample, uint32_t ABufferSize)
  : SAT_VoiceProcessor(AListener) {
    // test / experimental
    // move to voice_processor ?
    SAT_VoiceContext* context = getVoiceContext();
    context->oversample = AOversample;
    context->buffersize = ABufferSize;
    SAT_PRINT("oversample %i\n",AOversample);
    SAT_PRINT("buffersize %i\n",ABufferSize);
  }

  //----------

  virtual ~sa_synth_voice_processor() {
    SAT_TRACE;
  }

//------------------------------
public:
//------------------------------

  // void activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) {
  //   //setOversample(2);
  //   SAT_VoiceProcessor::activate(sample_rate,min_frames_count,max_frames_count);
  // }

  //----------

  void process(SAT_ProcessContext* AContext) override {
    SAT_VoiceProcessor::process(AContext);
    const clap_process_t* process = AContext->process;
    float** output = process->audio_outputs[0].data32;
    uint32_t length = process->frames_count;
    sat_param_t gain = AContext->parameters->getItem(SA_SYNTH_PARAM_GLOBAL_GAIN)->getValue();
    gain = (gain * gain * gain);
    SAT_ScaleStereoBuffer(output,gain,length);
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

  sa_synth_voice_processor* MProcessor    = nullptr;
  SAT_VoicesWidget*         MVoicesWidget = nullptr;

  uint64_t    MTrackFlags                 = 0;
  char        MTrackName[CLAP_NAME_SIZE]  = {0};
  SAT_Color   MTrackColor                 = SAT_Black;
  int32_t     MTrackChannelCount          = 0;
  const char* MTrackPortType              = nullptr;
  bool        MTrackIsReturnTrack         = false;
  bool        MTrackIsBus                 = false;
  bool        MTrackIsMaster              = false;

//------------------------------
public:
//------------------------------

  SAT_PLUGIN_DEFAULT_CONSTRUCTOR(sa_synth_plugin);

//------------------------------
public:
//------------------------------

  bool init() final {
    SAT_TRACE;
    registerDefaultSynthExtensions();
    registerExtension(CLAP_EXT_PARAM_INDICATION);
    registerExtension(CLAP_EXT_REMOTE_CONTROLS);
    registerExtension(CLAP_EXT_PRESET_LOAD);
    registerExtension(CLAP_EXT_TRACK_INFO);
    appendClapNoteInputPort("In");
    appendStereoAudioOutputPort("Out");
    MProcessor = new sa_synth_voice_processor(this,2,4096);
    setProcessor(MProcessor);
    MProcessor->init(getClapPlugin(),getClapHost());
    MProcessor->setProcessThreaded(true);
    MProcessor->setEventMode(SAT_VOICE_EVENT_MODE_INTERLEAVED);
    sa_synth_setup_parameters(this);
    #ifndef SAT_NO_GUI
      setInitialEditorSize(SA_SYNTH_EDITOR_WIDTH,SA_SYNTH_EDITOR_HEIGHT,SA_SYNTH_EDITOR_SCALE,true);
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
    info->voice_count     = SA_SYNTH_MAX_VOICES;
    info->voice_capacity  = SA_SYNTH_MAX_VOICES;
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
      case 0: { // OSC
        strcpy(page->section_name,"Section");
        page->page_id = 0;
        strcpy(page->page_name,"OSC");
        //for (uint32_t i=0; i<8; i++) page->param_ids[i] = i;
        page->param_ids[0] = SA_SYNTH_PARAM_OSC1_SQU;
        page->param_ids[1] = SA_SYNTH_PARAM_OSC1_TRI;
        page->param_ids[2] = SA_SYNTH_PARAM_OSC1_SIN;
        page->param_ids[3] = SA_SYNTH_PARAM_OSC1_WIDTH;
        page->param_ids[4] = SA_SYNTH_PARAM_OSC2_SQU;
        page->param_ids[5] = SA_SYNTH_PARAM_OSC2_TRI;
        page->param_ids[6] = SA_SYNTH_PARAM_OSC2_SIN;
        page->param_ids[7] = SA_SYNTH_PARAM_OSC2_WIDTH;
        page->is_for_preset = false;
        return true;
      }
    }
    return false;
  }  
  
//------------------------------
public: // preset load
//------------------------------

  bool preset_load_from_location(uint32_t location_kind, const char *location, const char *load_key) final {
    if (location_kind == CLAP_PRESET_DISCOVERY_LOCATION_FILE) {
      bool loaded = loadPresetFromFile(location,load_key);
      const clap_host_t* claphost = getClapHost();
      SAT_Assert(claphost);
      SAT_Host* host = getHost();
      SAT_Assert(host);
      if (host->ext.preset_load) {
        if (loaded) host->ext.preset_load->loaded(claphost,location_kind,location,load_key);
        else host->ext.preset_load->on_error(claphost,location_kind,location,load_key,0,"error loading preset");
      }
      return loaded;
    }
    return false;
  }

//------------------------------
public: // track info
//------------------------------

  void track_info_changed() override {
    SAT_Host* host = getHost();
    const clap_host_t* claphost = getClapHost();
    clap_track_info_t info;
    if (host->ext.track_info) {
      if (host->ext.track_info->get(claphost,&info)) {
        MTrackFlags = info.flags;
        if (info.flags & CLAP_TRACK_INFO_HAS_TRACK_NAME) {
          SAT_Strlcpy(MTrackName,info.name,CLAP_NAME_SIZE);
          SAT_PRINT("name: %s\n",MTrackName);
        }
        if (info.flags & CLAP_TRACK_INFO_HAS_TRACK_COLOR) {
          MTrackColor = SAT_Color(info.color.red,info.color.green,info.color.blue,info.color.alpha);
          SAT_PRINT("color: %i,%i,%i,%i \n",info.color.red,info.color.green,info.color.blue,info.color.alpha);
        }
        if (info.flags & CLAP_TRACK_INFO_HAS_AUDIO_CHANNEL) {
          MTrackChannelCount = info.audio_channel_count;
          MTrackPortType = info.audio_port_type;
          SAT_PRINT("count %i type %s\n",MTrackChannelCount,MTrackPortType);
        }
        MTrackIsReturnTrack = ((info.flags & CLAP_TRACK_INFO_IS_FOR_RETURN_TRACK)  != 0);
        MTrackIsBus         = ((info.flags & CLAP_TRACK_INFO_IS_FOR_BUS)           != 0);
        MTrackIsMaster      = ((info.flags & CLAP_TRACK_INFO_IS_FOR_MASTER)        != 0);
        SAT_PRINT("return %i bus %i master %i\n",MTrackIsReturnTrack,MTrackIsBus,MTrackIsMaster);
        //SAT_Widget* widget = (SAT_Widget*)param->getWidget();
        //if (widget && MEditor && MEditor->isOpen()) widget->do_Widget_redraw(widget,0,SAT_WIDGET_REDRAW_SELF);
      }
    }
  }

//------------------------------
public: // gui
//------------------------------

  #ifndef SAT_NO_GUI
    #include "sa_synth/sa_synth_gui.h"
  #endif

//------------------------------
public: // timer
//------------------------------

  #ifdef SAT_EDITOR_EMBEDDED
  
    void on_EditorListener_timer(double ADelta) override {
      SAT_Plugin::on_EditorListener_timer(ADelta);
      bool changed = false;
      for (uint32_t i=0; i<SA_SYNTH_MAX_VOICES; i++) {
        uint32_t state = MProcessor->getVoiceState(i);
        if (state != MVoicesWidget->getVoiceState(i)) {
          //SAT_TRACE;
          changed = true;
          MVoicesWidget->setVoiceState(i,state);
        }
      }
      MVoicesWidget->setNumPlayingVoices(MProcessor->getNumPlayingVoices());
      MVoicesWidget->setNumReleasedVoices(MProcessor->getNumReleasedVoices());
      if (changed) {
        //SAT_TRACE;
        MVoicesWidget->do_Widget_redraw(MVoicesWidget);
      }
    }

  #endif

};

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY
  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(sa_synth_descriptor,sa_synth_plugin)
#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#undef SA_SYNTH_MAX_VOICES
#undef SA_SYNTH_EDITOR_WIDTH
#undef SA_SYNTH_EDITOR_HEIGHT
#undef SA_SYNTH_EDITOR_SCALE

//----------------------------------------------------------------------
#endif