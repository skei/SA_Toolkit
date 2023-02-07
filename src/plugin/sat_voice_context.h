#ifndef sat_voice_context_included
#define sat_voice_context_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "plugin/clap/sat_clap.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

struct SAT_VoiceContext {
  SAT_ProcessContext    process_context = nullptr;
  double                samplerate      = 0.0;
};

//----------------------------------------------------------------------
#endif

