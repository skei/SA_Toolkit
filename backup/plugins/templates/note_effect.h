
#include "plugin/sat_plugin.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

const clap_plugin_descriptor_t note_effect_descriptor = {
  .clap_version = CLAP_VERSION,
  .id           = "sat_note_effect",
  .name         = "note_effect",
  .vendor       = "skei.audio",
  .url          = "https://github.com/skei/SA_Toolkit",
  .manual_url   = "",
  .support_url  = "",
  .version      = "0.0.1",
  .description  = "",
  .features     = (const char*[]){ CLAP_PLUGIN_FEATURE_NOTE_EFFECT, nullptr }
};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class note_effect
: public SAT_Plugin {
  
//------------------------------
public:
//------------------------------

  SAT_DEFAULT_PLUGIN_CONSTRUCTOR(note_effect)
 
  //----------

  bool send_event(void* event) {
    SAT_ProcessContext* context = getProcessContext();
    const clap_output_events_t *out_events = context->process->out_events;
    out_events->try_push(out_events,(const clap_event_header_t*)event);
    return true;
  }

  //----------

  bool init() final {
    registerDefaultExtensions();    
    appendClapNoteInputPort("In");
    appendClapNoteOutputPort("Out");
    setAllParameterFlags(CLAP_PARAM_IS_MODULATABLE);
    return SAT_Plugin::init();
  }
  
  //----------

  //clap_event_note_t note_event;
  //note_event.header.size     = sizeof(clap_event_note_t);
  //note_event.header.time     = event.header.time;
  //note_event.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
  //note_event.header.type     = CLAP_EVENT_NOTE_ON;
  //note_event.header.flags    = 0; // CLAP_EVENT_IS_LIVE, CLAP_EVENT_DONT_RECORD
  //note_event.note_id         = (chan * 128) + index;
  //note_event.port_index      = event->port_index;
  //note_event.channel         = chan;
  //note_event.key             = index;
  //note_event.velocity        = value * (double)SAT_INV127;
  //const clap_event_header_t* header = (const clap_event_header_t*)&note_event;

  //----------

  bool on_plugin_noteOn(const clap_event_note_t* event) final {
    clap_event_note_t new_event;
    memcpy(&new_event,event,sizeof(clap_event_note_t));
    new_event.velocity *= 0.5;
    send_event(&new_event);
    return true;
  }

  //----------

  bool on_plugin_noteOff(const clap_event_note_t* event) final {
    send_event((void*)event);
    return true;
  }

  //----------

  bool on_plugin_noteChoke(const clap_event_note_t* event) final {
    send_event((void*)event);
    return true;
  }

  //----------

  bool on_plugin_noteExpression(const clap_event_note_expression_t* event) final {
    send_event((void*)event);
    return true;
  }

  //----------

  // bool on_plugin_midi(const clap_event_midi_t* event) final {
  //   return true;
  // }

  //----------

  // bool on_plugin_midiSysex(const clap_event_midi_sysex_t* event) {
  //   return true;
  // }

  //----------

  // bool on_plugin_midi2(const clap_event_midi2_t* event) {
  //   return true;
  // }

  //----------

  // bool on_plugin_paramValue(const clap_event_param_value_t* event) final {
  //   return true;
  // }

  //----------

  // bool on_plugin_paramMod(const clap_event_param_mod_t* event) final {
  //   return true;
  // }

  //----------

  // bool on_plugin_transport(const clap_event_transport_t* event) final {
  //   return true;
  // }
  
};

//----------------------------------------------------------------------
//
// entry
//
//----------------------------------------------------------------------

#ifndef SAT_NO_ENTRY
  #include "plugin/sat_entry.h"
  SAT_PLUGIN_ENTRY(note_effect_descriptor,note_effect);
#endif
