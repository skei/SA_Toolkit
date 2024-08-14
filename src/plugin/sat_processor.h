#ifndef sat_processor_included
#define sat_processor_included
//----------------------------------------------------------------------

// todo:
// - output events

//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/sat_plugin_base.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/sat_audio_port.h"
#include "plugin/sat_note_port.h"
#include "plugin/sat_parameter.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_Processor {

//------------------------------
protected:
//------------------------------

  SAT_ProcessorListener*  MListener       = nullptr;
  SAT_AudioPortArray*     MAudioInputs    = nullptr;
  SAT_AudioPortArray*     MAudioOutputs   = nullptr;
  SAT_NotePortArray*      MNoteInputs     = nullptr;
  SAT_NotePortArray*      MNoteOutputs    = nullptr;
  SAT_ParameterArray*     MParameters     = nullptr;
  SAT_ProcessContext*     MProcessContext = nullptr; // valud during process() calls

//------------------------------
public:
//------------------------------

  SAT_Processor(SAT_ProcessorListener* AListener) {
    //SAT_TRACE;
    MListener = AListener;
    MAudioInputs = AListener->on_processorListener_getAudioInputPorts();
    MAudioOutputs = AListener->on_processorListener_getAudioOutputPorts();
    MNoteInputs = AListener->on_processorListener_getNoteInputPorts();
    MNoteOutputs = AListener->on_processorListener_getNoteOutputPorts();
    MParameters = AListener->on_processorListener_getParameters();
    //SAT_Assert(MParameters);
  }

  //----------

  virtual ~SAT_Processor() {
    //SAT_TRACE;
  }

//------------------------------
public:
//------------------------------

  SAT_ProcessorListener*  getProcessorListener()              { return MListener; }
  SAT_ProcessContext*     getProcessContext()                 { return MProcessContext; }
  SAT_AudioPortArray*     getAudioInputs()                    { return MAudioInputs; }
  SAT_AudioPortArray*     getAudioOutputs()                   { return MAudioOutputs; }
  SAT_NotePortArray*      getNoteInputs()                     { return MNoteInputs; }
  SAT_NotePortArray*      getNoteOutputs()                    { return MNoteOutputs; }
  SAT_ParameterArray*     getParameters()                     { return MParameters; }
  uint32_t                getNumParameters()                  { return MParameters->size(); }
  SAT_Parameter*          getParameter(uint32_t AIndex)       { return MParameters->getItem(AIndex); }
  sat_param_t             getParameterValue(uint32_t AIndex)  { return MParameters->getItem(AIndex)->getValue(); }

//------------------------------
public: // events
//------------------------------

  virtual void noteOnEvent(const clap_event_note_t* event) {}
  virtual void noteOffEvent(const clap_event_note_t* event) {}
  virtual void noteChokeEvent(const clap_event_note_t* event) {}
  virtual void noteExpressionEvent(const clap_event_note_expression_t* event) {}
  virtual void paramValueEvent(const clap_event_param_value_t* event) {}
  virtual void paramModEvent(const clap_event_param_mod_t* event) {}
  virtual void transportEvent(const clap_event_transport_t* event) {}
  virtual void midiEvent(const clap_event_midi_t* event) {}
  virtual void midiSysexEvent(const clap_event_midi_sysex_t* event) {}
  virtual void midi2Event(const clap_event_midi2_t* event) {}

