#ifndef mip_voice_included
#define mip_voice_included
//----------------------------------------------------------------------

#include "base/mip.h"
#include "plugin/mip_note.h"

//----------------------------------------------------------------------

//#define MIP_VOICE_MAX_EVENTS_PER_BLOCK 256

//----------

enum MIP_EVoiceEvents {
  MIP_VOICE_EVENT_NOTE_ON         = 0,
  MIP_VOICE_EVENT_NOTE_OFF        = 1,
  MIP_VOICE_EVENT_NOTE_CHOKE      = 2,
  MIP_VOICE_EVENT_NOTE_EXPRESSION = 3,
  MIP_VOICE_EVENT_PARAMETER       = 4,
  MIP_VOICE_EVENT_MODULATION      = 5
};

//----------

struct MIP_VoiceEvent {
  uint32_t  type  = 0;
  uint32_t  time  = 0;
  uint32_t  index = 0;
  float     value = 0;
  MIP_VoiceEvent() {}
  MIP_VoiceEvent(uint32_t typ, uint32_t tim, uint32_t ind, float val) {
    type  = typ;
    time  = tim;
    index = ind;
    value = val;
  }
};

//----------

struct MIP_VoiceContext {
//float*                voicebuffer0   = nullptr;
//float*                voicebuffer1   = nullptr;
  float*                voicebuffer   = nullptr;
  const clap_process_t* process       = nullptr;
  double                samplerate    = 0.0;
  double                invsamplerate = 0.0;
  MIP_ParameterArray*   parameters    = nullptr;
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

template <class VOICE>
class MIP_Voice {

//------------------------------
public:
//------------------------------

  uint32_t          MIndex        = 0;
  VOICE             MVoice        = {};
  MIP_VoiceContext* MContext      = nullptr;
  uint32_t          MState        = MIP_VOICE_OFF;
  MIP_Note          MNote         = {};

  // Split voices
  MIP_Queue<MIP_VoiceEvent,MIP_VOICE_MAX_EVENTS_PER_BLOCK*2> events = {};

//------------------------------
public:
//------------------------------

  void setup(uint32_t AIndex, MIP_VoiceContext* AContext) {
    MIndex = AIndex;
    MContext = AContext;
    MState = MIP_VOICE_OFF;
    MVoice.setup(AIndex,MContext);
  }

  //----------

  double    getLevel()                { return MVoice.getVoiceLevel(); }

  uint32_t  getState()                { return MState; }
  void      setState(uint32_t AState) { MState = AState; }

  //

  MIP_Note getNote() {
    return MNote;
  }

  void setNote(MIP_Note ANote) {
    MNote = ANote;
  }

  void clearNote() {
    MNote.port        = -1;
    MNote.channel     = -1;
    MNote.key         = -1;
    MNote.noteid      = -1;
  }

  void setNote(int32_t p, int32_t c, int32_t k, int32_t n) {
    MNote.port        = p;
    MNote.channel     = c;
    MNote.key         = k;
    MNote.noteid      = n;
  }

  void setNotePortIndex(int32_t p) {
    MNote.port        = p;
  }

  void setNoteChannel(int32_t c) {
    MNote.channel = c;
  }
  void setNoteKey(int32_t k) {
    MNote.key = k;
  }
  void setNoteNoteId(int32_t n) {
    MNote.noteid  = n;
  }
//------------------------------
public: // interleaved events
//------------------------------

  uint32_t process(uint32_t AState, uint32_t ASize, uint32_t AOffset) {
    return MVoice.process(AState,ASize,AOffset);
  }

//------------------------------
public:
//------------------------------

  void handleVoiceEvent(MIP_VoiceEvent event) {
    switch (event.type) {
      case MIP_VOICE_EVENT_NOTE_ON:         note_on(event.index,event.value);     break;
      case MIP_VOICE_EVENT_NOTE_OFF:        note_off(event.value);                break;
      case MIP_VOICE_EVENT_NOTE_CHOKE:      note_choke();                         break;
      case MIP_VOICE_EVENT_NOTE_EXPRESSION: expression(event.index,event.value);  break;
      case MIP_VOICE_EVENT_PARAMETER:       parameter(event.index,event.value);   break;
      case MIP_VOICE_EVENT_MODULATION:      modulation(event.index,event.value);  break;
    }
  }

  //----------

  void note_on(int32_t key, double velocity) {
    MState = MVoice.note_on(key,velocity);
  }

  //----------

