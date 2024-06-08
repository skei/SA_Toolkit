#ifndef mip_voice_manager_included
#define mip_voice_manager_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "audio/mip_audio_utils.h"
//#include "base/types/mip_queue.h"
//#include "plugin/clap/mip_clap.h"
#include "plugin/mip_note.h"
#include "audio/old/mip_voice.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

// cpu is more or less the same, but calculating to separate buffers,
// then summing them creates a lot more spikes..

#define MIP_VOICE_ADD_TO_BUFFER

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

template <class VOICE, int VOICE_COUNT>
class MIP_VoiceManager {

  // note_end queue
  typedef MIP_Queue<MIP_Note,VOICE_COUNT * 2> MIP_NoteQueue; // why *2 ?

  #define MIP_VOICE_BUFFERSIZE      (VOICE_COUNT * MIP_VOICE_MANAGER_MAX_BLOCK_SIZE)

//------------------------------
private:
//------------------------------


  /*__MIP_ALIGNED(MIP_ALIGNMENT_CACHE)*/ float MVoiceBuffer[MIP_VOICE_BUFFERSIZE]     = {0};
  /*__MIP_ALIGNED(MIP_ALIGNMENT_CACHE)*/ float MFrameBuffer[MIP_VOICE_MAX_FRAMESIZE]  = {0}; // -> MIP_VOICE_MANAGER_MAX_BLOCK_SIZE ???

  MIP_VoiceContext  MVoiceContext                 = {};
  MIP_NoteQueue     MNoteEndQueue                 = {};
  MIP_Voice<VOICE>  MVoices[VOICE_COUNT]          = {};

  uint32_t          MActiveVoices[VOICE_COUNT]    = {0};
  //uint32_t          MNumPlayingVoices             = 0;
  //uint32_t          MNumReleasedVoices            = 0;

//------------------------------
public:
//------------------------------

  MIP_VoiceManager() {
  }

  //----------

  ~MIP_VoiceManager() {
  }

//------------------------------
public:
//------------------------------

  void setup(float ASampleRate, uint32_t min_frames_count, uint32_t max_frames_count, MIP_ParameterArray* AParameters) {
    MIP_Assert(ASampleRate > 0);
    MVoiceContext.samplerate    = ASampleRate;
    MVoiceContext.invsamplerate = 1.0 / ASampleRate;
    MVoiceContext.voicebuffer   = MVoiceBuffer;
    MVoiceContext.parameters    = AParameters;
    MVoiceContext.process       = nullptr;
    for (uint32_t i=0; i<VOICE_COUNT; i++) {
      MVoices[i].setup(i,&MVoiceContext);
    }
  }

  //----------

  uint32_t getVoiceState(uint32_t AIndex) {
    return MVoices[AIndex].getState();
  }

  //uint32_t getNumPlayingVoices()  { return MNumPlayingVoices; }
  //uint32_t getNumReleasedVoices() { return MNumReleasedVoices; }

  float* getFrameBuffer() {
    return MFrameBuffer;
  }

  float* getVoiceBuffer(uint32_t AIndex) {
    return MVoiceBuffer + (AIndex * MIP_VOICE_MAX_FRAMESIZE);
  }

  //----------

//  void setParameter(uint32_t AIndex, double AValue) {
//    for (uint32_t i=0; i<VOICE_COUNT; i++) {
//      MVoices[i].parameter(AIndex,AValue);
//    }
//  }

  //----------

//  void setModulation(uint32_t AIndex, double AValue) {
//    for (uint32_t i=0; i<VOICE_COUNT; i++) {
//      MVoices[i].modulation(AIndex,AValue);
//    }
//  }

  //

  void updateParameterFromEditor(MIP_Parameter* AParameter) {
    //MVoices.setParameter(AIndex,AValue);
    clap_event_param_value_t event;
    event.header.size     = sizeof(clap_event_param_value_t);
    event.header.time     = 0;
    event.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
    event.header.type     = CLAP_EVENT_PARAM_VALUE;
    event.header.flags    = 0; //CLAP_EVENT_IS_LIVE; // _DONT_RECORD
    event.param_id        = AParameter->getIndex();
    event.cookie          = AParameter;
    event.note_id         = -1;
    event.port_index      = -1;
    event.channel         = -1;
    event.key             = -1;
    event.value           = AParameter->getValue();
    const clap_event_header_t* header = (const clap_event_header_t*)&event;
    handleEvent(header);
  }


//------------------------------
public:
//------------------------------

  void preProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
  }

  //----------

  void postProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
    flushFinishedVoices();
    flushNoteEnds();
  }

  //----------

  //void processEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
  //  // do nothing..
  //}

