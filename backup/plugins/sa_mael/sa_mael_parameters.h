#ifndef sa_mael_parameters_included
#define sa_mael_parameters_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/clap/sat_clap.h"

//----------------------------------------------------------------------

enum sa_mael_parameter_e {

  SA_MAEL_PARAM_GAIN = 0,
  //SA_MAEL_PARAM_TUNING,

  SA_MAEL_PARAM_OSC1_SQU,
  SA_MAEL_PARAM_OSC1_TRI,
  SA_MAEL_PARAM_OSC1_SIN,
  SA_MAEL_PARAM_OSC1_WIDTH,
  SA_MAEL_PARAM_OSC1_OCT,
  SA_MAEL_PARAM_OSC1_SEMI,
  SA_MAEL_PARAM_OSC1_CENT,
  
  SA_MAEL_PARAM_OSC2_SQU,
  SA_MAEL_PARAM_OSC2_TRI,
  SA_MAEL_PARAM_OSC2_SIN,
  SA_MAEL_PARAM_OSC2_WIDTH,
  SA_MAEL_PARAM_OSC2_OCT,
  SA_MAEL_PARAM_OSC2_SEMI,
  SA_MAEL_PARAM_OSC2_CENT,

  SA_MAEL_PARAM_MIX_TYPE,
  SA_MAEL_PARAM_MIX_AMOUNT,

  // SA_MAEL_PARAM_WSH_TYPE,
  // SA_MAEL_PARAM_WSH_AMOUNT,

  // SA_MAEL_PARAM_PSH_TYPE,
  // SA_MAEL_PARAM_PSH_AMOUNT,

  SA_MAEL_PARAM_FLT1_TYPE,
  SA_MAEL_PARAM_FLT1_FREQ,
  SA_MAEL_PARAM_FLT1_Q,

  SA_MAEL_PARAM_ENV1_ATT,
  SA_MAEL_PARAM_ENV1_DEC,
  SA_MAEL_PARAM_ENV1_SUS,
  SA_MAEL_PARAM_ENV1_REL,

  SA_MAEL_PARAM_COUNT
};

//----------------------------------------------------------------------

#define A   CLAP_PARAM_IS_AUTOMATABLE
#define M   CLAP_PARAM_IS_MODULATABLE
#define MN (CLAP_PARAM_IS_MODULATABLE | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID)
#define S   CLAP_PARAM_IS_STEPPED
#define SE (CLAP_PARAM_IS_STEPPED | CLAP_PARAM_IS_ENUM )
#define H   CLAP_PARAM_IS_HIDDEN

//----------

//const
clap_param_info_t sa_mael_Parameters[SA_MAEL_PARAM_COUNT] = {
  { SA_MAEL_PARAM_GAIN,           A,    nullptr, "Gain",      "Global",   0.0,  1.0, 0.5    },

  { SA_MAEL_PARAM_OSC1_SQU,       A|MN, nullptr, "Squ",       "Osc1",     0.0,  1.0, 0.0    },
  { SA_MAEL_PARAM_OSC1_TRI,       A|MN, nullptr, "Tri",       "Osc1",     0.0,  1.0, 0.0    },
  { SA_MAEL_PARAM_OSC1_SIN,       A|MN, nullptr, "Sin",       "Osc1",     0.0,  1.0, 0.0    },
  { SA_MAEL_PARAM_OSC1_WIDTH,     A|MN, nullptr, "Width",     "Osc1",     0.0,  1.0, 0.5    },
  { SA_MAEL_PARAM_OSC1_OCT,       A|MN, nullptr, "Oct",       "Osc1",    -4,    4,   0      },
  { SA_MAEL_PARAM_OSC1_SEMI,      A|MN, nullptr, "Semi",      "Osc1",    -12,   12,  0      },
  { SA_MAEL_PARAM_OSC1_CENT,      A|MN, nullptr, "Cent",      "Osc1",    -1.0,  1.0, 0      },

  { SA_MAEL_PARAM_OSC2_SQU,       A|MN, nullptr, "Squ",       "Osc2",     0.0,  1.0, 0.0    },
  { SA_MAEL_PARAM_OSC2_TRI,       A|MN, nullptr, "Tri",       "Osc2",     0.0,  1.0, 0.0    },
  { SA_MAEL_PARAM_OSC2_SIN,       A|MN, nullptr, "Sin",       "Osc2",     0.0,  1.0, 0.0    },
  { SA_MAEL_PARAM_OSC2_WIDTH,     A|MN, nullptr, "Width",     "Osc2",     0.0,  1.0, 0.5    },
  { SA_MAEL_PARAM_OSC2_OCT,       A|MN, nullptr, "Oct",       "Osc2",    -4,    4,   0      },
  { SA_MAEL_PARAM_OSC2_SEMI,      A|MN, nullptr, "Semi",      "Osc2",    -12,   12,  0      },
  { SA_MAEL_PARAM_OSC2_CENT,      A|MN, nullptr, "Cent",      "Osc2",    -1.0,  1.0, 0      },

  { SA_MAEL_PARAM_MIX_TYPE,       A|SE, nullptr, "Type",      "PSh",      0,    4,   0      },
  { SA_MAEL_PARAM_MIX_AMOUNT,     A|MN, nullptr, "Amount",    "PSh",      0,    1,   0      },

  { SA_MAEL_PARAM_FLT1_TYPE,      A|SE, nullptr, "Type",      "Flt1",     0,    9,   1      },
  { SA_MAEL_PARAM_FLT1_FREQ,      A|MN, nullptr, "Freq",      "Flt1",     0.0,  1.0, 0.75   },
  { SA_MAEL_PARAM_FLT1_Q,         A|MN, nullptr, "Q",         "Flt1",     0.0,  1.0, 0.2325 },
  { SA_MAEL_PARAM_ENV1_ATT,       A,    nullptr, "Att",       "Env1",     0.0,  1.0, 0.0    },
  { SA_MAEL_PARAM_ENV1_DEC,       A,    nullptr, "Dec",       "Env1",     0.0,  1.0, 0.0    },
  { SA_MAEL_PARAM_ENV1_SUS,       A,    nullptr, "Sus",       "Env1",     0.0,  1.0, 1.0    },
  { SA_MAEL_PARAM_ENV1_REL,       A,    nullptr, "Rel",       "Env1",     0.0,  1.0, 0.0    }
};

