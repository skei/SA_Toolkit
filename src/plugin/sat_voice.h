#ifndef sat_voice_included
#define sat_voice_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/sat_note.h"
#include "plugin/sat_plugin_base.h"

//----------------------------------------------------------------------

struct SAT_VoiceEvent {
  uint32_t time   = 0;
  uint32_t type   = 0;
  uint32_t index  = 0;
  double   value  = 0.0;
  SAT_VoiceEvent(uint32_t ty=0, uint32_t ti=0, uint32_t in=0, double va=0.0) {
    type  = ty;
    time  = ti;
    index = in;
    value = va;
  }
};

//typedef SAT_Queue<SAT_VoiceEvent,SAT_VOICE_MAX_EVENTS_PER_BLOCK> SAT_VoiceEventQueue;
//typedef SAT_AtomicQueue<SAT_VoiceEvent,SAT_VOICE_MAX_EVENTS_PER_BLOCK> SAT_VoiceEventQueue;
typedef moodycamel::ReaderWriterQueue<SAT_VoiceEvent> SAT_VoiceEventQueue;



//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

template <class VOICE>
class SAT_Voice {

//------------------------------
public:
//------------------------------

  __SAT_ALIGNED(SAT_ALIGNMENT_CACHE)
  VOICE               voice       = {};
  
  SAT_VoiceContext*   context     = nullptr;
  uint32_t            index       = 0;

  SAT_Note            note        = {};
  uint32_t            state       = SAT_VOICE_OFF;
  uint32_t            event_mode  = SAT_VOICE_EVENT_MODE_INTERLEAVED;
  SAT_VoiceEventQueue events;//      = {};

//------------------------------
public:
//------------------------------

  SAT_Voice() {
  }

  //----------

  ~SAT_Voice() {
  }

//------------------------------
public:
//------------------------------

  void init(uint32_t AIndex, SAT_VoiceContext* AContext) {
    //parameters = AParameters;
    context = AContext;
    index = AIndex;
    voice.init(AIndex,context);
  }

  //----------

  void process() {
    switch (event_mode) {
      case SAT_VOICE_EVENT_MODE_BLOCK: {
        handleBlockEvents();
        break;
      }
      case SAT_VOICE_EVENT_MODE_INTERLEAVED: {
        handleInterleavedEvents();
        break;
      }
      case SAT_VOICE_EVENT_MODE_QUANTIZED: {
        handleQuantizedEvents();
        break;
      }
    }
  }

  //----------

  sat_sample_t getVoiceLevel() {
    return voice.getVoiceLevel();
  }
  
  //----------
  
  void clearVoiceQueue() {
    SAT_VoiceEvent event;
    //while (events.read(&event)) {}
    while (events.try_dequeue(event)) {}
  }

  //double getVolume() {}
  //double getPanning() {}

//------------------------------
private:
//------------------------------

  void handleEvent(SAT_VoiceEvent event) {
    //SAT_PRINT("voice index %i\n",index);
    switch(event.type) {
      case CLAP_EVENT_NOTE_ON: {
        state = voice.noteOn(event.index,event.value);
        break;
      }
      case CLAP_EVENT_NOTE_OFF: {
        state = voice.noteOff(event.index,event.value);
        break;
      }
      case CLAP_EVENT_NOTE_CHOKE: {
        /*state =*/ voice.noteChoke(event.index,event.value);
        break;
      }
      //case CLAP_EVENT_NOTE_END:
      //  break;
      case CLAP_EVENT_NOTE_EXPRESSION: {
        /*state =*/ voice.noteExpression(event.index,event.value);
        break;
      }
      case CLAP_EVENT_PARAM_VALUE: {
        voice.parameter(event.index,event.value);
        break;
      }
      case CLAP_EVENT_PARAM_MOD: {
        voice.modulation(event.index,event.value);
        break;
      }
      //case CLAP_EVENT_PARAM_GESTURE_BEGIN:
      //case CLAP_EVENT_PARAM_GESTURE_END:
      //case CLAP_EVENT_TRANSPORT:
      //case CLAP_EVENT_MIDI:
      //case CLAP_EVENT_MIDI_SYSEX:
      //case CLAP_EVENT_MIDI2:
      //  break;
    } // switch
  }

  //----------

  void handleBlockEvents() {
    SAT_VoiceEvent event;
    //while (events.read(&event)) handleEvent(event);
    while (events.try_dequeue(event)) {
      handleEvent(event);
    }
    uint32_t length = context->block_length;
    state = voice.process(state,0,length);
   
//    SAT_Assert( events.try_dequeue(&event) == false );
    
  }

  //----------

  void handleInterleavedEvents() {
    uint32_t event_count = 0;
    uint32_t current_time = 0;
    uint32_t remaining = context->block_length;         // !!!!! remaining = 0
    SAT_VoiceEvent event = {};

    while (remaining > 0) {
      //if (events.read(&event)) {
      if (events.try_dequeue(event)) {
        int32_t length = event.time - current_time; // samples until next event
        //SAT_Assert(length >= 0);
        //SAT_Assert((current_time + length) <= context->process_context->voice_length);
        if (length < 0) {
          SAT_PRINT("events not sorted! voice %i current_time %i, event.time %i type %i event_count %i\n",index,current_time,event.time,event.type,event_count);
        }
        //else if (length == 0) {
        //}
        else if (length > 0) {
          state = voice.process(state,current_time,length);
          remaining -= length;
          current_time += length;
        }
        handleEvent(event);
        event_count += 1;
      } // event
      else {
        // no more events
        int32_t length = remaining;
        state = voice.process(state,current_time,length);
        remaining -= length;
        current_time += length;
      } // !event
    } // remaining > 0
    
//    SAT_Print("remaining %i current_time %i\n",remaining,current_time);

    //SAT_Assert( events.read(&event) == false );
    SAT_Assert( events.try_dequeue(event) == false );
    
  }

  //----------
  
  void handleQuantizedEvents() {
    uint32_t buffer_length = context->block_length;
    uint32_t current_time  = 0;
    uint32_t remaining     = buffer_length;
    uint32_t next_event    = 0;
    SAT_VoiceEvent  event         = {};
    //if (events.read(&event)) {
    if (events.try_dequeue(event)) {
      next_event = event.time;
      do {
        // process events for next slice
        while (next_event < (current_time + SAT_AUDIO_QUANTIZED_SIZE)) {
          handleEvent(event);
          //if (events.read(&event)) next_event = event.time;
          if (events.try_dequeue(event)) next_event = event.time;
          else next_event = buffer_length; // ???
        }
        // process next slice
        if (remaining < SAT_AUDIO_QUANTIZED_SIZE) {
          state = voice.process(state,current_time,remaining);
          current_time += remaining;
          remaining = 0;
        }
        else {
          state = voice.processSlice(state,current_time);
          current_time += SAT_AUDIO_QUANTIZED_SIZE;
          remaining -= SAT_AUDIO_QUANTIZED_SIZE;
        }
      } while (remaining > 0);
    }
    else {
      // no events..
      do {
        if (remaining < SAT_AUDIO_QUANTIZED_SIZE) state = voice.process(state,current_time,remaining);
        else state = voice.processSlice(state,current_time);
        current_time += SAT_AUDIO_QUANTIZED_SIZE;
        remaining -= SAT_AUDIO_QUANTIZED_SIZE;
      } while (remaining > 0);
    }
    
//    SAT_Assert( events.try_dequeue(event) == false );
    
  }

  //----------

};


//----------------------------------------------------------------------
#endif




















#if 0


#endif // 0