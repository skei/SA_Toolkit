#ifndef mip_audio_jack_included
#define mip_audio_jack_included
//----------------------------------------------------------------------

#include <jack/jack.h>
#include <jack/midiport.h>

//----------------------------------------------------------------------

class MIP_AudioIOListener {
  //virtual int on_audio_process(uint32_t length) { return 0; }
};

//----------------------------------------------------------------------

class MIP_AudioJack {

//------------------------------
private:
//------------------------------

  MIP_AudioIOListener*  MListener         = nullptr;

  jack_client_t*        MJackClient       = nullptr;
  jack_port_t*          MAudioInputPort1  = nullptr;
  jack_port_t*          MAudioInputPort2  = nullptr;
  jack_port_t*          MAudioOutputPort1 = nullptr;
  jack_port_t*          MAudioOutputPort2 = nullptr;
  jack_port_t*          MMidiInputPort    = nullptr;
  jack_port_t*          MMidiOutputPort   = nullptr;

  jack_nframes_t        MSampleRate       = 0;
  jack_nframes_t        MBufferSize       = 0;

//------------------------------
public:
//------------------------------

static
int jack_process_callback(jack_nframes_t nframes, void* arg) {
  MIP_AudioJack* jack = (MIP_AudioJack*)arg;
  return jack->process(nframes);
}

//------------------------------
public:
//------------------------------

  int process(jack_nframes_t nframes) {

    // midi

    void* midi_in_port  = jack_port_get_buffer(MMidiInputPort,nframes);
    void* midi_out_port = jack_port_get_buffer(MMidiOutputPort,nframes);

    jack_midi_clear_buffer(midi_out_port);
    jack_nframes_t num_events = jack_midi_get_event_count(midi_in_port);
    if (num_events > 0) 	{
      jack_midi_event_t event;
      for (uint32_t i=0; i<num_events; i++) {
        jack_midi_event_get(&event,midi_in_port,i);
        uint8_t msg1 = event.buffer[0];
        uint8_t msg2 = event.buffer[1];
        uint8_t msg3 = event.buffer[2];

        //TODO: insert into MIP_MidiTrack
        MIP_Print("midi: %02x %02x %02x\n",msg1,msg2,msg3);

        uint32_t offset = i;
        jack_midi_data_t* buffer = jack_midi_event_reserve(midi_out_port,offset,3);
        buffer[0] = msg1;
        buffer[1] = msg2;
        buffer[2] = msg3;
      }
    }

    // audio

    jack_default_audio_sample_t *input0  = (jack_default_audio_sample_t*)jack_port_get_buffer(MAudioInputPort1,nframes);
    jack_default_audio_sample_t *input1  = (jack_default_audio_sample_t*)jack_port_get_buffer(MAudioInputPort2,nframes);
    jack_default_audio_sample_t *output0 = (jack_default_audio_sample_t*)jack_port_get_buffer(MAudioOutputPort1,nframes);
    jack_default_audio_sample_t *output1 = (jack_default_audio_sample_t*)jack_port_get_buffer(MAudioOutputPort2,nframes);
    for (uint32_t i=0; i<nframes; i++) {
      *output0++ = *input0++;
      *output1++ = *input1++;
    }

    return 0;
  }

//------------------------------
public:
//------------------------------

    bool init(MIP_AudioIOListener* AListener) {
      MListener   = AListener;
      jack_status_t status;// = (jack_status_t)0;
      MJackClient = jack_client_open("MIP",JackNullOption,&status);
      jack_set_process_callback(MJackClient,jack_process_callback,this);
      MAudioInputPort1  = jack_port_register(MJackClient,"audio_in1", JACK_DEFAULT_AUDIO_TYPE,JackPortIsInput, 0);
      MAudioInputPort2  = jack_port_register(MJackClient,"audio_in2", JACK_DEFAULT_AUDIO_TYPE,JackPortIsInput, 0);
      MAudioOutputPort1 = jack_port_register(MJackClient,"audio_out1",JACK_DEFAULT_AUDIO_TYPE,JackPortIsOutput,0);
      MAudioOutputPort2 = jack_port_register(MJackClient,"audio_out2",JACK_DEFAULT_AUDIO_TYPE,JackPortIsOutput,0);
      MMidiInputPort    = jack_port_register(MJackClient,"midi_in",   JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0);
      MMidiOutputPort   = jack_port_register(MJackClient,"midi_out",  JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput,0);
      MSampleRate = jack_get_sample_rate(MJackClient);
      MBufferSize = jack_get_buffer_size(MJackClient);
      return true;
    }

    //----------

    void exit() {
      jack_client_close(MJackClient);
    }

    //----------

    bool start() {
      jack_activate(MJackClient);
      return true;
    }

    //----------

    void stop() {
      jack_deactivate(MJackClient);
    }

};

//----------------------------------------------------------------------
#endif