//------------------------------
public:
//------------------------------

  /*
    frame buffer is split at events, and all voices rendered inbetween
    voices render to frame buffer directly (adding)..
  */

  void processAudioBlock(MIP_ProcessContext* AContext) {

    const clap_process_t* process = AContext->process;
    MVoiceContext.process = process;

    uint32_t length       = process->frames_count;
    uint32_t current_time = 0;
    uint32_t remaining    = process->frames_count;
    uint32_t num_events   = process->in_events->size(process->in_events);

    #ifdef MIP_VOICE_ADD_TO_BUFFER
    MIP_ClearMonoBuffer(MVoiceBuffer,length);
    #endif

    for (uint32_t ev=0; ev<num_events; ev++) {
      const clap_event_header_t* header = process->in_events->get(process->in_events,ev);
      // todo if header space..
      if (header->time > current_time) {
        // event is in the 'future'
        uint32_t num = header->time - current_time;
        // process num samples from current_time
        for (uint32_t v=0; v<VOICE_COUNT; v++) {
          uint32_t state = MVoices[v].getState();
          if ((state == MIP_VOICE_PLAYING)
            || (state == MIP_VOICE_RELEASED)
            || (state == MIP_VOICE_WAITING)) {
            MVoices[v].setState(  MVoices[v].process( state, num, current_time) );
          }
        }
        current_time += num;
        remaining -= num;
      }
      handleEvent(header);
    }
    // calculate remaining samples (no more events)
    if (remaining > 0) {
      for (uint32_t v=0; v<VOICE_COUNT; v++) {
        uint32_t state = MVoices[v].getState();
        if ((state == MIP_VOICE_PLAYING)
          || (state == MIP_VOICE_RELEASED)
          /*|| (state == MIP_VOICE_WAITING)*/) {
          MVoices[v].setState( MVoices[v].process( state, remaining, current_time) );
        }
      }
    }

    // sum voices

    #ifdef MIP_VOICE_ADD_TO_BUFFER

      MIP_CopyMonoBuffer(MFrameBuffer,MVoiceBuffer,length);

    #else

      MIP_ClearMonoBuffer(MFrameBuffer,length);
      for (uint32_t v=0; v<VOICE_COUNT; v++) {
        uint32_t state = MVoices[v].getState();
        if ((state == MIP_VOICE_PLAYING)
          || (state == MIP_VOICE_RELEASED)
          /*|| (state == MIP_VOICE_WAITING)*/) {
          float* dst = MFrameBuffer;
          float* src = MVoiceBuffer + (v * MIP_VOICE_MAX_FRAMESIZE);
          MIP_AddMonoBuffer(dst,src,length);
        }
      }

    #endif

    // counting

//    MNumPlayingVoices = 0;
//    MNumReleasedVoices = 0;
//    for (uint32_t v=0; v<VOICE_COUNT; v++) {
//      uint32_t state = MVoices[v].getState();
//      if (state == MIP_VOICE_PLAYING) MNumPlayingVoices += 1;
//      if (state == MIP_VOICE_RELEASED) MNumReleasedVoices += 1;
//    }

  }

//------------------------------
public:
//------------------------------

  /*
    events are split for each voice
    then each voice is calculated (separately)
    and finally, all voice buffers are summed to frame buffer
  */

  // Split voices

  // voices are processed individually, bu host-provided thread pool
  // (or manually one by one if thread pool not availablle

  void processPreparedVoices(MIP_ProcessContext* AContext) {
    const clap_process_t* process = AContext->process;
    //MIP_PRINT;
    //count_param_events(process);
    MVoiceContext.process = process;
    float* out0 = process->audio_outputs->data32[0];
    float* out1 = process->audio_outputs->data32[1];
    MVoiceContext.voicebuffer = MVoiceBuffer;//out0;
    uint32_t length = process->frames_count;

    // clear frame buffer (voices are added to it)
    MIP_ClearMonoBuffer(MFrameBuffer,length);

    // prepare events (separate them for each individual voice)
    //prepareEvents(AContext);//(process->in_events,process->out_events);

    // setup threaded voice array
    uint32_t num_voices  = 0;
    for (uint32_t i=0; i<VOICE_COUNT; i++) {
      if ((MVoices[i].getState() == MIP_VOICE_PLAYING)
        || (MVoices[i].getState() == MIP_VOICE_RELEASED)
        || (MVoices[i].getState() == MIP_VOICE_WAITING)) {
        MActiveVoices[num_voices++] = i;
      }
    }

    // calc voices
    if (num_voices > 0) {
      processActiveVoices(num_voices);
    }

    // add all voice buffers to frame buffer
    for (uint32_t i=0; i<num_voices; i++) {
      int32_t voice = MActiveVoices[i];
      float* src = MVoiceBuffer + (process->frames_count * voice);
      MIP_AddMonoBuffer(MFrameBuffer,src,length);
    }

    // cleanup..
    flushFinishedVoices();
    flushNoteEnds();

    // copy frame buffer to output
    MIP_CopyMonoBuffer(out0,MFrameBuffer,length);
    MIP_CopyMonoBuffer(out1,MFrameBuffer,length);

  }

  //----------

  // called by host for each voice

  void processVoice(uint32_t index) {
    int32_t voice = MActiveVoices[index];
    if (voice >= 0) {
      //MVoices[voice].processPrepared(voice);
    }
  }

  //----------

  // if there is no thread pool, or host refused for some reason,
  // we just calculate the voices manyally, one by one..
  // coud do our own threading...

  void processActiveVoices(uint32_t num) {
    for (uint32_t i=0; i<num; i++) {
      processVoice(i);
    }
  }

