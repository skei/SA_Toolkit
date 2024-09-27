#ifndef sat_synth_parameters_included
#define sat_synth_parameters_included
//----------------------------------------------------------------------
/*

  TODO:
  - mod input filter?
  - mod input src: post fx
*/
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/sat_plugin_base.h"
#include "plugin/sat_parameters.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

enum sa_synth_osc_type_e {
  SA_SYNTH_OSC_NONE = 0,
  SA_SYNTH_OSC_RING_MOD,
  SA_SYNTH_OSC_AMPL_MOD,
  SA_SYNTH_OSC_PHASE_MOD,
  SA_SYNTH_OSC_FREQ_MOD,
  SA_SYNTH_OSC_MAX
};

#define sa_synth_osc_type_count 6
const char* osc_type_text[sa_synth_osc_type_count] = {
  "none",
  "RingMod",
  "AmplMod",
  "PhaseMod",
  "FreqMod",
  "Max"
};

// enum sa_synth_res_type_e {
//   SA_SYNTH_OSC_NONE = 0,
//   SA_SYNTH_OSC_RING_MOD,
//   SA_SYNTH_OSC_AMPL_MOD,
//   SA_SYNTH_OSC_PHASE_MOD
// };

#define sa_synth_res_type_count 4
const char* res_type_text[sa_synth_res_type_count] = {
  "none",
  "plucked",
  "blown",
  "bowed"
};

