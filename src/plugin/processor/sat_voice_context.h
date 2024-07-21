#ifndef sat_voice_context_included
#define sat_voice_context_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/sat_plugin.h"
#include "plugin/processor/sat_process_context.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

struct SAT_VoiceContext {
  SAT_ProcessContext*   process_context   = nullptr;  // only valid during process()
  double                sample_rate       = 0.0;
  uint32_t              block_length      = 0.0;
  sat_sample_t*         voice_buffer      = nullptr;
//sat_sample_t*         block_buffer      = nullptr;
//uint32_t              min_frames_count  = 0;
//uint32_t              max_frames_count  = 0;
//SAT_Plugin*           plugin            = nullptr;
};

//----------------------------------------------------------------------
#endif

