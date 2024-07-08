#ifndef sat_interleaved_processor_included
#define sat_interleaved_processor_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/sat_processor.h"

//----------------------------------------------------------------------

class SAT_InterleavedProcessor
: public SAT_Processor {

//------------------------------
private:
//------------------------------



//------------------------------
public:
//------------------------------

  SAT_InterleavedProcessor(SAT_ProcessorOwner* AOwner)
  : SAT_Processor(AOwner) {
    //SAT_TRACE;
  }

  //----------

  virtual ~SAT_InterleavedProcessor() {
    //SAT_TRACE;
  }

//------------------------------
public:
//------------------------------

  void process(SAT_ProcessContext* AContext) override {
    MProcessContext = AContext;
    const clap_input_events_t* in_events = AContext->process->in_events;
    uint32_t remaining = AContext->process->frames_count;
    uint32_t num_events = in_events->size(in_events);
    uint32_t current_time = 0;
    uint32_t current_event = 0;
    //#ifndef SAT_NO_GUI
    //  clearAutomationToGui();
    //  clearModulationToGui();
    //#endif
    while (remaining > 0) {
      if (current_event < num_events) {
        const clap_event_header_t* header = in_events->get(in_events,current_event);
        current_event += 1;
        int32_t length = header->time - current_time;
        // if length > remaining ...
        //while (length > 0) {
        if (length > 0) {
          processAudio(AContext,current_time,length);
          remaining -= length;    // -= 32;
          current_time += length; // -= 32;
        }
        //processEventInterleaved(header);
        processEvent(header);
      }
      else { // no more events
        int32_t length = remaining;
        processAudio(AContext,current_time,length);
        remaining -= length;
        current_time += length;
      }
    }
    //SAT_Assert( events.read(&event) == false );
    //#ifndef SAT_NO_GUI
    //  queueAutomationToGui();
    //  queueModulationToGui();
    //#endif
    MProcessContext = nullptr;
  }

};

//----------------------------------------------------------------------
#endif