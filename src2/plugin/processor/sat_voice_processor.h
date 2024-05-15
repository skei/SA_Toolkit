#ifndef sat_voice_processor_included
#define sat_voice_processor_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/sat_processor.h"
#include "plugin/processor/sat_process_context.h"
#include "plugin/processor/sat_processor_owner.h"

//----------------------------------------------------------------------

class SAT_VoiceProcessor
: public SAT_Processor {

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

  void process(SAT_ProcessContext* AContext) override {
  }

};

//----------------------------------------------------------------------
#endif