#define sa_synth_flt_type_count 10
const char* flt_type_text[sa_synth_flt_type_count] = {
  "None",
  "LowPass",
  "BandPass",
  "HighPass",
  "Notch",
  "Peak",
  "AllPass",
  "Bell",
  "LowShelf",
  "HighShelf"
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

enum sa_synth_parameter_e {

  SA_SYNTH_PARAM_GLOBAL_GAIN = 0,

  SA_SYNTH_PARAM_OSC1_SQU,
  SA_SYNTH_PARAM_OSC1_TRI,
  SA_SYNTH_PARAM_OSC1_SIN,
  SA_SYNTH_PARAM_OSC1_WIDTH,
  SA_SYNTH_PARAM_OSC1_OCT,
  SA_SYNTH_PARAM_OSC1_SEMI,
  SA_SYNTH_PARAM_OSC1_CENT,
  SA_SYNTH_PARAM_OSC1_TYPE,
  SA_SYNTH_PARAM_OSC1_IN_AMOUNT,
  SA_SYNTH_PARAM_OSC1_IN_O1,
  SA_SYNTH_PARAM_OSC1_IN_O2,
  SA_SYNTH_PARAM_OSC1_IN_R1,
  SA_SYNTH_PARAM_OSC1_IN_R2,
  SA_SYNTH_PARAM_OSC1_IN_A,
  SA_SYNTH_PARAM_OSC1_IN_N,

  SA_SYNTH_PARAM_OSC2_SQU,
  SA_SYNTH_PARAM_OSC2_TRI,
  SA_SYNTH_PARAM_OSC2_SIN,
  SA_SYNTH_PARAM_OSC2_WIDTH,
  SA_SYNTH_PARAM_OSC2_OCT,
  SA_SYNTH_PARAM_OSC2_SEMI,
  SA_SYNTH_PARAM_OSC2_CENT,
  SA_SYNTH_PARAM_OSC2_TYPE,
  SA_SYNTH_PARAM_OSC2_IN_AMOUNT,
  SA_SYNTH_PARAM_OSC2_IN_O1,
  SA_SYNTH_PARAM_OSC2_IN_O2,
  SA_SYNTH_PARAM_OSC2_IN_R1,
  SA_SYNTH_PARAM_OSC2_IN_R2,
  SA_SYNTH_PARAM_OSC2_IN_A,
  SA_SYNTH_PARAM_OSC2_IN_N,

  SA_SYNTH_PARAM_RES1_IMPULSE,
  SA_SYNTH_PARAM_RES1_SHAPE,
  SA_SYNTH_PARAM_RES1_FEEDBACK,
  SA_SYNTH_PARAM_RES1_DAMPING,
  SA_SYNTH_PARAM_RES1_OCT,
  SA_SYNTH_PARAM_RES1_SEMI,
  SA_SYNTH_PARAM_RES1_CENT,
  SA_SYNTH_PARAM_RES1_TYPE,
  SA_SYNTH_PARAM_RES1_IN_AMOUNT,
  SA_SYNTH_PARAM_RES1_IN_O1,
  SA_SYNTH_PARAM_RES1_IN_O2,
  SA_SYNTH_PARAM_RES1_IN_R1,
  SA_SYNTH_PARAM_RES1_IN_R2,
  SA_SYNTH_PARAM_RES1_IN_A,
  SA_SYNTH_PARAM_RES1_IN_N,

  SA_SYNTH_PARAM_RES2_IMPULSE,
  SA_SYNTH_PARAM_RES2_SHAPE,
  SA_SYNTH_PARAM_RES2_FEEDBACK,
  SA_SYNTH_PARAM_RES2_DAMPING,
  SA_SYNTH_PARAM_RES2_OCT,
  SA_SYNTH_PARAM_RES2_SEMI,
  SA_SYNTH_PARAM_RES2_CENT,
  SA_SYNTH_PARAM_RES2_TYPE,
  SA_SYNTH_PARAM_RES2_IN_AMOUNT,
  SA_SYNTH_PARAM_RES2_IN_O1,
  SA_SYNTH_PARAM_RES2_IN_O2,
  SA_SYNTH_PARAM_RES2_IN_R1,
  SA_SYNTH_PARAM_RES2_IN_R2,
  SA_SYNTH_PARAM_RES2_IN_A,
  SA_SYNTH_PARAM_RES2_IN_N,

  SA_SYNTH_PARAM_MIX_O1,
  SA_SYNTH_PARAM_MIX_O2,
  SA_SYNTH_PARAM_MIX_R1,
  SA_SYNTH_PARAM_MIX_R2,

  SA_SYNTH_PARAM_FLT1_TYPE,
  SA_SYNTH_PARAM_FLT1_FREQ,
  SA_SYNTH_PARAM_FLT1_Q,
  SA_SYNTH_PARAM_FLT1_GAIN,

  SA_SYNTH_PARAM_ENV1_ATT,
  SA_SYNTH_PARAM_ENV1_DEC,
  SA_SYNTH_PARAM_ENV1_SUS,
  SA_SYNTH_PARAM_ENV1_REL,

  SA_SYNTH_PARAM_COUNT
};

//----------------------------------------------------------------------

#define A   CLAP_PARAM_IS_AUTOMATABLE
#define M   CLAP_PARAM_IS_MODULATABLE
#define N   CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID

#define AM  (A|M)
#define AMN (A|M|N)

#define S   CLAP_PARAM_IS_STEPPED
#define SE (CLAP_PARAM_IS_STEPPED | CLAP_PARAM_IS_ENUM )
#define H   CLAP_PARAM_IS_HIDDEN

//----------

//const
clap_param_info_t sa_synth_parameters[SA_SYNTH_PARAM_COUNT] = {

  { SA_SYNTH_PARAM_GLOBAL_GAIN,     AM,     nullptr,  "Gain",     "Global",   0.0, 1.0, 0.5 },
  
  { SA_SYNTH_PARAM_OSC1_SQU,        AMN,    nullptr,  "Squ",      "Osc1",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_OSC1_TRI,        AMN,    nullptr,  "Tri",      "Osc1",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_OSC1_SIN,        AMN,    nullptr,  "Sin",      "Osc1",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_OSC1_WIDTH,      AMN,    nullptr,  "Width",    "Osc1",     0.0, 1.0, 0.5 },
  { SA_SYNTH_PARAM_OSC1_OCT,        AMN|S,  nullptr,  "Oct",      "Osc1",    -4,   4,   0   },
  { SA_SYNTH_PARAM_OSC1_SEMI,       AMN|S,  nullptr,  "Semi",     "Osc1",   -12,  12,   0   },
  { SA_SYNTH_PARAM_OSC1_CENT,       AMN,    nullptr,  "Cent",     "Osc1",    -1.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_OSC1_TYPE,       AMN|SE, nullptr,  "Type",     "Osc1",     0,   sa_synth_osc_type_count - 1, 0 },
  { SA_SYNTH_PARAM_OSC1_IN_AMOUNT,  AMN,    nullptr,  "Amt",      "Osc1",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_OSC1_IN_O1,      AMN,    nullptr,  "O1",       "Osc1",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_OSC1_IN_O2,      AMN,    nullptr,  "O2",       "Osc1",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_OSC1_IN_R1,      AMN,    nullptr,  "R1",       "Osc1",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_OSC1_IN_R2,      AMN,    nullptr,  "R2",       "Osc1",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_OSC1_IN_A,       AMN,    nullptr,  "A",        "Osc1",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_OSC1_IN_N,       AMN,    nullptr,  "N",        "Osc1",     0.0, 1.0, 0.0 },
  
  { SA_SYNTH_PARAM_OSC2_SQU,        AMN,    nullptr,  "Squ",      "Osc2",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_OSC2_TRI,        AMN,    nullptr,  "Tri",      "Osc2",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_OSC2_SIN,        AMN,    nullptr,  "Sin",      "Osc2",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_OSC2_WIDTH,      AMN,    nullptr,  "Width",    "Osc2",     0.0, 1.0, 0.5 },
  { SA_SYNTH_PARAM_OSC2_OCT,        AMN|S,  nullptr,  "Oct",      "Osc2",    -4,   4,   0   },
  { SA_SYNTH_PARAM_OSC2_SEMI,       AMN|S,  nullptr,  "Semi",     "Osc2",   -12,  12,   0   },
  { SA_SYNTH_PARAM_OSC2_CENT,       AMN,    nullptr,  "Cent",     "Osc2",    -1.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_OSC2_TYPE,       AMN|SE, nullptr,  "Type",     "Osc2",     0,   sa_synth_osc_type_count - 1, 0 },
  { SA_SYNTH_PARAM_OSC2_IN_AMOUNT,  AMN,    nullptr,  "Amt",      "Osc2",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_OSC2_IN_O1,      AMN,    nullptr,  "O1",       "Osc2",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_OSC2_IN_O2,      AMN,    nullptr,  "O2",       "Osc2",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_OSC2_IN_R1,      AMN,    nullptr,  "R1",       "Osc2",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_OSC2_IN_R2,      AMN,    nullptr,  "R2",       "Osc2",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_OSC2_IN_A,       AMN,    nullptr,  "A",        "Osc2",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_OSC2_IN_N,       AMN,    nullptr,  "N",        "Osc2",     0.0, 1.0, 0.0 },

  { SA_SYNTH_PARAM_RES1_IMPULSE,    AMN,    nullptr,  "Impulse",  "Res1",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_RES1_SHAPE,      AMN,    nullptr,  "Shape",    "Res1",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_RES1_FEEDBACK,   AMN,    nullptr,  "Feedback", "Res1",     0.0, 1.0, 0.9 },
  { SA_SYNTH_PARAM_RES1_DAMPING,    AMN,    nullptr,  "Damping",  "Res1",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_RES1_OCT,        AMN|S,  nullptr,  "Oct",      "Res1",    -4,   4,   0   },
  { SA_SYNTH_PARAM_RES1_SEMI,       AMN|S,  nullptr,  "Semi",     "Res1",   -12,  12,   0   },
  { SA_SYNTH_PARAM_RES1_CENT,       AMN,    nullptr,  "Cent",     "Res1",    -1.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_RES1_TYPE,       AMN|SE, nullptr,  "Type",     "Res1",     0.0, sa_synth_res_type_count - 1, 0 },
  { SA_SYNTH_PARAM_RES1_IN_AMOUNT,  AMN,    nullptr,  "Amt",      "Res1",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_RES1_IN_O1,      AMN,    nullptr,  "O1",       "Res1",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_RES1_IN_O2,      AMN,    nullptr,  "O2",       "Res1",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_RES1_IN_R1,      AMN,    nullptr,  "R1",       "Res1",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_RES1_IN_R2,      AMN,    nullptr,  "R2",       "Res1",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_RES1_IN_A,       AMN,    nullptr,  "A",        "Res1",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_RES1_IN_N,       AMN,    nullptr,  "N",        "Res1",     0.0, 1.0, 0.0 },
  
  { SA_SYNTH_PARAM_RES2_IMPULSE,    AMN,    nullptr,  "Impulse",  "Res2",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_RES2_SHAPE,      AMN,    nullptr,  "Shape",    "Res2",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_RES2_FEEDBACK,   AMN,    nullptr,  "Feedback", "Res2",     0.0, 1.0, 0.9 },
  { SA_SYNTH_PARAM_RES2_DAMPING,    AMN,    nullptr,  "Damping",  "Res2",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_RES2_OCT,        AMN|S,  nullptr,  "Oct",      "Res2",    -4,   4,   0   },
  { SA_SYNTH_PARAM_RES2_SEMI,       AMN|S,  nullptr,  "Semi",     "Res2",   -12,  12,   0   },
  { SA_SYNTH_PARAM_RES2_CENT,       AMN,    nullptr,  "Cent",     "Res2",    -1.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_RES2_TYPE,       AMN|SE, nullptr,  "Type",     "Res2",     0, sa_synth_res_type_count - 1, 0 },
  { SA_SYNTH_PARAM_RES2_IN_AMOUNT,  AMN,    nullptr,  "Amt",      "Res2",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_RES2_IN_O1,      AMN,    nullptr,  "O1",       "Res2",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_RES2_IN_O2,      AMN,    nullptr,  "O2",       "Res2",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_RES2_IN_R1,      AMN,    nullptr,  "R1",       "Res2",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_RES2_IN_R2,      AMN,    nullptr,  "R2",       "Res2",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_RES2_IN_A,       AMN,    nullptr,  "A",        "Res2",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_RES2_IN_N,       AMN,    nullptr,  "N",        "Res2",     0.0, 1.0, 0.0 },

  { SA_SYNTH_PARAM_MIX_O1,          AMN,    nullptr,  "O1",       "Mix",      0.0, 1.0, 1.0 },
  { SA_SYNTH_PARAM_MIX_O2,          AMN,    nullptr,  "O2",       "Mix",      0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_MIX_R1,          AMN,    nullptr,  "R1",       "Mix",      0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_MIX_R2,          AMN,    nullptr,  "R2",       "Mix",      0.0, 1.0, 0.0 },
  
  { SA_SYNTH_PARAM_FLT1_TYPE,       AMN|SE, nullptr,  "Type",     "Flt1",     0, sa_synth_flt_type_count - 1, 1 },
  { SA_SYNTH_PARAM_FLT1_FREQ,       AMN,    nullptr,  "Freq",     "Flt1",     0.0, 1.0, 0.73 },
  { SA_SYNTH_PARAM_FLT1_Q,          AMN,    nullptr,  "Q",        "Flt1",     0.0, 1.0, 0.27 },
  { SA_SYNTH_PARAM_FLT1_GAIN,       AMN,    nullptr,  "Gain",     "Flt1",     0.0, 1.0, 0.5 },

  { SA_SYNTH_PARAM_ENV1_ATT,        A,      nullptr,  "Attack",   "Env1",     0.0, 1.0, 0.1 },
  { SA_SYNTH_PARAM_ENV1_DEC,        A,      nullptr,  "Decay",    "Env1",     0.0, 1.0, 0.0 },
  { SA_SYNTH_PARAM_ENV1_SUS,        A,      nullptr,  "Sustain",  "Env1",     0.0, 1.0, 1.0 },
  { SA_SYNTH_PARAM_ENV1_REL,        A,      nullptr,  "Release",  "Env1",     0.0, 1.0, 0.5 }
};

//----------

#undef A
#undef M
#undef N
#undef AM
#undef AMN
#undef S
#undef SE
#undef H

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

bool sa_synth_setup_parameters(SAT_Plugin* APlugin) {

  SAT_PRINT("%i parameters\n",SA_SYNTH_PARAM_COUNT);

  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_GLOBAL_GAIN] ));

  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_OSC1_SQU] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_OSC1_TRI] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_OSC1_SIN] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_OSC1_WIDTH] ));
  APlugin->appendParameter( new SAT_IntParameter( &sa_synth_parameters[SA_SYNTH_PARAM_OSC1_OCT] ));
  APlugin->appendParameter( new SAT_IntParameter( &sa_synth_parameters[SA_SYNTH_PARAM_OSC1_SEMI] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_OSC1_CENT] ));
  APlugin->appendParameter( new SAT_TextParameter(&sa_synth_parameters[SA_SYNTH_PARAM_OSC1_TYPE], osc_type_text));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_OSC1_IN_AMOUNT] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_OSC1_IN_O1] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_OSC1_IN_O2] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_OSC1_IN_R1] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_OSC1_IN_R2] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_OSC1_IN_A] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_OSC1_IN_N] ));

  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_OSC2_SQU] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_OSC2_TRI] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_OSC2_SIN] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_OSC2_WIDTH] ));
  APlugin->appendParameter( new SAT_IntParameter( &sa_synth_parameters[SA_SYNTH_PARAM_OSC2_OCT] ));
  APlugin->appendParameter( new SAT_IntParameter( &sa_synth_parameters[SA_SYNTH_PARAM_OSC2_SEMI] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_OSC2_CENT] ));
  APlugin->appendParameter( new SAT_TextParameter(&sa_synth_parameters[SA_SYNTH_PARAM_OSC2_TYPE], osc_type_text ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_OSC2_IN_AMOUNT] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_OSC2_IN_O1] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_OSC2_IN_O2] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_OSC2_IN_R1] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_OSC2_IN_R2] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_OSC2_IN_A] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_OSC2_IN_N] ));

  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_RES1_IMPULSE] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_RES1_SHAPE] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_RES1_FEEDBACK] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_RES1_DAMPING] ));
  APlugin->appendParameter( new SAT_IntParameter( &sa_synth_parameters[SA_SYNTH_PARAM_RES1_OCT] ));
  APlugin->appendParameter( new SAT_IntParameter( &sa_synth_parameters[SA_SYNTH_PARAM_RES1_SEMI] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_RES1_CENT] ));
  APlugin->appendParameter( new SAT_TextParameter(&sa_synth_parameters[SA_SYNTH_PARAM_RES1_TYPE], res_type_text ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_RES1_IN_AMOUNT] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_RES1_IN_O1] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_RES1_IN_O2] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_RES1_IN_R1] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_RES1_IN_R2] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_RES1_IN_A] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_RES1_IN_N] ));

  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_RES2_IMPULSE] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_RES2_SHAPE] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_RES2_FEEDBACK] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_RES2_DAMPING] ));
  APlugin->appendParameter( new SAT_IntParameter( &sa_synth_parameters[SA_SYNTH_PARAM_RES2_OCT] ));
  APlugin->appendParameter( new SAT_IntParameter( &sa_synth_parameters[SA_SYNTH_PARAM_RES2_SEMI] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_RES2_CENT] ));
  APlugin->appendParameter( new SAT_TextParameter(&sa_synth_parameters[SA_SYNTH_PARAM_RES2_TYPE], res_type_text ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_RES2_IN_AMOUNT] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_RES2_IN_O1] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_RES2_IN_O2] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_RES2_IN_R1] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_RES2_IN_R2] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_RES2_IN_A] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_RES2_IN_N] ));

  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_MIX_O1] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_MIX_O2] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_MIX_R1] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_MIX_R2] ));

  APlugin->appendParameter( new SAT_TextParameter(&sa_synth_parameters[SA_SYNTH_PARAM_FLT1_TYPE], flt_type_text ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_FLT1_FREQ] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_FLT1_Q] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_FLT1_GAIN] ));

  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_ENV1_ATT] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_ENV1_DEC] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_ENV1_SUS] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_synth_parameters[SA_SYNTH_PARAM_ENV1_REL] ));

  return true;
}

//----------------------------------------------------------------------
#endif