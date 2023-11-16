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
  uint32_t              minbufsize;
  uint32_t              maxbufsize;
  uint32_t              process_counter;
  uint64_t              sample_counter;
  uint32_t              voice_length;
  float**               voice_buffer;
};

//----------------------------------------------------------------------
#endif
