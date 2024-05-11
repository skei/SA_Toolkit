#ifndef sat_process_context_included
#define sat_process_context_included
//----------------------------------------------------------------------

#include "plugin/lib/sat_clap.h"
#include "plugin/sat_parameter.h"

//

struct SAT_ProcessContext {
  const clap_process_t* process         = nullptr;
  SAT_ParameterArray*   parameters      = nullptr;
  double                samplerate      = 0.0;
  uint32_t              minframescount  = 0;
  uint32_t              maxframescount  = 0;
  uint32_t              process_counter = 0;
  uint32_t              sample_counter  = 0;
};

//----------------------------------------------------------------------
#endif