//------------------------------
public: // handle events NOW
//------------------------------

  void handleEvent(const clap_event_header_t* header) {
    switch (header->type) {
      case CLAP_EVENT_NOTE_ON:          handleNoteOn((const clap_event_note_t*)header);                     break;
      case CLAP_EVENT_NOTE_OFF:         handleNoteOff((const clap_event_note_t*)header);                    break;
      case CLAP_EVENT_NOTE_CHOKE:       handleNoteChoke((const clap_event_note_t*)header);                  break;
      case CLAP_EVENT_NOTE_EXPRESSION:  handleNoteExpression((const clap_event_note_expression_t*)header);  break;
      case CLAP_EVENT_PARAM_VALUE:      handleParamValue((const clap_event_param_value_t*)header);          break;
      case CLAP_EVENT_PARAM_MOD:        handleParamMod((const clap_event_param_mod_t*)header);              break;
      case CLAP_EVENT_MIDI:             handleMidi((const clap_event_midi_t*)header);                       break;
      case CLAP_EVENT_MIDI_SYSEX:       handleMidiSysex((const clap_event_midi_sysex_t*)header);            break;
      case CLAP_EVENT_MIDI2:            handleMidi2((const clap_event_midi2_t*)header);                     break;
    }
  }

  //----------

  /*
    note_id: 16,32, 48, ...
  */

  void handleNoteOn(const clap_event_note_t* event) {
    //MIP_Print("NOTE ON: key %i channel %i port %i note_id %i\n",event->key,event->channel,event->port_index,event->note_id);
    int32_t voice = findAvailableVoice(true); // true = search released voices too
    if (voice >= 0) {
      if (MVoices[voice].getState() == MIP_VOICE_RELEASED) {
        queueNoteEnd(MVoices[voice].getNote());
      }
      MVoices[voice].note_on(event->key,event->velocity);
      MVoices[voice].setNote(event->port_index,event->channel,event->key,event->note_id);
    }
    else {
      MIP_Note note;
      note.key         = event->key;
      note.channel     = event->channel;
      note.port        = event->port_index;
      note.noteid      = event->note_id;
      queueNoteEnd(note);
    }
  }

  //----------

  // note_id is always -1

  void handleNoteOff(const clap_event_note_t* event) {
    //MIP_Print("NOTE OFF: key %i channel %i port %i note_id %i\n",event->key,event->channel,event->port_index,event->note_id);
    for (uint32_t i=0; i<VOICE_COUNT; i++) {
      if (MVoices[i].getState() == MIP_VOICE_PLAYING) {
        //if (MVoices[i].note.note_id == event->note_id) {
          if ((MVoices[i].getNote().channel == event->channel) && (MVoices[i].getNote().key == event->key)) {
            MVoices[i].note_off(event->velocity);
          }
        //}
      }
    }
  }

  //----------

  void handleNoteChoke(const clap_event_note_t* event) {
    //MIP_Print("NOTE CHOKE: key %i channel %i port %i note_id %i\n",event->key,event->channel,event->port_index,event->note_id);
  }

  //----------

  // note_id is always -1

  void handleNoteExpression(const clap_event_note_expression_t* event) {
    //MIP_Print("id %i port %i chan %i key %i val %.3f\n",event->note_id,event->port_index,event->channel,event->key,event->value);
    for (uint32_t i=0; i<VOICE_COUNT; i++) {
      //if (MVoices[i].note.note_id == event->note_id) {
        if ((MVoices[i].getNote().key == event->key) && (MVoices[i].getNote().channel == event->channel)) {
          MVoices[i].expression(event->expression_id,event->value);
        }
      //}
    }
  }

  //----------

  /*
    parameter.flags : CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID

    called from event
    see also: voiceParameter (called from gui)
  */

  void handleParamValue(const clap_event_param_value_t* event) {
    //MIP_Print("param %i note %i port %i chan %i key %i value %.3f\n",event->param_id,event->note_id,event->port_index,event->channel,event->key,event->value);
    if (event->note_id == -1) {
      // global
      for (uint32_t i=0; i<VOICE_COUNT; i++) {
        //if ((MVoices[i].note.key == event->key) && (MVoices[i].note.channel == event->channel)) {
          MVoices[i].parameter(event->param_id,event->value);
        //}
        //}
      }
    }
    else {
      // voice
      for (uint32_t i=0; i<VOICE_COUNT; i++) {
        if (MVoices[i].getNote().noteid == event->note_id) {
          //if ((MVoices[i].note.key == event->key) && (MVoices[i].note.channel == event->channel)) {
            MVoices[i].parameter(event->param_id,event->value);
          //}

        }
      }
    }
  }

  //----------

  // parameter.flags : CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID
  // mono: all is -l
  // poly: all is ok

  void handleParamMod(const clap_event_param_mod_t* event) {
    //MIP_Print("param %i note %i port %i chan %i key %i amount %.3f\n",event->param_id,event->note_id,event->port_index,event->channel,event->key,event->amount);
    if (event->note_id == -1) {
      // global
      for (uint32_t i=0; i<VOICE_COUNT; i++) {
        if ((MVoices[i].getState() == MIP_VOICE_PLAYING) || (MVoices[i].getState() == MIP_VOICE_RELEASED)) {
        //if ((MVoices[i].note.key == event->key) && (MVoices[i].note.channel == event->channel)) {
          MVoices[i].modulation(event->param_id,event->amount);
        //}
        }
      }
    }
    else {
      // voice
      for (uint32_t i=0; i<VOICE_COUNT; i++) {
        if (MVoices[i].getNote().noteid == event->note_id) {
          //if ((MVoices[i].note.key == event->key) && (MVoices[i].note.channel == event->channel)) {
          MVoices[i].modulation(event->param_id,event->amount);
          //}
        }
      }
    }
  }

  //----------

  void handleMidi(const clap_event_midi_t* event) {
    //MIP_Print("port %i data %02x,%02x,%02x\n",event->port_index,event->data[0],event->data[1],event->data[2]);
  }

  //----------

  void handleMidiSysex(const clap_event_midi_sysex_t* event) {
    //MIP_Print("port %i size %i\n",event->port_index,event->size);
  }

  //----------

  void handleMidi2(const clap_event_midi2_t* event) {
    //MIP_Print("port %i data %i,%i,%i,%i\n",event->port_index,event->data[0],event->data[1],event->data[2],event->data[3]);
  }

