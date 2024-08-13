#ifndef sat_block_processor_included
#define sat_block_processor_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/sat_processor.h"

//----------------------------------------------------------------------

class SAT_BlockProcessor
: public SAT_Processor {

//------------------------------
protected:
//------------------------------


//------------------------------
public:
//------------------------------

  SAT_BlockProcessor(SAT_ProcessorListener* AListener)
  : SAT_Processor(AListener) {
    //SAT_TRACE;
  }

  //----------

  virtual ~SAT_BlockProcessor() {
    //SAT_TRACE;
  }

//------------------------------
public:
//------------------------------

  void process(SAT_ProcessContext* AContext) override {
    //SAT_TRACE;
    MProcessContext = AContext;
    const clap_process_t* process = AContext->process;
    uint32_t length = process->frames_count;
    // #ifndef SAT_NO_GUI
    //   clearAutomationToGui();
    //   clearModulationToGui();
    // #endif
    processEvents(process->in_events,process->out_events);
    processAudio(AContext,0,length);
    // #ifndef SAT_NO_GUI
    //   queueAutomationToGui();
    //   queueModulationToGui();
    // #endif
    MProcessContext = nullptr;

  }

};

//----------------------------------------------------------------------
#endif