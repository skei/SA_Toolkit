#ifndef sat_audio_processor_included
#define sat_audio_processor_included
//----------------------------------------------------------------------

/*
  keep track of 'age' of voices..
  - number of process frames since note on
  - count samples, or process blocks

*/

#include "base/sat.h"
#include "plugin/sat_parameter.h"
#include "plugin/sat_process_context.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class SAT_AudioProcessor {

//------------------------------
private:
//------------------------------


//------------------------------
public:
//------------------------------

  SAT_AudioProcessor(SAT_ParameterArray* AParameters, SAT_ProcessContext* AContext) {
  }

  //----------

  ~SAT_AudioProcessor() {
  }

//------------------------------
public:
//------------------------------

  virtual void handleEvents(const clap_input_events_t* in_events, const clap_output_events_t* out_events) {
  }

  //----------

  virtual void processAudio(SAT_ProcessContext* AContext) {
  }

};

//----------------------------------------------------------------------
#endif
