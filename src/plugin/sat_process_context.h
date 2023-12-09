#ifndef sat_process_context_included
#define sat_process_context_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/clap/sat_clap.h"

class SAT_Plugin;

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

  SAT_Plugin*           plugin;

//uint32_t              oversampling        = 1;        // oversampling for current block
//SAT_ParameterArray*   parameters          = {};         // plugin parameters

//bool                  has_tempo           = false;
//double                tempo               = 0.0;        // in bpm
//double                tempo_inc           = 0.0;        // tempo increment for each samples and until the next time info event
//bool                  has_beats_time      = false;
//clap_beattime         loop_start_beats    = 0;
//clap_beattime         loop_end_beats      = 0;
//clap_beattime         bar_start           = 0;          // start pos of the current bar
//int32_t               bar_number          = 0;          // bar at song pos 0 has the number 0
//bool                  has_seconds_time    = false;
//clap_sectime          song_pos_seconds    = 0;          // position in seconds
//clap_sectime          loop_start_seconds  = 0;
//clap_sectime          loop_end_seconds    = 0;

};

//----------------------------------------------------------------------
#endif
