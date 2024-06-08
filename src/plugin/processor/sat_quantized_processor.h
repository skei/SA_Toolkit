#ifndef sat_quantized_processor_included
#define sat_quantized_processor_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/sat_processor.h"

//----------------------------------------------------------------------

class SAT_QuantizedProcessor
: public SAT_Processor {

//------------------------------
private:
//------------------------------



//------------------------------
public:
//------------------------------

  SAT_QuantizedProcessor(SAT_ProcessorOwner* AOwner)
  : SAT_Processor(AOwner) {
    //SAT_TRACE;
  }

  //----------

  virtual ~SAT_QuantizedProcessor() {
    //SAT_TRACE;
  }

//------------------------------
public:
//------------------------------

  void process(SAT_ProcessContext* AContext) override {
    uint32_t buffer_length = AContext->process->frames_count;
    uint32_t remaining = buffer_length;
    uint32_t current_time = 0;
    uint32_t current_event = 0;
    uint32_t next_event_time = 0;
    //#ifndef SAT_NO_GUI
    //  clearAutomationToGui();
    //  clearModulationToGui();
    //#endif
    const clap_input_events_t* in_events = AContext->process->in_events;
    uint32_t num_events = in_events->size(in_events);
    if (num_events > 0) {
      const clap_event_header_t* header = in_events->get(in_events,current_event);
      current_event += 1;
      next_event_time = header->time;
      do {
        // process events for next slice
        while (next_event_time < (current_time + SAT_AUDIO_QUANTIZED_SIZE)) {
          processEvent(header);
          if (current_event < num_events) {
            header = in_events->get(in_events,current_event);
            // if (header)
            current_event += 1;
            next_event_time = header->time;
          }
          else {
            next_event_time = buffer_length; // ???
          }
        }
        // process next slice
        if (remaining < SAT_AUDIO_QUANTIZED_SIZE) {
          processAudio(AContext,current_time,remaining);
          current_time += remaining;
          remaining = 0;
        }
        else {
          processAudio(AContext,current_time);
          current_time += SAT_AUDIO_QUANTIZED_SIZE;
          remaining -= SAT_AUDIO_QUANTIZED_SIZE;
        }
      } while (remaining > 0);
    }
    else { // no events..
      do {
        if (remaining < SAT_AUDIO_QUANTIZED_SIZE) processAudio(AContext,current_time,remaining);
        else processAudio(AContext,current_time);
        current_time += SAT_AUDIO_QUANTIZED_SIZE;
        remaining -= SAT_AUDIO_QUANTIZED_SIZE;
      } while (remaining > 0);
    }
    //#ifndef SAT_NO_GUI
    //  queueAutomationToGui();
    //  queueModulationToGui();
    //#endif
  }

};

//----------------------------------------------------------------------
#endif