//------------------------------
public: // prepare events per voice
//------------------------------

  /*

  // Split voices

  //int32_t findVoice(const clap_event_note_t* event) {
  //  return -1;
  //}

  //----------

  void prepareEvents(MIP_ProcessContext* AContext) {
    const clap_process_t* process = AContext->process;
    uint32_t num_events = process->in_events->size(process->in_events);
    for (uint32_t ev=0; ev<num_events; ev++) {
      const clap_event_header_t* header = process->in_events->get(process->in_events,ev);
      // todo if header space..
      prepareEvent(header);
      //uint32_t num = header->time;
    }
  }

  //----------

  void prepareEvent(const clap_event_header_t* header) {
    switch (header->type) {
      case CLAP_EVENT_NOTE_ON:          prepareNoteOn((const clap_event_note_t*)header);                     break;
      case CLAP_EVENT_NOTE_OFF:         prepareNoteOff((const clap_event_note_t*)header);                    break;
      case CLAP_EVENT_NOTE_CHOKE:       prepareNoteChoke((const clap_event_note_t*)header);                  break;
      case CLAP_EVENT_NOTE_EXPRESSION:  prepareNoteExpression((const clap_event_note_expression_t*)header);  break;
      case CLAP_EVENT_PARAM_VALUE:      prepareParamValue((const clap_event_param_value_t*)header);          break;
      case CLAP_EVENT_PARAM_MOD:        prepareParamMod((const clap_event_param_mod_t*)header);              break;
      case CLAP_EVENT_MIDI:             prepareMidi((const clap_event_midi_t*)header);                       break;
      case CLAP_EVENT_MIDI_SYSEX:       prepareMidiSysex((const clap_event_midi_sysex_t*)header);            break;
      case CLAP_EVENT_MIDI2:            prepareMidi2((const clap_event_midi2_t*)header);                     break;
    }
  }

  //----------

  void prepareNoteOn(const clap_event_note_t* event) {
    MIP_PRINT;
    int32_t voice = findAvailableVoice(true); // true = search released voices too
    if (voice >= 0) {
      if (MVoices[voice].getState() == MIP_VOICE_RELEASED) {
        queueNoteEnd(MVoices[voice].getNote());
      }
      MVoices[voice].prepare_note_on(event->header.time,event->key,event->velocity);
    }
    else {
      MIP_Note note;
      note.key         = event->key;
      note.channel     = event->channel;
      note.port_index  = event->port_index;
      note.note_id     = event->note_id;
      queueNoteEnd(note);
    }
  }

  //----------

  void prepareNoteOff(const clap_event_note_t* event) {
    MIP_PRINT;
    for (uint32_t i=0; i<VOICE_COUNT; i++) {
      MIP_Note note = MVoices[i].getNote();
      if (voiceMatchesEvent(note,event)) {
        MVoices[i].prepare_note_off(event->header.time,event->velocity);
      }
    }
  }

  //----------

  void prepareNoteChoke(const clap_event_note_t* event) {
    MIP_PRINT;
  }

  //----------

  void prepareNoteExpression(const clap_event_note_expression_t* event) {
    MIP_PRINT;
  }

  //----------

  void prepareParamValue(const clap_event_param_value_t* event) {
    MIP_PRINT;
  }

  //----------

  void prepareParamMod(const clap_event_param_mod_t* event) {
    MIP_PRINT;
  }

  //----------

  void prepareMidi(const clap_event_midi_t* event) {
    MIP_PRINT;
  }

  //----------

  void prepareMidiSysex(const clap_event_midi_sysex_t* event) {
    MIP_PRINT;
  }

  //----------

  void prepareMidi2(const clap_event_midi2_t* event) {
    MIP_PRINT;
  }

  */