//------------------------------
public:
//------------------------------

  virtual void processEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
    //SAT_PRINT("in_events: %p out_events: %p\n",in_events,out_events);
    if (!in_events) return;
    // #ifndef SAT_NO_GUI
    //   clearAutomationToGui();
    //   clearModulationToGui();
    // #endif
    uint32_t prev_time = 0;
    uint32_t size = in_events->size(in_events);
    //if (size > 0) { SAT_PRINT("%i events\n",size); }
    for (uint32_t i=0; i<size; i++) {
      const clap_event_header_t* header = in_events->get(in_events,i);
      if (header->space_id == CLAP_CORE_EVENT_SPACE_ID) {
        if (header->time < prev_time) {
          SAT_PRINT("not sorted? prev_time %i header->time %i header->type %i\n",prev_time,header->time,header->type);
        }
        processEvent(header);
        prev_time = header->time;
      }
    }
    // #if !defined (SAT_NO_GUI)
    //   queueAutomationToGui();
    //   queueModulationToGui();
    // #endif
  }

  //----------

  virtual void preProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
  }

  //----------

  virtual void postProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
  }

  //----------

  void processEvent(const clap_event_header_t* event) {
    switch(event->type) {
      case CLAP_EVENT_NOTE_ON:              processNoteOnEvent((const clap_event_note_t*)event); break;
      case CLAP_EVENT_NOTE_OFF:             processNoteOffEvent((const clap_event_note_t*)event); break;
      case CLAP_EVENT_NOTE_CHOKE:           processNoteChokeEvent((const clap_event_note_t*)event); break;
      case CLAP_EVENT_NOTE_EXPRESSION:      processNoteExpressionEvent((const clap_event_note_expression_t*)event); break;
      case CLAP_EVENT_PARAM_VALUE:          processParamValueEvent((const clap_event_param_value_t*)event); break;
      case CLAP_EVENT_PARAM_MOD:            processParamModEvent((const clap_event_param_mod_t*)event); break;
      case CLAP_EVENT_TRANSPORT:            processTransportEvent((const clap_event_transport_t*)event); break;
      case CLAP_EVENT_MIDI:                 processMidiEvent((const clap_event_midi_t*)event); break;
      case CLAP_EVENT_MIDI_SYSEX:           processMidiSysexEvent((const clap_event_midi_sysex_t*)event); break;
      case CLAP_EVENT_MIDI2:                processMidi2Event((const clap_event_midi2_t*)event); break;
    }
  }

  //----------

  void processNoteOnEvent(const clap_event_note_t* event) {
    //SAT_PRINT("\n");
    SAT_Assert(event);
    noteOnEvent(event);
  }

  //----------

  void processNoteOffEvent(const clap_event_note_t* event) {
    //SAT_PRINT("\n");
    SAT_Assert(event);
    noteOffEvent(event);
  }

  //----------

  void processNoteChokeEvent(const clap_event_note_t* event) {
    //SAT_PRINT("\n");
    SAT_Assert(event);
    noteChokeEvent(event);
  }

  //----------

  void processNoteExpressionEvent(const clap_event_note_expression_t* event) {
    //SAT_PRINT("\n");
    SAT_Assert(event);
    noteExpressionEvent(event);
  }

  //----------

  void processParamValueEvent(const clap_event_param_value_t* event) {
    SAT_Assert(event);
    uint32_t index = event->param_id;
    double value = event->value;
    //SAT_PRINT("index %i value %.3f\n",index,value);
    SAT_Parameter* param = MParameters->getItem(index);
    param->setValue(value);
    #ifndef SAT_NO_GUI
      // MListener->on_processorListener_queueParamFromHostToGui(index,value);
      MListener->on_processorListener_updateParamFromHostToGui(index,value);
      // param->setGuiAutomationDirty(true);
      // param->setLastAutomatedValue(value);
    #endif
    paramValueEvent(event);
  }

  //----------

  void processParamModEvent(const clap_event_param_mod_t* event) {
    SAT_Assert(event);
    uint32_t index = event->param_id;
    double value = event->amount;
    //SAT_PRINT("index %i value %.3f\n",index,value);
    SAT_Parameter* param = MParameters->getItem(index);
    param->setModulation(value);
    #ifndef SAT_NO_GUI
      MListener->on_processorListener_updateModFromHostToGui(index,value);
      // MListener->on_processorListener_queueModFromHostToGui(index,value);
      // param->setGuiModulationDirty(true);
      // param->setLastModulatedValue(value);
    #endif
    paramModEvent(event);
  }

  //----------

  void processTransportEvent(const clap_event_transport_t* event) {
    SAT_Assert(event);
    if (event->flags & CLAP_TRANSPORT_HAS_TEMPO) {
      //MProcessContext.has_tempo = true
      //MProcessContext.tempo     = event->tempo;             // in bpm
      //MProcessContext.tempo_inc = event->tempo_inc;         // tempo increment for each samples and until the next time info event
    }
    if (event->flags & CLAP_TRANSPORT_HAS_BEATS_TIMELINE) {
      //MProcessContext.has_beats = true
      //MProcessContext. = clap_beattime song_pos_beats;      // position in beats
      //clap_beattime loop_start_beats;
      //clap_beattime loop_end_beats;
      //clap_beattime bar_start;                              // start pos of the current bar
      //int32_t       bar_number;                             // bar at song pos 0 has the number 0
    }
    if (event->flags & CLAP_TRANSPORT_HAS_SECONDS_TIMELINE) {
      //MProcessContext.has_seconds = true
      //clap_sectime  song_pos_seconds;                       // position in seconds
      //clap_sectime  loop_start_seconds;
      //clap_sectime  loop_end_seconds;
    }
    if (event->flags & CLAP_TRANSPORT_HAS_TIME_SIGNATURE) {
      //MProcessContext.has_timesig = true
      //uint16_t      tsig_num;                               // time signature numerator
      //uint16_t      tsig_denom;                             // time signature denominator
    }
    if (event->flags & CLAP_TRANSPORT_IS_PLAYING) {}
    if (event->flags & CLAP_TRANSPORT_IS_RECORDING) {}
    if (event->flags & CLAP_TRANSPORT_IS_LOOP_ACTIVE) {}
    if (event->flags & CLAP_TRANSPORT_IS_WITHIN_PRE_ROLL) {}
    transportEvent(event);
  }

  //----------

  void processMidiEvent(const clap_event_midi_t* event) {
    SAT_Assert(event);
    //SAT_PRINT("\n");
    // #ifdef SAT_PLUGIN_CONVERT_MIDI_TO_CLAP
    //   uint8_t msg0 = event->data[0];
    //   //uint8_t index = event->data[1];
    //   //uint8_t value = event->data[2];
    //   uint32_t msg  = msg0 & 0xf0;
    //   //uint32_t chan = msg0 & 0x0f;
    //   switch (msg) {
    //     case SAT_MIDI_NOTE_OFF: {
    //       //SAT_Print("MIDI NOTE OFF. chan %i key %i vel %i\n",chan,data2,data3);
    //       //clap_event_note_t note_event;
    //       //note_event.header.size     = sizeof(clap_event_note_t);
    //       //note_event.header.time     = event.header.time;
    //       //note_event.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
    //       //note_event.header.type     = CLAP_EVENT_NOTE_OFF;
    //       //note_event.header.flags    = 0; // CLAP_EVENT_IS_LIVE, CLAP_EVENT_DONT_RECORD
    //       //note_event.note_id         = (chan * 128) + index;
    //       //note_event.port_index      = event->port_index;
    //       //note_event.channel         = chan;
    //       //note_event.key             = index;
    //       //note_event.velocity        = value * (double)SAT_INV127;
    //       //const clap_event_header_t* header = (const clap_event_header_t*)&note_event;
    //       break;
    //     }
    //     case SAT_MIDI_NOTE_ON: {
    //       //SAT_Print("MIDI NOTE ON. chan %i key %i vel %i\n",chan,data2,data3);
    //       //clap_event_note_t note_event;
    //       //note_event.header.size     = sizeof(clap_event_note_t);
    //       //note_event.header.time     = event.header.time;
    //       //note_event.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
    //       //note_event.header.type     = CLAP_EVENT_NOTE_ON;
    //       //note_event.header.flags    = 0; // CLAP_EVENT_IS_LIVE, CLAP_EVENT_DONT_RECORD
    //       //note_event.note_id         = (chan * 128) + index;
    //       //note_event.port_index      = event->port_index;
    //       //note_event.channel         = chan;
    //       //note_event.key             = index;
    //       //note_event.velocity        = value * (double)SAT_INV127;
    //       //const clap_event_header_t* header = (const clap_event_header_t*)&note_event;
    //       break;
    //     }
    //     case SAT_MIDI_POLY_AFTERTOUCH: {
    //       //SAT_Print("MIDI POLY AFTERTOUCH. chan %i data1 %i data2 %i\n",chan,data2,data3);
    //       // pressure note expression
    //       break;
    //     }
    //     case SAT_MIDI_CONTROL_CHANGE: {
    //       //SAT_Print("MIDI CONTROL_CHANGE. chan %i index %i val %i\n",chan,data2,data3);
    //       // cc74: brightness
    //       break;
    //     }
    //     case SAT_MIDI_PROGRAM_CHANGE: {
    //       //SAT_Print("MIDI PROGRAM CHANGE. chan %i data1 %i data2 %i\n",chan,data2,data3);
    //       break;
    //     }
    //     case SAT_MIDI_CHANNEL_AFTERTOUCH: {
    //       //SAT_Print("MIDI CHANNEL AFTERTOUCH. chan %i data1 %i data2 %i\n",chan,data2,data3);
    //       // pressure note expression
    //       break;
    //     }
    //     case SAT_MIDI_PITCHBEND: {
    //       //SAT_Print("MIDI PITCH BEND. chan %i data1 %i data2 %i\n",chan,data2,data3);
    //       // tuning note expression
    //       break;
    //     }
    //     case SAT_MIDI_SYS: {
    //       //SAT_Print("MIDI SYS. chan %i data1 %i data2 %i\n",chan,data2,data3);
    //       break;
    //     }
    //   } // switch msg
    //   return true;
    // #endif
    midiEvent(event);
  }

  //----------

  void processMidiSysexEvent(const clap_event_midi_sysex_t* event) {
    //SAT_PRINT("\n");
    SAT_Assert(event);
    midiSysexEvent(event);
  }

  //----------

  void processMidi2Event(const clap_event_midi2_t* event) {
    //SAT_PRINT("\n");
    SAT_Assert(event);
    midi2Event(event);
  }

