#ifndef sa_cred_included
#define sa_cred_included
//----------------------------------------------------------------------

#include "sat.h"
#include "audio/sat_audio_utils.h"
#include "plugin/sat_entry.h"
#include "plugin/sat_parameters.h"
#include "plugin/sat_plugin.h"
#include "gui/sat_widgets.h"

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
//
//
//----------------------------------------------------------------------

class sa_cred_plugin
: public SAT_Plugin {
  
//------------------------------
private:
//------------------------------

  int32_t par_value1            = 0;
  int32_t par_value2            = 1;

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

  SAT_DEFAULT_PLUGIN_CONSTRUCTOR(sa_cred_plugin)
  
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
  }

//------------------------------
public:
//------------------------------

  bool init() final {

    registerDefaultExtensions();    

    registerExtension(CLAP_EXT_NOTE_PORTS);
    appendClapNoteInputPort("In");
    appendClapNoteOutputPort("Out");

    appendParameter( new SAT_IntParameter( "Value1", 0, -12, 12 ));
    appendParameter( new SAT_IntParameter( "Value2", 1, -12, 12 ));
    appendParameter( new SAT_IntParameter( "Modulo", 12,  1, 24 ));
    setAllParameterFlags(CLAP_PARAM_IS_MODULATABLE);
    
    return SAT_Plugin::init();
  }
  
  //----------

  bool on_plugin_noteOn(const clap_event_note_t* event) final {
    //SAT_PRINT;
    SAT_ProcessContext* context = getProcessContext();
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
    return true;
  }

  //----------

  bool on_plugin_noteOff(const clap_event_note_t* event) final {
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
    return true;
  }

  //----------

  bool on_plugin_noteChoke(const clap_event_note_t* event) final {
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
    return true;
  }

  //----------

  bool on_plugin_noteExpression(const clap_event_note_expression_t* event) final {
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
    return true;
  }

  //----------

  bool on_plugin_paramValue(const clap_event_param_value_t* event) final {
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
        MModulo = value;
        restart_fibo();
        break;
    }
    return true;
  }

  //----------

  bool on_plugin_paramMod(const clap_event_param_mod_t* event) final {
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
    return true;
  }

  //----------

  bool on_plugin_transport(const clap_event_transport_t* event) final {
    bool is_playing = (event->flags & CLAP_TRANSPORT_IS_PLAYING);
    if (is_playing && !MWasPlaying) {
      SAT_Print("start\n");
      restart_fibo();
    }
    MWasPlaying = is_playing;
    return true; // false;
  }
  
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