//------------------------------
private: // voices
//------------------------------

  int32_t findQuietestReleasedVoice() {
    float lowest = 999999.0;
    int32_t voice = -1;
    for (uint32_t i=0; i<VOICE_COUNT; i++) {
      if (MVoices[i].getState() == MIP_VOICE_RELEASED) {
        float env = MVoices[i].getLevel();
        if (env < lowest) {
          voice = i;
          lowest = env;
        }
      }
    }
    return voice;
  }

  //----------

  int32_t findAvailableVoice(bool ATryReleased=false) {
    for (uint32_t i=0; i<VOICE_COUNT; i++) {
      if (MVoices[i].getState() == MIP_VOICE_OFF) return i;
    }
    if (ATryReleased) {
      return findQuietestReleasedVoice();
    }
    return -1;
  }

//------------------------------
public:
//------------------------------

  void flushFinishedVoices() {
    for (uint32_t i=0; i<VOICE_COUNT; i++) {
      if (MVoices[i].getState() == MIP_VOICE_FINISHED) {
        queueNoteEnd(MVoices[i].getNote());
        MVoices[i].setState(MIP_VOICE_OFF);
        MVoices[i].clearNote();
      }
      if (MVoices[i].getState() == MIP_VOICE_WAITING) {
        /*
          wasn't started, or stopped again?
          do we need to send note_end?
          (could there be dangling mod voices?)
        */
        MVoices[i].setState(MIP_VOICE_OFF);
        MVoices[i].clearNote();
      }
    }
  }

  //----------

  void queueNoteEnd(MIP_Note ANote) {
    MNoteEndQueue.write(ANote);
  }

  //----------

  void flushNoteEnds() {
    const clap_process_t* process = MVoiceContext.process;
    const clap_output_events_t* out_events = process->out_events;
    MIP_Note note;
    while (MNoteEndQueue.read(&note)) {
      sendNoteEnd(note,out_events);
    }
  }

  //----------

  void sendNoteEnd(MIP_Note ANote, const clap_output_events_t* out_events) {
    clap_event_note_t event;
    clap_event_header_t* header = (clap_event_header_t*)&event;
    header->size      = sizeof(clap_event_note_t);
    header->time      = 0; // crashed if this was anything else than 0
    header->space_id  = CLAP_CORE_EVENT_SPACE_ID;
    header->type      = CLAP_EVENT_NOTE_END;
    header->flags     = 0;
    event.note_id     = ANote.noteid;
    event.port_index  = ANote.port;
    event.key         = ANote.key;
    event.channel     = ANote.channel;
    event.velocity    = 0.0;
    out_events->try_push(out_events,header);
  }

};

//----------------------------------------------------------------------
#endif
