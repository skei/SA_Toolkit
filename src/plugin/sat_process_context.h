#ifndef sat_process_context_included
#define sat_process_context_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "plugin/clap/sat_clap.h"
#include "plugin/sat_parameter.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

struct SAT_ProcessContext {

  const clap_process_t* process             = nullptr;    // process pointer, (process)
  double                samplerate          = 0.0;        // sampolerate (activate)
  uint32_t              minbufsize          = 0;          // minimum bolck size (activate)
  uint32_t              maxbufsize          = 0;          // maximum block size (activate)
  SAT_ParameterArray*   parameters          = {};         // plugin parameters
  uint32_t              counter             = 0;          // ever-increasing counter (each process call)

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


  //uint32_t              oversampling  = 1;        // oversampling for current block
  float**               voice_buffer  = nullptr;  // voice/output buffer
  uint32_t              voice_length  = 0;        // length


};

//----------------------------------------------------------------------
#endif