  void note_off(double velocity) {
    MState = MVoice.note_off(velocity);
  }

  //----------

  void note_choke() {
    MVoice.note_choke();
    MState = MIP_VOICE_FINISHED;
  }

  //----------

  void expression(uint32_t type, double value) {
    switch (type) {
      case CLAP_NOTE_EXPRESSION_VOLUME:     MVoice.volume(value);      break;
      case CLAP_NOTE_EXPRESSION_PAN:        MVoice.pan(value);         break;
      case CLAP_NOTE_EXPRESSION_TUNING:     MVoice.tuning(value);      break;
      case CLAP_NOTE_EXPRESSION_VIBRATO:    MVoice.vibrato(value);     break;
      case CLAP_NOTE_EXPRESSION_EXPRESSION: MVoice.expression(value);  break;
      case CLAP_NOTE_EXPRESSION_BRIGHTNESS: MVoice.brightness(value);  break;
      case CLAP_NOTE_EXPRESSION_PRESSURE:   MVoice.pressure(value);    break;
    }
  }

  //----------

  void parameter(uint32_t index, double value) {
    MVoice.parameter(index,value);
  }

  //----------

  void modulation(uint32_t index, double value) {
    MVoice.modulation(index,value);
  }

//------------------------------
public: // prepared events
//------------------------------

  // Split voices

  void prepare_note_on(uint32_t time, int32_t key, double velocity) {
    MIP_VoiceEvent ev = MIP_VoiceEvent(MIP_VOICE_EVENT_NOTE_ON,time,key,velocity);
    events.write(ev);
    MState = MIP_VOICE_WAITING;
  }

  //----------

  void prepare_note_off(uint32_t time, double velocity) {
    MIP_VoiceEvent ev = MIP_VoiceEvent(MIP_VOICE_EVENT_NOTE_OFF,time,0,velocity);
    events.write(ev);
  }

  //----------

  void prepare_note_choke(uint32_t time) {
    MIP_VoiceEvent ev = MIP_VoiceEvent(MIP_VOICE_EVENT_NOTE_CHOKE,time,0,0);
    events.write(ev);
  }

  //----------

  void prepare_expression(uint32_t time, uint32_t type, double value) {
    MIP_VoiceEvent ev = MIP_VoiceEvent(MIP_VOICE_EVENT_NOTE_EXPRESSION,time,type,value);
    events.write(ev);
  }

  //----------

  void prepare_parameter(uint32_t time, uint32_t index, double value) {
    MIP_VoiceEvent ev = MIP_VoiceEvent(MIP_VOICE_EVENT_PARAMETER,time,index,value);
    events.write(ev);
  }

  //----------

  void prepare_modulation(uint32_t time, uint32_t index, double value) {
    MIP_VoiceEvent ev = MIP_VoiceEvent(MIP_VOICE_EVENT_MODULATION,time,index,value);
    events.write(ev);
  }

  //----------

  //todo: add offset to current_time before calling MVoice.process
  //so we get rid of this in the voice:
  //#ifdef MIP_VOICE_PREPARE_EVENTS
  //  MVoiceManager.processPrepared(process,MHost);
  //#else
  //  MVoiceManager.processBlock(process);
  //#endif

  void processPrepared() {
    const clap_process_t* process = MContext->process;
    uint32_t current_time = 0;
    uint32_t remaining = process->frames_count;
    MIP_VoiceEvent next_event;
    while (remaining > 0) {

      if (events.read(&next_event)) {
        // we have more events
        //MIP_Print("current_time: %i next_event.time: %i\n",current_time,next_event.time);
        int32_t length = next_event.time - current_time;
        if (length > 0) {
          if (MState != MIP_VOICE_WAITING) {
          //if ((MState == MIP_VOICE_PLAYING) || ((MState == MIP_VOICE_RELEASED)) {
            MState = MVoice.process(MIndex,MState,length,current_time);
          }
          remaining -= length;
          current_time += length;
        }
        handleVoiceEvent(next_event);
      } // event

      else {
        // no more events
        int32_t length = remaining;
        if (MState != MIP_VOICE_WAITING) {
        //if ((MState == MIP_VOICE_PLAYING) || ((MState == MIP_VOICE_RELEASED)) {
          MState = MVoice.process(MIndex,MState,length,current_time);
        }
        remaining -= length;
        current_time += length;
      } // !event

    } // remaining > 0
    MIP_Assert( events.read(&next_event) == false );
  }

};

//----------------------------------------------------------------------
#endif
