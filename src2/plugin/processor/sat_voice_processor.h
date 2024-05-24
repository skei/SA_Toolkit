#ifndef sat_voice_processor_included
#define sat_voice_processor_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/sat_processor.h"
#include "plugin/processor/sat_process_context.h"
#include "plugin/processor/sat_processor_owner.h"

//----------------------------------------------------------------------

template <class VOICE>
class SAT_VoiceProcessor
: public SAT_Processor {

//------------------------------
private:
//------------------------------

  VOICE MVoice = {};

//------------------------------
public:
//------------------------------

  SAT_VoiceProcessor(SAT_ProcessorOwner* AOwner)
  : SAT_Processor(AOwner) {
    //SAT_TRACE;
  }

  //----------

  virtual ~SAT_VoiceProcessor() {
    //SAT_TRACE;
  }

//------------------------------
public:
//------------------------------

  // void processStereoSample(sat_sample_t* spl0, sat_sample_t* spl1) override {
  // }

  // void processAudio(SAT_ProcessContext* AContext, uint32_t AOffset, uint32_t ALength) override {
  // }

  // void processAudio(SAT_ProcessContext* AContext, uint32_t AOffset) override {
  // }

//------------------------------
public:
//------------------------------

  // void process(SAT_ProcessContext* AContext) override {
  // }

//------------------------------
public:
//------------------------------

  // void noteOnEvent(const clap_event_note_t* event) override {
  // }

  // void noteOffEvent(const clap_event_note_t* event) override {
  // }

  // void noteChokeEvent(const clap_event_note_t* event) override {
  // }

  // void noteExpressionEvent(const clap_event_note_expression_t* event) override {
  // }

  // void paramValueEvent(const clap_event_param_value_t* event) override {
  // }

  // void paramModEvent(const clap_event_param_mod_t* event) override {
  // }

  // void transportEvent(const clap_event_transport_t* event) override {
  // }

  // void midiEvent(const clap_event_midi_t* event) override {
  // }

  // void midiSysexEvent(const clap_event_midi_sysex_t* event) override {
  // }

  // void midi2Event(const clap_event_midi2_t* event) override {
  // }

//------------------------------
public:
//------------------------------

  // void processEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) override {
  // }

  // void preProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) override {
  // }

  // void postProcessEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) override {
  // }

};

//----------------------------------------------------------------------
#endif