//------------------------------
public: // audio
//------------------------------

  // virtual void processSample() {
  // }

  //----------

  // spl0/1 might be 0 if no input/output ports!

  virtual void processStereoSample(sat_sample_t* spl0, sat_sample_t* spl1) {
  }

//------------------------------
public:
//------------------------------

  // note: stereo only
  // advice: override this with your own..

  virtual void processAudio(SAT_ProcessContext* AContext, uint32_t AOffset, uint32_t ALength) {
    //MProcessContext = AContext;
    const clap_process_t* process = AContext->process;
    bool have_audio_inputs = (process->audio_inputs_count > 0);
    bool have_audio_outputs = (process->audio_outputs_count > 0);
    bool have_audio_ports = have_audio_inputs | have_audio_outputs;
    if (have_audio_outputs) {
      // inputs & outputs
      if (have_audio_inputs) {
        float* input0  = process->audio_inputs[0].data32[0]  + AOffset;
        float* input1  = process->audio_inputs[0].data32[1]  + AOffset;
        float* output0 = process->audio_outputs[0].data32[0] + AOffset;
        float* output1 = process->audio_outputs[0].data32[1] + AOffset;
        for (uint32_t i=0; i<ALength; i++) {
          float spl0 = *input0++;
          float spl1 = *input1++;
          processStereoSample(&spl0,&spl1);
          *output0++ = spl0;
          *output1++ = spl1;
        }
      }
      // outputs only
      else {
        float* output0 = process->audio_outputs[0].data32[0] + AOffset;
        float* output1 = process->audio_outputs[0].data32[1] + AOffset;
        for (uint32_t i=0; i<ALength; i++) {
          float spl0 = 0.0;
          float spl1 = 0.0;
          processStereoSample(&spl0,&spl1);
          *output0++ = spl0;
          *output1++ = spl1;
        }
      }
    }
    else {
      // inputs only
      if (have_audio_inputs) {
        float* input0 = process->audio_inputs[0].data32[0] + AOffset;
        float* input1 = process->audio_inputs[0].data32[1] + AOffset;
        for (uint32_t i=0; i<ALength; i++) {
          float spl0 = *input0++;
          float spl1 = *input1++;
          processStereoSample(&spl0,&spl1);
        }
      }
      // no audio ports (note effect?)
      else {
        for (uint32_t i=0; i<ALength; i++) {
          processStereoSample(nullptr,nullptr);
        }
      }
    } // no outputs
    //MProcessContext = nullptr;
  }

  //----------

  // process SAT_AUDIO_QUANTIZED_SIZE samples, from AOffset

  virtual void processAudio(SAT_ProcessContext* AContext, uint32_t AOffset) {
    processAudio(AContext,AOffset,SAT_AUDIO_QUANTIZED_SIZE);
  }

