#ifndef sa_cred_included
#define sa_cred_included
//----------------------------------------------------------------------

#include "sat.h"
#include "audio/sat_audio_utils.h"
#include "plugin/sat_entry.h"
#include "plugin/sat_parameters.h"
#include "plugin/sat_plugin.h"

#ifndef SAT_NO_GUI
#include "gui/sat_widgets.h"
#endif

#include "plugin/processor/sat_interleaved_processor.h"

//----------------------------------------------------------------------

//#ifdef SAT_DEBUG
//  #define PLUGIN_NAME   "sa_cred (debug)"
//#else
  #define PLUGIN_NAME   "sa_cred"
//#endif

//#define EDITOR_WIDTH    400
//#define EDITOR_HEIGHT   (40 + 10 + (13 * (20 + 10)) + 20)

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t sa_cred_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = SAT_VENDOR "/" PLUGIN_NAME,
  .name         = PLUGIN_NAME,
  .vendor       = SAT_VENDOR,
  .url          = SAT_URL,
  .manual_url   = "",
  .support_url  = "",
  .version      = SAT_VERSION,
  .description  = "fibonacci, pisano, sacred geometry..",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_NOTE_EFFECT, nullptr }
};

//----------------------------------------------------------------------
//
// processor
//
//----------------------------------------------------------------------