const uint32_t num_flt_types  = 10;
const char* flt_types_txt[num_flt_types]  = { "Off", "Lowpass", "Bandpass", "Highpass", "Notch", "Peak", "Allpass", "Bell", "Lowshelf", "Highshelf" };

const uint32_t num_mix_types  = 5;
const char* mix_types_txt[num_mix_types]  = { "Mix", "RingMod", "AmplMod", "FreqMod", "PhaseMod" };

//const uint32_t num_psh_types  = 2;
//const char* psh_types_txt[num_psh_types]  = { "Off", "Curve" };

//const uint32_t num_wsh_types  = 2;
//const char* wsh_types_txt[num_wsh_types]  = { "Off", "Curve" };

//----------

#undef A
#undef M
#undef MN
#undef S
#undef SE
#undef H

//----------------------------------------------------------------------

bool sa_mael_SetupParameters(SAT_Plugin* APlugin) {

  APlugin->appendParameter( new SAT_Parameter(    &sa_mael_Parameters[SA_MAEL_PARAM_GAIN]           ));

  APlugin->appendParameter( new SAT_Parameter(    &sa_mael_Parameters[SA_MAEL_PARAM_OSC1_SQU]       ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_mael_Parameters[SA_MAEL_PARAM_OSC1_TRI]       ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_mael_Parameters[SA_MAEL_PARAM_OSC1_SIN]       ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_mael_Parameters[SA_MAEL_PARAM_OSC1_WIDTH]     ));
  APlugin->appendParameter( new SAT_IntParameter( &sa_mael_Parameters[SA_MAEL_PARAM_OSC1_OCT]       ));
  APlugin->appendParameter( new SAT_IntParameter( &sa_mael_Parameters[SA_MAEL_PARAM_OSC1_SEMI]      ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_mael_Parameters[SA_MAEL_PARAM_OSC1_CENT]      ));

  APlugin->appendParameter( new SAT_Parameter(    &sa_mael_Parameters[SA_MAEL_PARAM_OSC2_SQU]       ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_mael_Parameters[SA_MAEL_PARAM_OSC2_TRI]       ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_mael_Parameters[SA_MAEL_PARAM_OSC2_SIN]       ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_mael_Parameters[SA_MAEL_PARAM_OSC2_WIDTH]     ));
  APlugin->appendParameter( new SAT_IntParameter( &sa_mael_Parameters[SA_MAEL_PARAM_OSC2_OCT]       ));
  APlugin->appendParameter( new SAT_IntParameter( &sa_mael_Parameters[SA_MAEL_PARAM_OSC2_SEMI]      ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_mael_Parameters[SA_MAEL_PARAM_OSC2_CENT]      ));

  APlugin->appendParameter( new SAT_TextParameter(&sa_mael_Parameters[SA_MAEL_PARAM_MIX_TYPE], mix_types_txt ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_mael_Parameters[SA_MAEL_PARAM_MIX_AMOUNT]     ));

  APlugin->appendParameter( new SAT_TextParameter(&sa_mael_Parameters[SA_MAEL_PARAM_FLT1_TYPE], flt_types_txt ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_mael_Parameters[SA_MAEL_PARAM_FLT1_FREQ]      ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_mael_Parameters[SA_MAEL_PARAM_FLT1_Q]         ));

  APlugin->appendParameter( new SAT_Parameter(    &sa_mael_Parameters[SA_MAEL_PARAM_ENV1_ATT]       ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_mael_Parameters[SA_MAEL_PARAM_ENV1_DEC]       ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_mael_Parameters[SA_MAEL_PARAM_ENV1_SUS]       ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_mael_Parameters[SA_MAEL_PARAM_ENV1_REL]       ));

  return true;
}

//----------------------------------------------------------------------
#endif
