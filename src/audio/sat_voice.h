#ifndef sat_voice_included
#define sat_voice_included
//----------------------------------------------------------------------

#include "plugin/sat_process_context.h"
#include "audio/sat_voice_context.h"

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

typedef SAT_LockFreeQueue<SAT_VoiceEvent,SAT_VOICE_MAX_EVENTS_PER_BLOCK> SAT_VoiceEventQueue;

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
  uint32_t            event_mode  = SAT_PLUGIN_EVENT_MODE_BLOCK;
  SAT_VoiceEventQueue events      = {};

//SAT_ParameterArray* parameters  = nullptr;

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
      case SAT_PLUGIN_EVENT_MODE_BLOCK: {
        handleBlockEvents();
        break;
      }
      case SAT_PLUGIN_EVENT_MODE_INTERLEAVED: {
        handleInterleavedEvents();
        break;
      }
      case SAT_PLUGIN_EVENT_MODE_QUANTIZED: {
        handleQuantizedEvents();
        break;
      }
    }
  }

  //----------

  sat_sample_t getEnvLevel() {
    return voice.getEnvLevel();
  }
  
  //----------

  //double getVolume() {}
  //double getPanning() {}

//------------------------------
private:
//------------------------------

  void handleEvent(SAT_VoiceEvent event) {
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
    } // switch
  }

  //----------

  void handleBlockEvents() {
    SAT_VoiceEvent event;
    while (events.read(&event)) handleEvent(event);
    uint32_t length = context->process_context->voice_length;
    state = voice.process(state,0,length);
  }

  //----------

  void handleInterleavedEvents() {
    uint32_t current_time = 0;
    uint32_t remaining = context->process_context->voice_length;
    SAT_VoiceEvent event = {};
    while (remaining > 0) {
      
      if (events.read(&event)) {
        //SAT_Print("event\n");
        
        int32_t length = event.time - current_time;
        
        //if (length < 0) {
        //  SAT_Print("length < 0\n");
        //  length = 0;
        //}
        
        //SAT_Assert(length >= 0);
        //SAT_Assert((current_time + length) <= context->process_context->voice_length);
        
        if (length > 0) {
          state = voice.process(state,current_time,length);
          remaining -= length;
          current_time += length;
        }
        
        handleEvent(event);
        
      } // event
      else {
        
        // no more events
        int32_t length = remaining;
        state = voice.process(state,current_time,length);
        remaining -= length;
        current_time += length;
        
      } // !event
    } // remaining > 0
    
    SAT_Assert( events.read(&event) == false );

  }

  //----------

  void handleQuantizedEvents() {
    uint32_t buffer_length = context->process_context->voice_length;
    uint32_t        current_time  = 0;
    uint32_t        remaining     = buffer_length;
    uint32_t        next_event    = 0;
    SAT_VoiceEvent  event         = {};
    if (events.read(&event)) {
      next_event = event.time;
      do {
        // process events for next slice
        while (next_event < (current_time + SAT_AUDIO_QUANTIZED_SIZE)) {
          handleEvent(event);
          if (events.read(&event)) next_event = event.time;
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
    //SAT_Assert( events.read(&event) == false );
  }

  //----------

};

//----------------------------------------------------------------------
#endif
