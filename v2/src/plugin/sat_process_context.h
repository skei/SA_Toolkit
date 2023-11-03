#ifndef sat_process_context_included
#define sat_process_context_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/clap/sat_clap.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

struct SAT_ProcessContext {
  const clap_process_t* process;
  double                samplerate;
  uint32_t              counter;
};

//----------------------------------------------------------------------
#endif
