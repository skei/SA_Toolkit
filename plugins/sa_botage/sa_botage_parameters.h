#ifndef sa_botage_parameters_included
#define sa_botage_parameters_included
//----------------------------------------------------------------------

#include "base/sat.h"
#include "plugin/clap/sat_clap.h"

//----------------------------------------------------------------------

#if 0

enum sa_botage_parameters_e {
  
  // Global

  SA_BOTAGE_PARAM_GAIN = 0,

  // Buffer
  
  SA_BOTAGE_PARAM_BUFFER_BEATS,
  SA_BOTAGE_PARAM_BUFFER_SLICES,

  // Envelopes
  
//  SA_BOTAGE_PARAM_RANGE_ENV_ATTACK,
//  SA_BOTAGE_PARAM_RANGE_ENV_DECAY,
//  SA_BOTAGE_PARAM_SLICE_ENV_ATTACK,
//  SA_BOTAGE_PARAM_SLICE_ENV_DECAY,

  // Activate

//  SA_BOTAGE_PARAM_RANGE_PROB,
//  SA_BOTAGE_PARAM_RANGE_SLICES,
//  SA_BOTAGE_PARAM_RANGE_LOOPS,

  // Range probabilities

//  SA_BOTAGE_PARAM_PROB_RANGE_LENGTH,
//  SA_BOTAGE_PARAM_PROB_RANGE_LENGTH_MIN,
//  SA_BOTAGE_PARAM_PROB_RANGE_LENGTH_MIN,
//  SA_BOTAGE_PARAM_PROB_RANGE_SPEED,
//  SA_BOTAGE_PARAM_PROB_RANGE_SPEED_MIN,
//  SA_BOTAGE_PARAM_PROB_RANGE_SPEED_MIN,
//  SA_BOTAGE_PARAM_PROB_RANGE_OFFSET,
//  SA_BOTAGE_PARAM_PROB_RANGE_OFFSET_MIN,
//  SA_BOTAGE_PARAM_PROB_RANGE_OFFSET_MIN,
//  SA_BOTAGE_PARAM_PROB_RANGE_REVERSE,
//  //SA_BOTAGE_PARAM_PROB_RANGE_REVERSE_MIN,
//  //SA_BOTAGE_PARAM_PROB_RANGE_REVERSE_MIN,
//  SA_BOTAGE_PARAM_PROB_RANGE_FX,
//  SA_BOTAGE_PARAM_PROB_RANGE_FX_MIN,
//  SA_BOTAGE_PARAM_PROB_RANGE_FX_MIN,

  // Loop probabilities

//  SA_BOTAGE_PARAM_PROB_LOOP_LENGTH,
//  SA_BOTAGE_PARAM_PROB_LOOP_LENGTH_MIN,
//  SA_BOTAGE_PARAM_PROB_LOOP_LENGTH_MIN,
//  SA_BOTAGE_PARAM_PROB_LOOP_SPEED,
//  SA_BOTAGE_PARAM_PROB_LOOP_SPEED_MIN,
//  SA_BOTAGE_PARAM_PROB_LOOP_SPEED_MIN,
//  SA_BOTAGE_PARAM_PROB_LOOP_OFFSET,
//  SA_BOTAGE_PARAM_PROB_LOOP_OFFSET_MIN,
//  SA_BOTAGE_PARAM_PROB_LOOP_OFFSET_MIN,
//  SA_BOTAGE_PARAM_PROB_LOOP_REVERSE,
//  //SA_BOTAGE_PARAM_PROB_LOOP_REVERSE_MIN,
//  //SA_BOTAGE_PARAM_PROB_LOOP_REVERSE_MIN,
//  SA_BOTAGE_PARAM_PROB_LOOP_FX,
//  SA_BOTAGE_PARAM_PROB_LOOP_FX_MIN,
//  SA_BOTAGE_PARAM_PROB_LOOP_FX_MIN,

  SA_BOTAGE_PARAM_COUNT
};

//----------------------------------------------------------------------

//enum {
//   CLAP_PARAM_IS_STEPPED
//   CLAP_PARAM_IS_PERIODIC
//   CLAP_PARAM_IS_HIDDEN
//   CLAP_PARAM_IS_READONLY
//   CLAP_PARAM_IS_BYPASS
//   CLAP_PARAM_IS_AUTOMATABLE
//   CLAP_PARAM_IS_AUTOMATABLE_PER_NOTE_ID
//   CLAP_PARAM_IS_AUTOMATABLE_PER_KEY
//   CLAP_PARAM_IS_AUTOMATABLE_PER_CHANNEL
//   CLAP_PARAM_IS_AUTOMATABLE_PER_PORT
//   CLAP_PARAM_IS_MODULATABLE
//   CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID
//   CLAP_PARAM_IS_MODULATABLE_PER_KEY
//   CLAP_PARAM_IS_MODULATABLE_PER_CHANNEL
//   CLAP_PARAM_IS_MODULATABLE_PER_PORT
//   CLAP_PARAM_REQUIRES_PROCESS
//};
//typedef uint32_t clap_param_info_flags;

//struct clap_param_info {
//   clap_id id;
//   clap_param_info_flags flags;
//   void *cookie;
//   char name[CLAP_NAME_SIZE];
//   char module[CLAP_PATH_SIZE];
//   double min_value;
//   double max_value;
//   double default_value;
//};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//clap_param_info_t sa_botage_parameter_infos[SA_BOTAGE_PARAM_COUNT] = {
//  
//  // Global
//  
//  {
//    SA_BOTAGE_PARAM_GAIN,
//    CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_MODULATABLE,
//    nullptr,
//    "Gain",
//    "Global",
//    0.0,
//    1.0,
//    0.25
//  },
//  
//  // Buffer
//
//  {
//    SA_BOTAGE_PARAM_BUFFER_BEATS,
//    CLAP_PARAM_IS_STEPPED | CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_MODULATABLE,
//    nullptr,
//    "Beats",
//    "Buffer",
//    1,
//    8,
//    4
//  },
//
//  {
//    SA_BOTAGE_PARAM_BUFFER_SLICES,
//    CLAP_PARAM_IS_STEPPED | CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_MODULATABLE,
//    nullptr,
//    "Slices",
//    "Buffer",
//    1,
//    8,
//    4
//  }
//  
//};

#endif // 0

//----------------------------------------------------------------------
#endif

