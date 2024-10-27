#ifndef sat_voice_processor_included
#define sat_voice_processor_included
//----------------------------------------------------------------------

#include "sat.h"
#include "audio/sat_audio_utils.h"
#include "base/system/sat_cpu.h"
#include "plugin/sat_note.h"
#include "plugin/sat_plugin_base.h"
#include "plugin/sat_processor.h"
#include "plugin/sat_voice.h"

#ifdef SAT_VOICE_PROCESSOR_THREADED
  #include "base/util/sat_thread_pool.h"
#endif


//----------

//typedef SAT_Queue<SAT_Note,SAT_VOICE_PROCESSOR_MAX_EVENTS_PER_BLOCK> SAT_NoteQueue;
//typedef SAT_AtomicQueue<SAT_Note,SAT_VOICE_PROCESSOR_MAX_EVENTS_PER_BLOCK> SAT_NoteQueue;
typedef moodycamel::ReaderWriterQueue<SAT_Note> SAT_NoteQueue;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

template <class VOICE, int COUNT>
class SAT_VoiceProcessor
: public SAT_Processor
#ifdef SAT_VOICE_PROCESSOR_THREADED
, public SAT_ThreadPoolListener {
  #else
{
  #endif

//------------------------------
protected:
//------------------------------

  // __SAT_ALIGNED(SAT_ALIGNMENT_CACHE)
  SAT_Voice<VOICE>              MVoices[COUNT]          = {};

  // __SAT_ALIGNED(SAT_ALIGNMENT_CACHE)
  float MVoiceBuffer[COUNT * SAT_PLUGIN_MAX_BLOCK_SIZE] = {0};
  
  SAT_VoiceContext              MVoiceContext           = {};
  SAT_NoteQueue                 MNoteEndQueue;

  const clap_plugin_t*          MClapPlugin             = nullptr;
  const clap_host_t*            MClapHost               = nullptr;

  uint32_t                      MActiveVoices[COUNT]    = {};
  uint32_t                      MNumActiveVoices        = 0;
  uint32_t                      MNumPlayingVoices       = 0; // set up in processAudio (read by gui)
  uint32_t                      MNumReleasedVoices      = 0; // --"--

  double                        MSampleRate             = 0.0;
  uint32_t                      MEventMode              = SAT_VOICE_EVENT_MODE_BLOCK;

  bool                          MProcessThreaded        = false;

  #ifdef SAT_VOICE_PROCESSOR_THREADED
    #ifdef SAT_VOICE_PROCESSOR_CLAP_THREAD_POOL
      const clap_host_thread_pool* MClapThreadPool = nullptr;
    #endif
    SAT_ThreadPool* MThreadPool = nullptr;
  #endif

//------------------------------
public:
//------------------------------

  SAT_VoiceProcessor(SAT_ProcessorListener* AListener)
  : SAT_Processor(AListener) {
  }

  //----------

  virtual ~SAT_VoiceProcessor() {
    SAT_TRACE;
    #ifdef SAT_VOICE_PROCESSOR_THREADED
      #ifndef SAT_VOICE_PROCESSOR_CLAP_THREAD_POOL
        if (MThreadPool) delete MThreadPool;
      #endif
    #endif
  }

//------------------------------
public:
//------------------------------

  uint32_t  getMaxVoices()                  { return COUNT; }
  uint32_t  getNumPlayingVoices()           { return MNumPlayingVoices; }
  uint32_t  getNumReleasedVoices()          { return MNumReleasedVoices; }
  uint32_t  getVoiceState(uint32_t AIndex)  { return MVoices[AIndex].state; }
//uint32_t  getEventMode()                  { return MEventMode; }
  bool      isProcessingThreaded()          { return MProcessThreaded; }

  SAT_VoiceContext* getVoiceContext()       { return &MVoiceContext; }

//------------------------------
public:
//------------------------------

  void setProcessThreaded(bool AThreaded=true) {
    MProcessThreaded = AThreaded;
  }
  
  //----------

  void setEventMode(uint32_t AMode) {
    MEventMode = AMode;
    for (uint32_t i=0; i<COUNT; i++) {
      MVoices[i].event_mode = AMode;
    }
  }

//------------------------------
public:
//------------------------------

  void init(const clap_plugin_t* APlugin, const clap_host_t* AHost) {
    //SAT_Plugin* plugin = (SAT_Plugin*)APlugin->plugin_data;
    //MVoiceContext.plugin = plugin;
    MClapPlugin = APlugin;
    MClapHost = AHost;
    SAT_Assert(APlugin);
    SAT_Assert(AHost);
    #ifdef SAT_VOICE_PROCESSOR_THREADED
      SAT_ClapPlugin* plugin = (SAT_ClapPlugin*)APlugin->plugin_data;
      SAT_Assert(plugin);
      #ifdef SAT_VOICE_PROCESSOR_CLAP_THREAD_POOL
        // todo: if plugin->MSupportedExtensions.hasItem(CLAP_EXT_THREAD_POOL)
        if (plugin->findExtension(CLAP_EXT_THREAD_POOL)) {
          SAT_PRINT("use clap threadpool\n");
          MClapThreadPool = (const clap_host_thread_pool*)MClapHost->get_extension(MClapHost,CLAP_EXT_THREAD_POOL);
        }
      #endif
      //#else
      if (!MClapThreadPool) {
        SAT_PRINT("use sat_threadpool\n");
        uint32_t numprocs = SAT_GetNumProcessors();
        MThreadPool = new SAT_ThreadPool(this,numprocs); // SAT_VOICE_PROCESSOR_NUM_THREADS
        if (!MThreadPool) {
          SAT_PRINT("no threadpool\n");
        }
      }
      //#endif
    #endif
  }

  //----------

  void activate(double sample_rate, uint32_t min_frames_count, uint32_t max_frames_count) {
    MVoiceContext.process_context   = nullptr; //
    MVoiceContext.sample_rate       = sample_rate;
    MVoiceContext.min_frames_count  = min_frames_count;
    MVoiceContext.max_frames_count  = max_frames_count;
    MVoiceContext.voice_buffer      = MVoiceBuffer;
    for (uint32_t i=0; i<COUNT; i++) {
      MVoices[i].init(i,&MVoiceContext);
    }
    // send (initial) parameters to all voices?
  }

//------------------------------
public:
//------------------------------

  bool isTargeted(uint32_t index, int32_t noteid, int32_t port, int32_t channel, int32_t key) {
    if ((noteid  != -1) && (MVoices[index].note.noteid  != noteid))   return false;
    if ((port    != -1) && (MVoices[index].note.port    != port))     return false;
    if ((channel != -1) && (MVoices[index].note.channel != channel))  return false;
    if ((key     != -1) && (MVoices[index].note.key     != key))      return false;
    //SAT_Print( "index %i noteid %i port %i channel %i key %i == %i,%i,%i,%i",
    //  index, noteid, port, channel, key,
    //  MVoices[index].note.noteid, MVoices[index].note.port, MVoices[index].note.channel, MVoices[index].note.key );
    return true;
  }

  //----------
  
  bool isActive(uint32_t index) {
    if ( (MVoices[index].state == SAT_VOICE_WAITING) 
      || (MVoices[index].state == SAT_VOICE_PLAYING)
      || (MVoices[index].state == SAT_VOICE_RELEASED)) {
      return true;
    }
    return false;
  }

//------------------------------
public:
//------------------------------

  /*
    fill AProcessContext->block_length number of samples into
    AProcessContext->block_buffer

    float* buffer = MContext->voice_buffer;
    buffer += AOffset;
    buffer += (MIndex * SAT_PLUGIN_MAX_BLOCK_SIZE);
  */

  void processAudio(SAT_ProcessContext* AProcessContext) {
    MVoiceContext.process_context = AProcessContext;
    MVoiceContext.block_length    = AProcessContext->process->frames_count;

    // set up active voices

    MNumActiveVoices = 0;
    uint32_t num_playing = 0;
    uint32_t num_released = 0;
    for (uint32_t i=0; i<COUNT; i++) {
      if ((MVoices[i].state == SAT_VOICE_WAITING)
       || (MVoices[i].state == SAT_VOICE_PLAYING)
       || (MVoices[i].state == SAT_VOICE_RELEASED)) {
        MActiveVoices[MNumActiveVoices] = i;
        MNumActiveVoices += 1;
        if (MVoices[i].state == SAT_VOICE_PLAYING) num_playing += 1;
        if (MVoices[i].state == SAT_VOICE_RELEASED) num_released += 1;
      }
    }
    MNumPlayingVoices = num_playing;
    MNumReleasedVoices = num_released;

    // process active voices

    if (MNumActiveVoices > 0) {
      bool processed = false;
      // threaded..
      #ifdef SAT_VOICE_PROCESSOR_THREADED      
        if (MProcessThreaded) {
          #ifdef SAT_VOICE_PROCESSOR_CLAP_THREAD_POOL
            if (MClapThreadPool) processed = MClapThreadPool->request_exec(MClapHost,MNumActiveVoices);
          #else
            if (MThreadPool) processed = MThreadPool->request_exec(MNumActiveVoices);
          #endif
        }
      #endif
      // ..or manually
      if (!processed) {
        for (uint32_t i=0; i<MNumActiveVoices; i++) {
          uint32_t v = MActiveVoices[i];
          processVoice(v);
        }
      }
    }
  }

  //----------

  // void processStereoSample(sat_sample_t* spl0, sat_sample_t* spl1) override {
  // }

  // void processAudio(SAT_ProcessContext* AContext, uint32_t AOffset, uint32_t ALength) override {
  // }

  // void processAudio(SAT_ProcessContext* AContext, uint32_t AOffset) override {
  // }

//------------------------------
public:
//------------------------------

  /*
    similar to block processor..
    events and timing will be handled individually for each voice
    (note events split into individual voice queues, before processing)
  */

  void process(SAT_ProcessContext* AContext) override {
    MProcessContext = AContext;
    const clap_process_t* process = AContext->process;
    float** output = process->audio_outputs[0].data32;
    uint32_t length = process->frames_count;
    SAT_ClearStereoBuffer(output,length);
    processEvents(process->in_events,process->out_events);
    processAudio(AContext);
    mixActiveVoices(output,length);
    MProcessContext = nullptr;
  }

//------------------------------
public:
//------------------------------

  // called by host
  // separate thread for each task/voice

  void threadPoolExec(uint32_t AIndex) {
    //SAT_TRACE;
    uint32_t v = MActiveVoices[AIndex];
    processVoice(v);
  }
  
  //----------

  // assumes floats, voices = mono, and output = stereo

  void mixActiveVoices(float** AOutput, uint32_t ALength) {

    //float** output = MVoiceContext.process_context->process->audio_outputs[0].data32;// voice_buffer;
    //uint32_t blocksize = MVoiceContext.process_context->process->frames_count;// voice_length;

    for (uint32_t i=0; i<MNumActiveVoices; i++) {
      uint32_t voice = MActiveVoices[i];

      // calc voice buffer offset
      float* buffer = MVoiceBuffer;
      uint32_t index = MVoices[voice].index;
      buffer += (index * SAT_PLUGIN_MAX_BLOCK_SIZE);

      // and copy it to the output buffer
      SAT_AddMonoToStereoBuffer(AOutput,buffer,ALength);
    }
  }

  //----------

  #ifdef SAT_VOICE_PROCESSOR_THREADED
  
  void on_ThreadPoolListener_exec(uint32_t AIndex) override {
    //SAT_PRINT("task %i\n",AIndex);
    //printf("task %i\n",AIndex);
    uint32_t v = MActiveVoices[AIndex];
    processVoice(v);
  }

  #endif

//------------------------------
public:
//------------------------------

  void noteOnEvent(const clap_event_note_t* event) override {
    //SAT_PRINT("note on\n");
    //SAT_Print("note_id %i pck %i,%i,%i\n",event->note_id,event->port_index,event->channel,event->key);
    int32_t voice = findFreeVoice(true); // SAT_VOICE_PROCESSOR_STEAL_VOICES
    if (voice >= 0) {
      MVoices[voice].state        = SAT_VOICE_WAITING;
      MVoices[voice].note.port    = event->port_index;
      MVoices[voice].note.channel = event->channel;
      MVoices[voice].note.key     = event->key;
      MVoices[voice].note.noteid  = event->note_id;
      MVoices[voice].clearVoiceQueue();
      SAT_VoiceEvent ve = SAT_VoiceEvent(CLAP_EVENT_NOTE_ON, event->header.time, event->key, event->velocity);
      //MVoices[voice].events.write(ve);
      MVoices[voice].events.enqueue(ve);
    }
  }

  //----------

  void noteOffEvent(const clap_event_note_t* event) override {
    //SAT_PRINT("note off\n");
    //SAT_Print("note_id %i pck %i,%i,%i\n",event->note_id,event->port_index,event->channel,event->key);
    for (int32_t voice=0; voice<COUNT; voice++) {
      if (isActive(voice)) {
        if (isTargeted(voice,event->note_id,event->port_index,event->channel,event->key)) {
          SAT_VoiceEvent ve = SAT_VoiceEvent(CLAP_EVENT_NOTE_OFF, event->header.time, event->key, event->velocity);
          //MVoices[voice].events.write(ve);
          MVoices[voice].events.enqueue(ve);
        }
      }
    }
  }

  //----------

  void noteChokeEvent(const clap_event_note_t* event) override {
    //SAT_PRINT("note choke\n");
    //SAT_Print("note_id %i pck %i,%i,%i\n",event->note_id,event->port_index,event->channel,event->key);
    for (int32_t voice=0; voice<COUNT; voice++) {
      if (isActive(voice)) {
        if (isTargeted(voice,event->note_id,event->port_index,event->channel,event->key)) {
          SAT_VoiceEvent ve = SAT_VoiceEvent(CLAP_EVENT_NOTE_CHOKE, event->header.time, event->key, event->velocity);
          //MVoices[voice].events.write(ve);
          MVoices[voice].events.enqueue(ve);
        }
      }
    }
  }

  //----------

  void noteExpressionEvent(const clap_event_note_expression_t* event) override {
    //SAT_Print("note_id %i pck %i,%i,%i expr %i val %.3f\n",event->note_id,event->port_index,event->channel,event->key,event->expression_id,event->value);
    for (int32_t voice=0; voice<COUNT; voice++) {
      if (isActive(voice)) {
        if (isTargeted(voice,event->note_id,event->port_index,event->channel,event->key)) {
          SAT_VoiceEvent ve = SAT_VoiceEvent(CLAP_EVENT_NOTE_EXPRESSION, event->header.time, event->expression_id, event->value);
          //MVoices[voice].events.write(ve);
          MVoices[voice].events.enqueue(ve);
        }
      }
    }
  }

  //----------

  // parameters need to be sent to all voices, not just the currently active ones..

  void paramValueEvent(const clap_event_param_value_t* event) override {
    //SAT_PRINT("note_id %i pck %i,%i,%i param %i val %.3f\n",event->note_id,event->port_index,event->channel,event->key,event->param_id,event->value);
    for (int32_t voice=0; voice<COUNT; voice++) {
      //#ifdef SAT_VOICE_PROCESSOR_SEND_GLOBAL_PARAMS_TO_ALL_VOICES
      //#endif
      //if (isActive(voice)) {
        if (isTargeted(voice,event->note_id,event->port_index,event->channel,event->key)) {
          SAT_VoiceEvent ve = SAT_VoiceEvent(CLAP_EVENT_PARAM_VALUE, event->header.time, event->param_id, event->value);
          //MVoices[voice].events.write(ve);
          MVoices[voice].events.enqueue(ve);
        }
      //}
    }
  }

  //----------

  // modulations need to be sent to all voices, not just the currently active ones..
  // or?

  void paramModEvent(const clap_event_param_mod_t* event) override {
    //SAT_PRINT("note_id %i pck %i,%i,%i param %i amt %.3f\n",event->note_id,event->port_index,event->channel,event->key,event->param_id,event->amount);
    for (int32_t voice=0; voice<COUNT; voice++) {
      //#ifdef SAT_VOICE_PROCESSOR_SEND_GLOBAL_MODS_TO_ALL_VOICES
      //#endif
      //if (isActive(voice)) {
        if (isTargeted(voice,event->note_id,event->port_index,event->channel,event->key)) {
          SAT_VoiceEvent ve = SAT_VoiceEvent(CLAP_EVENT_PARAM_MOD, event->header.time, event->param_id, event->amount);
          //MVoices[voice].events.write(ve);
          MVoices[voice].events.enqueue(ve);
        }
      //}
    }
  }

  //----------

  void transportEvent(const clap_event_transport_t* event) override {
    //SAT_PRINT("transport\n");
  }

  //----------

  void midiEvent(const clap_event_midi_t* event) override {
    //SAT_PRINT("midi\n");
    // #ifdef SAT_VOICE_PROCESSOR_CONVERT_MIDI
    //   uint8_t msg   = event->data[0] & 0xf0;
    //   uint8_t chan  = event->data[0] & 0x0f;
    //   uint8_t index = event->data[1]; // & 0x7f;
    //   uint8_t val   = event->data[2]; // & 0x7f;
    //   switch (msg) {
    //     case SAT_MIDI_NOTE_OFF:
    //       SAT_Print("MIDI NOTE OFF chan %i index %i val %i\n",chan,index,val);
    //       //processNoteOff
    //       break;
    //     case SAT_MIDI_NOTE_ON:
    //       SAT_Print("MIDI NOTE ON chan %i index %i val %i\n",chan,index,val);
    //       //processNoteOn
    //       break;
    //     case SAT_MIDI_POLY_AFTERTOUCH:
    //       SAT_Print("MIDI POLY AFTERTOUCH chan %i index %i val %i\n",chan,index,val);
    //       //processNoteExpression
    //       break;
    //     case SAT_MIDI_CONTROL_CHANGE:
    //       SAT_Print("MIDI CONTROL CHANGE chan %i index %i val %i\n",chan,index,val);
    //       // 74 : processNoteExpression
    //       // midi mapping
    //       break;
    //     case SAT_MIDI_PROGRAM_CHANGE:
    //       SAT_Print("MIDI PROGRAM CHANGE chan %i index %i val %i\n",chan,index,val);
    //       // ?
    //       break;
    //     case SAT_MIDI_CHANNEL_AFTERTOUCH:
    //       SAT_Print("MIDI CHANNEL AFTERTOUCH chan %i index %i val %i\n",chan,index,val);
    //       //processNoteExpression
    //       break;
    //     case SAT_MIDI_PITCHBEND:
    //       SAT_Print("MIDI PITCHBEND chan %i index %i val %i\n",chan,index,val);
    //       //processNoteExpression
    //       break;
    //     case SAT_MIDI_SYS:
    //       SAT_Print("MIDI SYS chan %i index %i val %i\n",chan,index,val);
    //       //processMidiSysex
    //       break;
    //   }
    // #endif
  }

  //----------

  void midiSysexEvent(const clap_event_midi_sysex_t* event) override {
    //SAT_PRINT("sysex\n");
  }

  //----------

  void midi2Event(const clap_event_midi2_t* event) override {
    //SAT_PRINT("midi2\n");
  }

//------------------------------
public:
//------------------------------

  // void processEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) override {
  //   SAT_PRINT("\n");
  // }

  // void preProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) override {
  //   SAT_PRINT("\n");
  // }

  void postProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) override {
    //SAT_PRINT("\n");
     for (uint32_t i=0; i<COUNT; i++) {
       if (MVoices[i].state == SAT_VOICE_WAITING) {
         // still waiting, not started? something is wrong..
         MVoices[i].state = SAT_VOICE_OFF;
         queueNoteEnd(MVoices[i].note);      // ???
         //SAT_Print("voice %i -> OFF\n",i);
       }
       if (MVoices[i].state == SAT_VOICE_FINISHED) {
         MVoices[i].state = SAT_VOICE_OFF;
         // some voice cleanup here?
         queueNoteEnd(MVoices[i].note);
         //SAT_Print("voice %i -> OFF\n",i);
       }
     }
     flushNoteEnds(out_events);
  }

//------------------------------
private:
//------------------------------

  // if threaded: separate threads for each voice
  // else:        one voice after another

  void processVoice(uint32_t i) {
    MVoices[i].process();
  };

  //----------
  
  // void clearVoiceQueue(uint32_t i) {
  //   MVoices[i].clearVoiceQueue();
  // }
  
  //----------
  
  int32_t findFreeVoice(bool AReleased=false) {
    for (uint32_t i=0; i<COUNT; i++) {
      if ((MVoices[i].state == SAT_VOICE_OFF) /*|| (MVoices[i].state == SAT_VOICE_FINISHED)*/) {
        return i;
      }
    }
    if (AReleased) {
      int32_t lowest_index = -1;
      double  lowest_level = 666.0;
      for (uint32_t i=0; i<COUNT; i++) {
        if (MVoices[i].state == SAT_VOICE_RELEASED) {
          float env_level = MVoices[i].getVoiceLevel();
          if (env_level < lowest_level) {
            lowest_index = i;
            lowest_level = env_level;
          }
        }
      }
      if (lowest_index >= 0) {
        // kill released note..
        queueNoteEnd( MVoices[lowest_index].note );
        return lowest_index;
      }
    }
    return -1;
  }

  //----------

  void queueNoteEnd(SAT_Note ANote) {
    //MNoteEndQueue.write(ANote);
    MNoteEndQueue.enqueue(ANote);
  }

  //----------

  // called at end of postProcessEvents()
  // (end of process.. time should be blocksize-1 ?

  void flushNoteEnds(const clap_output_events_t* out_events) {
    uint32_t count = 0;
    SAT_Note note = {0};
    uint32_t blocksize = MVoiceContext.process_context->process->frames_count;
    //while (MNoteEndQueue.read(&note)) {
    while (MNoteEndQueue.try_dequeue(note)) {
      count += 1;
      clap_event_note_t note_event;
      note_event.header.flags     = 0;
      note_event.header.size      = sizeof(clap_event_note_t);
      note_event.header.space_id  = CLAP_CORE_EVENT_SPACE_ID;
      note_event.header.time      = blocksize - 1; //0;
      note_event.header.type      = CLAP_EVENT_NOTE_END;
      note_event.port_index       = note.port;
      note_event.channel          = note.channel;
      note_event.key              = note.key;
      note_event.note_id          = note.noteid;
      note_event.velocity         = 0.0;
      out_events->try_push(out_events,&note_event.header);
    }
    //if (count > 0) { SAT_Print("flushNoteEnds: %i events\n",count); }
  }
  
};

//----------------------------------------------------------------------
#endif
