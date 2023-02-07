#ifndef sat_process_context_included
#define sat_process_context_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "plugin/clap/sat_clap.h"
//#include "plugin/sat_note_manager.h"
//#include "plugin/sat_parameter_manager.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

struct SAT_ProcessContext {
  const clap_process_t* process       = nullptr;  // process pointer, (process)
  double                samplerate    = 0.0;      // sampolerate (activate)
  uint32_t              minbufsize    = 0;        // minimum bolck size (activate)
  uint32_t              maxbufsize    = 0;        // maximum block size (activate)
//  SAT_ParameterArray*   parameters    = {};       // plugin parameters
//  uint32_t              counter       = 0;        // ever-increasing counter (each process call)
//  uint32_t              oversampling  = 1;        // oversampling for current block
//  float**               block_buffer  = nullptr;  // voice/output buffer
//  uint32_t              block_length  = 0;        // length
};

//----------------------------------------------------------------------
#endif