//------------------------------
public:
//------------------------------

  virtual void process(SAT_ProcessContext* AContext) {
    MProcessContext = AContext;
    // default: do nothing
    // (no audio/event pssthrough)
    MProcessContext = nullptr;
  }

//------------------------------
public: // gui dirty queues
//------------------------------

  /*
    this is a bit hacky..
    to avoid sending the same events multiple times (modulation)..
    we go through all parameters, 4 times per block (automation and modulation, in and out)
    or sub-block for interleaved/quantized..

  */

  #if !defined (SAT_NO_GUI)

    // call before processing all events
    
    void clearAutomationToGui() {
      // uint32_t num = MParameters->size();
      // for (uint32_t i=0; i<num; i++) {
      //   SAT_Parameter* param = MParameters->getItem(i);
      //   param->setGuiAutomationDirty(false);
      //   //param->setLastAutomatedValue(0.0);
      // }
    }
    
    void clearModulationToGui() {
      // uint32_t num = MParameters->size();
      // for (uint32_t i=0; i<num; i++) {
      //   SAT_Parameter* param = MParameters->getItem(i);
      //   param->setGuiModulationDirty(false);
      //   //param->setLastModulatedValue(0.0);
      // }
    }
    
    //----------
      
    // call after processing all events
    // lastAutomatedValue set in processParamValueEvent
    // ParamFromHostToGui queue flushed at the end of process()
    // should this be flushed in (start of) gui repaint?
    
    void queueAutomationToGui() {
      // uint32_t num = MParameters->size();
      // for (uint32_t i=0; i<num; i++) {
      //   SAT_Parameter* param = MParameters->getItem(i);
      //   if (param->isGuiAutomationDirty()) {
      //     double value = param->getLastAutomatedValue();
      //     MListener->on_processorListener_queueParamFromHostToGui(i,value);
      //   }
      // }
    }

    // same here..

    void queueModulationToGui() {
      // uint32_t num = MParameters->size();
      // for (uint32_t i=0; i<num; i++) {
      //   SAT_Parameter* param = MParameters->getItem(i);
      //   if (param->isGuiModulationDirty()) {
      //     double value = param->getLastModulatedValue();
      //     MListener->on_processorListener_queueModFromHostToGui(i,value);
      //   }
      // }
    }

  #endif // nogui

};

//----------------------------------------------------------------------
#endif