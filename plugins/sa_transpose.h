#if 0

#ifndef sa_transpose_included
#define sa_transpose_included
//----------------------------------------------------------------------




//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#include "plugin/mip_plugin.h"

//----------------------------------------------------------------------

const char* sa_transpose_features[] = {
  "audio_effect",
  nullptr
};

//----------

const clap_plugin_descriptor_t sa_transpose_descriptor = {
  CLAP_VERSION,
  "skei.audio/sa_transpose",
  "sa_transpose",
  "skei.audio",
  "https://torhelgeskei.com",
  "",
  "",
  "0.0.1",
  "sa_transpose",
  sa_transpose_features
};

//----------

//#define

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_transpose_plugin
: public MIP_Plugin {

//------------------------------
private:
//------------------------------

  bool    MNeedPrecalc  = false;
  bool    MNeedRecalc   = false;
  float   MSampleRate   = 0.0;

  int32_t MNoteBuffer[128*16]; // index = incoming note, value = outgoing note
  //int32_t MChanBuffer[128*16];

//------------------------------
public:
//------------------------------

  sa_transpose_plugin(const clap_plugin_descriptor_t* ADescriptor, const clap_host_t* AHost)
  : MIP_Plugin(ADescriptor,AHost) {
  }

  //----------

  virtual ~sa_transpose_plugin() {
  }

//------------------------------
private:
//------------------------------

  void precalc(float srate) {
    MNeedPrecalc = false;
  }

  //----------

  void recalc(float srate) {
    MNeedRecalc = false;
  }

  //----------

//------------------------------
private:
//------------------------------

  //void handle_parameter_event(const clap_event_param_value_t* param_value) final {
  //  MIP_Plugin::handle_parameter_event(param_value);
  //  switch (param_value->param_id) {
  //    case 0: MTranspose = param_value->value; break;
  //  }
  //}

  //----------

  void handle_events_input(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
    uint32_t num_events = in_events->size(in_events);
    for (uint32_t i=0; i<num_events; i++) {
      const clap_event_header_t* header = in_events->get(in_events,i);
      if (header->space_id == CLAP_CORE_EVENT_SPACE_ID) {

        int32_t                   port        = -1;
        int32_t                   key         = -1;
        int32_t                   channel     = -1;
        double                    velocity    = 0.0;
        uint32_t                  index       = 0;
        const clap_event_note_t*  note_event  = nullptr;

        switch (header->type) {

          case CLAP_EVENT_NOTE_ON:
            //handle_note_on_event((clap_event_note_t*)header);
            note_event  = (const clap_event_note_t*)header;
            port        = note_event->port_index;
            key         = note_event->key;
            channel     = note_event->channel;
            velocity    = note_event->velocity;
            index       = (channel*128) + key;
            key += MParameterValues[0];
            MNoteBuffer[index] = key;
            send_note_event(CLAP_EVENT_NOTE_ON,port,key,channel,velocity,out_events);
            break;

          case CLAP_EVENT_NOTE_OFF:
            //handle_note_off_event((clap_event_note_t*)header);
            note_event  = (const clap_event_note_t*)header;
            port        = note_event->port_index;
            key         = note_event->key;
            channel     = note_event->channel;
            velocity    = note_event->velocity;
            index       = (channel*128) + key;
            key         = MNoteBuffer[index];
            send_note_event(CLAP_EVENT_NOTE_OFF,port,key,channel,velocity,out_events);
            break;

          //case CLAP_EVENT_NOTE_END:
          //  handle_note_end_event((clap_event_note_t*)header);
          //  break;

          //case CLAP_EVENT_NOTE_CHOKE:
          //  handle_note_choke_event((clap_event_note_t*)header);
          //  break;

          //case CLAP_EVENT_NOTE_EXPRESSION:
          //  handle_note_expression_event((clap_event_note_expression_t*)header);
          //  break;

          case CLAP_EVENT_PARAM_VALUE:
            handle_parameter_event((clap_event_param_value_t*)header);
            break;

          //case CLAP_EVENT_PARAM_MOD:
          //  handle_modulation_event((clap_event_param_mod_t*)header);
          //  break;

          //case CLAP_EVENT_MIDI:
          //  handle_midi_event((clap_event_midi_t*)header);
          //  break;

          //case CLAP_EVENT_MIDI2:
          //  handle_midi2_event((clap_event_midi2_t*)header);
          //  break;

          //case CLAP_EVENT_MIDI_SYSEX:
          //  handle_midi_sysex_event((clap_event_midi_sysex_t*)header);
          //  break;

          //case CLAP_EVENT_TRANSPORT:
          //  handle_transport_event((clap_event_transport_t*)header);
          //  break;

          default:
            send_event(header,out_events);
            break;

        }
      } // space id
      else {
        send_event(header,out_events);
      }
    }
  }

//------------------------------
public: // plugin
//------------------------------

  bool init() final {
    appendParameter(new MIP_IntParameter( 0, CLAP_PARAM_IS_AUTOMATABLE, "Transpose", "", -24, 24,  0 ));
    bool result = MIP_Plugin::init();
    if (result) {
      setDefaultParameterValues();
      //MNeedRecalc = true;
      return true;
    }
    return false;
  }

  //----------

  bool activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) final {
    MSampleRate = sample_rate;
    return MIP_Plugin::activate(sample_rate,min_frames_count,max_frames_count);
  }

};

//----------------------------------------------------------------------
#endif


#endif // 0