class sa_cred_processor
: public SAT_InterleavedProcessor {

//------------------------------
private:
//------------------------------

  int32_t par_value1            = 0;
  int32_t par_value2            = 1;
  int32_t par_modulo            = 12;

  int32_t mod_value1            = 0;
  int32_t mod_value2            = 0;
  int32_t mod_MModulo           = 0;

  int32_t MNoteOffsets[16*128]  = {0};
  int32_t MValue1               = 0;
  int32_t MValue2               = 1;
  int32_t MModulo               = 12;
  bool    MWasPlaying           = false;

//------------------------------
public:
//------------------------------

  sa_cred_processor(SAT_ProcessorListener* AListener)
  : SAT_InterleavedProcessor(AListener) {
  }

  //----------

  virtual ~sa_cred_processor() {
  }

//------------------------------
public: // fibonacci
//------------------------------

  int32_t get_fibo() {
    return MValue1;
  }

  //----------

  void next_fibo() {
    int32_t v1 = MValue1 + mod_value1;
    int32_t v2 = MValue2 + mod_value2;
    int32_t m =  MModulo + mod_MModulo;
    int32_t v = (v1 + v2) % m;
    MValue2 = MValue1;//v1;
    MValue1 = v;
  }

  //----------

  void restart_fibo() {
    MValue1 = par_value1;
    MValue2 = par_value2;
    MModulo = par_modulo;
  }

//------------------------------
public:
//------------------------------

  void noteOnEvent(const clap_event_note_t* event) final {
    SAT_ProcessContext* context = getProcessContext();
    SAT_Assert(context);
    const clap_output_events_t *out_events = context->process->out_events;
    int32_t channel = event->channel;
    int32_t key = event->key;
    int32_t value = get_fibo();
    MNoteOffsets[(channel * 128) + key] = value;
    clap_event_note_t new_event;// = {0};
    memcpy(&new_event,event,sizeof(clap_event_note_t));
    new_event.key += value;
    out_events->try_push(out_events,(const clap_event_header_t*)&new_event);
    next_fibo();
  }

  //----------

  void noteOffEvent(const clap_event_note_t* event) final {
    SAT_ProcessContext* context = getProcessContext();
    const clap_output_events_t *out_events = context->process->out_events;
    int32_t channel = event->channel;
    int32_t key = event->key;
    SAT_Assert( (channel >= 0) && (channel <= 15) );
    SAT_Assert( (key >= 0) && (key <= 127) );
    int32_t value = MNoteOffsets[(channel * 128) + key];
    clap_event_note_t new_event;// = {0};
    memcpy(&new_event,event,sizeof(clap_event_note_t));
    new_event.key += value;
    out_events->try_push(out_events,(const clap_event_header_t*)&new_event);
  }

  //----------

  void noteChokeEvent(const clap_event_note_t* event) final {
    SAT_ProcessContext* context = getProcessContext();
    const clap_output_events_t *out_events = context->process->out_events;
    int32_t channel = event->channel;
    int32_t key = event->key;
    SAT_Assert( (channel >= 0) && (channel <= 15) );
    SAT_Assert( (key >= 0) && (key <= 127) );
    int32_t value = MNoteOffsets[(channel*128)+key];
    clap_event_note_t new_event;// = {0};
    memcpy(&new_event,event,sizeof(clap_event_note_t));
    new_event.key += value;
    out_events->try_push(out_events,(const clap_event_header_t*)&new_event);
  }

  //----------

  void noteExpressionEvent(const clap_event_note_expression_t* event) final {
    SAT_ProcessContext* context = getProcessContext();
    const clap_output_events_t *out_events = context->process->out_events;
    int32_t channel = event->channel;
    int32_t key = event->key;
    SAT_Assert( (channel >= 0) && (channel <= 15) );
    SAT_Assert( (key >= 0) && (key <= 127) );
    int32_t value = MNoteOffsets[(channel*128)+key];
    clap_event_note_expression_t new_event;// = {0};
    memcpy(&new_event,event,sizeof(clap_event_note_expression_t));
    new_event.key += value;
    out_events->try_push(out_events,(const clap_event_header_t*)&new_event);
  }

  //----------

  void paramValueEvent(const clap_event_param_value_t* event) final {
    uint32_t index = event->param_id;
    double value = event->value;
    switch (index) {
      case 0:
        par_value1 = value;
        restart_fibo();
        break;
      case 1:
        par_value2 = value;
        restart_fibo();
        break;
      case 2:
        //MModulo = value;
        par_modulo = value;
        restart_fibo();
        break;
    }
  }

  //----------

  void paramModEvent(const clap_event_param_mod_t* event) final {
    uint32_t index = event->param_id;
    double value = event->amount;
    switch (index) {
      case 0:
        mod_value1 = value;
        //restart_fibo();
        break;
      case 1:
        mod_value2 = value;
        //restart_fibo();
        break;
      case 2:
        mod_MModulo = value;
        //restart_fibo();
        break;
    }
  }

  //----------

  void transportEvent(const clap_event_transport_t* event) final {
    //SAT_TRACE;
    bool is_playing = (event->flags & CLAP_TRANSPORT_IS_PLAYING);
    if (is_playing && !MWasPlaying) {
      SAT_PRINT("start\n");
      restart_fibo();
    }
    MWasPlaying = is_playing;
    //SAT_TRACE;
  }

};


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_cred_plugin
: public SAT_Plugin {
 

//------------------------------
public:
//------------------------------

  //SAT_PLUGIN_DEFAULT_CONSTRUCTOR(sa_cred_plugin)

  sa_cred_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : SAT_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~sa_cred_plugin() {
  }
  
//------------------------------
public:
//------------------------------

  bool init() final {

    registerDefaultExtensions();    
    registerExtension(CLAP_EXT_NOTE_PORTS);
    appendClapNoteInputPort("In");
    appendClapNoteOutputPort("Out");

    appendParameter( new SAT_IntParameter( "Value1", "", 0, -12, 12, CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED ));
    appendParameter( new SAT_IntParameter( "Value2", "", 1, -12, 12, CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED ));
    appendParameter( new SAT_IntParameter( "Modulo", "", 12,  1, 24, CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED ));
    //setAllParameterFlags(CLAP_PARAM_IS_MODULATABLE);

    sa_cred_processor* processor = new sa_cred_processor(this);
    setProcessor(processor);

    // processor->init(getClapPlugin(),getClapHost());
    // processor->setProcessThreaded(true);
    // processor->setEventMode(SAT_VOICE_EVENT_MODE_QUANTIZED);
    
    return SAT_Plugin::init();
  }
  
  //----------

  // bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
  //   MProcessor->setSampleRate(sample_rate);
  //   return SAT_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  // }
  
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY
  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(sa_cred_descriptor,sa_cred_plugin);
#endif

//----------

#undef PLUGIN_NAME
//#undef EDITOR_WIDTH
//#undef EDITOR_HEIGHT

//----------------------------------------------------------------------
#endif
