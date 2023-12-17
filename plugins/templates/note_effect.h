
#include "sat.h"
#include "audio/sat_audio_utils.h"
#include "plugin/sat_entry.h"
#include "plugin/sat_parameters.h"
#include "plugin/sat_plugin.h"
#include "gui/sat_widgets.h"

//----------------------------------------------------------------------

#define PLUGIN_NAME "note_effect"
#define PLUGIN_AUTHOR "me"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t my_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = PLUGIN_AUTHOR "/" PLUGIN_NAME,
  .name         = PLUGIN_NAME,
  .vendor       = PLUGIN_AUTHOR,
  .url          = "",
  .manual_url   = "",
  .support_url  = "",
  .version      = "0.0.1",
  .description  = "",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_NOTE_EFFECT, nullptr }
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class note_effect
: public SAT_Plugin {
  
//------------------------------
private:
//------------------------------

  // int32_t par_value1            = 0;
  // int32_t par_value2            = 1;

  // int32_t mod_value1            = 0;
  // int32_t mod_value2            = 0;
  // int32_t mod_MModulo           = 0;

  // int32_t MNoteOffsets[16*128]  = {0};
  // int32_t MValue1               = 0;
  // int32_t MValue2               = 1;
  // int32_t MModulo               = 12;
  // bool    MWasPlaying           = false;

//------------------------------
public:
//------------------------------

  SAT_DEFAULT_PLUGIN_CONSTRUCTOR(note_effect)
 
  //----------

  bool init() final {
    registerDefaultExtensions();    
    appendClapNoteInputPort("In");
    appendClapNoteOutputPort("Out");
    // appendParameter( new SAT_IntParameter( "Value1", 0, -12, 12 ));
    // appendParameter( new SAT_IntParameter( "Value2", 1, -12, 12 ));
    // appendParameter( new SAT_IntParameter( "Modulo", 12,  1, 24 ));
    setAllParameterFlags(CLAP_PARAM_IS_MODULATABLE);
    return SAT_Plugin::init();
  }
  
  //----------

  bool on_plugin_noteOn(const clap_event_note_t* event) final {
    // SAT_ProcessContext* context = getProcessContext();
    // const clap_output_events_t *out_events = context->process->out_events;
    // int32_t channel = event->channel;
    // int32_t key = event->key;
    // int32_t value = get_fibo();
    // MNoteOffsets[(channel * 128) + key] = value;
    // clap_event_note_t new_event;// = {0};
    // memcpy(&new_event,event,sizeof(clap_event_note_t));
    // new_event.key += value;
    // out_events->try_push(out_events,(const clap_event_header_t*)&new_event);
    // next_fibo();
    return true;
  }

  //----------

  bool on_plugin_noteOff(const clap_event_note_t* event) final {
    // SAT_ProcessContext* context = getProcessContext();
    // const clap_output_events_t *out_events = context->process->out_events;
    // int32_t channel = event->channel;
    // int32_t key = event->key;
    // SAT_Assert( (channel >= 0) && (channel <= 15) );
    // SAT_Assert( (key >= 0) && (key <= 127) );
    // int32_t value = MNoteOffsets[(channel * 128) + key];
    // clap_event_note_t new_event;// = {0};
    // memcpy(&new_event,event,sizeof(clap_event_note_t));
    // new_event.key += value;
    // out_events->try_push(out_events,(const clap_event_header_t*)&new_event);
    return true;
  }

  //----------

  bool on_plugin_noteChoke(const clap_event_note_t* event) final {
    // SAT_ProcessContext* context = getProcessContext();
    // const clap_output_events_t *out_events = context->process->out_events;
    // int32_t channel = event->channel;
    // int32_t key = event->key;
    // SAT_Assert( (channel >= 0) && (channel <= 15) );
    // SAT_Assert( (key >= 0) && (key <= 127) );
    // int32_t value = MNoteOffsets[(channel*128)+key];
    // clap_event_note_t new_event;// = {0};
    // memcpy(&new_event,event,sizeof(clap_event_note_t));
    // new_event.key += value;
    // out_events->try_push(out_events,(const clap_event_header_t*)&new_event);
    return true;
  }

  //----------

  bool on_plugin_noteExpression(const clap_event_note_expression_t* event) final {
    // SAT_ProcessContext* context = getProcessContext();
    // const clap_output_events_t *out_events = context->process->out_events;
    // int32_t channel = event->channel;
    // int32_t key = event->key;
    // SAT_Assert( (channel >= 0) && (channel <= 15) );
    // SAT_Assert( (key >= 0) && (key <= 127) );
    // int32_t value = MNoteOffsets[(channel*128)+key];
    // clap_event_note_expression_t new_event;// = {0};
    // memcpy(&new_event,event,sizeof(clap_event_note_expression_t));
    // new_event.key += value;
    // out_events->try_push(out_events,(const clap_event_header_t*)&new_event);
    return true;
  }

  //----------

  bool on_plugin_paramValue(const clap_event_param_value_t* event) final {
    // uint32_t index = event->param_id;
    // double value = event->value;
    // switch (index) {
    //   case 0:
    //     par_value1 = value;
    //     restart_fibo();
    //     break;
    //   case 1:
    //     par_value2 = value;
    //     restart_fibo();
    //     break;
    //   case 2:
    //     MModulo = value;
    //     restart_fibo();
    //     break;
    // }
    return true;
  }

  //----------

  bool on_plugin_paramMod(const clap_event_param_mod_t* event) final {
    // uint32_t index = event->param_id;
    // double value = event->amount;
    // switch (index) {
    //   case 0:
    //     mod_value1 = value;
    //     //restart_fibo();
    //     break;
    //   case 1:
    //     mod_value2 = value;
    //     //restart_fibo();
    //     break;
    //   case 2:
    //     mod_MModulo = value;
    //     //restart_fibo();
    //     break;
    // }
    return true;
  }

  //----------

  bool on_plugin_transport(const clap_event_transport_t* event) final {
    // bool is_playing = (event->flags & CLAP_TRANSPORT_IS_PLAYING);
    // if (is_playing && !MWasPlaying) {
    //   SAT_Print("start\n");
    //   restart_fibo();
    // }
    // MWasPlaying = is_playing;
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
  SAT_PLUGIN_ENTRY(my_descriptor,note_effect);
#endif

//----------

#undef PLUGIN_NAME
#undef PLUGIN_AUTHOR
//#undef EDITOR_WIDTH
//#undef EDITOR_HEIGHT

