#ifndef sa_mael_parameters_included
#define sa_mael_parameters_included
//----------------------------------------------------------------------

#include "sat.h"
#include "plugin/clap/sat_clap.h"

//----------------------------------------------------------------------

enum sa_mael_parameter_e {

  SA_MAEL_PARAM_GAIN = 0,

  SA_MAEL_PARAM_OSC1_TYPE,
  SA_MAEL_PARAM_OSC1_SHAPE,
  SA_MAEL_PARAM_OSC1_WIDTH,

  SA_MAEL_PARAM_FLT1_TYPE,
  SA_MAEL_PARAM_FLT1_FREQ,
  SA_MAEL_PARAM_FLT1_BW,

  SA_MAEL_PARAM_ENV1_ATT,
  SA_MAEL_PARAM_ENV1_DEC,
  SA_MAEL_PARAM_ENV1_SUS,
  SA_MAEL_PARAM_ENV1_REL,
  SA_MAEL_PARAM_ENV1_ATT_CURVE,
  SA_MAEL_PARAM_ENV1_DEC_CURVE,
  SA_MAEL_PARAM_ENV1_REL_CURVE,

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
  { SA_MAEL_PARAM_GAIN,           A,    nullptr, "Gain",      "Global",   0.0, 1.0, 0.5 },
  { SA_MAEL_PARAM_OSC1_TYPE,      A|SE, nullptr, "Type",      "Osc1",     0,   4,   1   },
  { SA_MAEL_PARAM_OSC1_SHAPE,     A|MN, nullptr, "Shape",     "Osc1",    -1.0, 1.0, 0.5 },
  { SA_MAEL_PARAM_OSC1_WIDTH,     A|MN, nullptr, "Width",     "Osc1",     0.0, 1.0, 0.5 },
  { SA_MAEL_PARAM_FLT1_TYPE,      A|SE, nullptr, "Type",      "Flt1",     0,   9,   1   },
  { SA_MAEL_PARAM_FLT1_FREQ,      A|MN, nullptr, "Freq",      "Flt1",     0.0, 1.0, 0.5 },
  { SA_MAEL_PARAM_FLT1_BW,        A|MN, nullptr, "BW",        "Flt1",     0.0, 1.0, 1.0 },
  { SA_MAEL_PARAM_ENV1_ATT,       A,    nullptr, "All",       "Env1",     0.0, 1.0, 0.0 },
  { SA_MAEL_PARAM_ENV1_DEC,       A,    nullptr, "Dec",       "Env1",     0.0, 1.0, 0.0 },
  { SA_MAEL_PARAM_ENV1_SUS,       A,    nullptr, "Sus",       "Env1",     0.0, 1.0, 1.0 },
  { SA_MAEL_PARAM_ENV1_REL,       A,    nullptr, "Rel",       "Env1",     0.0, 1.0, 0.0 },
  { SA_MAEL_PARAM_ENV1_ATT_CURVE, A,    nullptr, "AttCurve",  "Env1",     0.0, 1.0, 0.5 },
  { SA_MAEL_PARAM_ENV1_DEC_CURVE, A,    nullptr, "DecCurve",  "Env1",     0.0, 1.0, 0.5 },
  { SA_MAEL_PARAM_ENV1_REL_CURVE, A,    nullptr, "RelCurve",  "Env1",     0.0, 1.0, 0.5 }
};

const uint32_t num_osc_types = 5;

const char* osc_types_txt[num_osc_types] = {
  "Off", "Morph", "DSF", "Wavetable", "Waveguide"
};
  
const uint32_t num_flt_types = 10;

const char* flt_types_txt[num_flt_types] = {
  "Off", "Lowpass", "Bandpass", "Highpass", "Notch", "Peak", "Allpass", "Bell", "Lowshelf", "Highshelf"
};

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

  APlugin->appendParameter( new SAT_TextParameter(&sa_mael_Parameters[SA_MAEL_PARAM_OSC1_TYPE], osc_types_txt ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_mael_Parameters[SA_MAEL_PARAM_OSC1_SHAPE]     ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_mael_Parameters[SA_MAEL_PARAM_OSC1_WIDTH]     ));

  APlugin->appendParameter( new SAT_TextParameter(&sa_mael_Parameters[SA_MAEL_PARAM_FLT1_TYPE], flt_types_txt ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_mael_Parameters[SA_MAEL_PARAM_FLT1_FREQ]      ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_mael_Parameters[SA_MAEL_PARAM_FLT1_BW]        ));

  APlugin->appendParameter( new SAT_Parameter(    &sa_mael_Parameters[SA_MAEL_PARAM_ENV1_ATT]       ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_mael_Parameters[SA_MAEL_PARAM_ENV1_DEC]       ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_mael_Parameters[SA_MAEL_PARAM_ENV1_SUS]       ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_mael_Parameters[SA_MAEL_PARAM_ENV1_REL]       ));

  APlugin->appendParameter( new SAT_Parameter(    &sa_mael_Parameters[SA_MAEL_PARAM_ENV1_ATT_CURVE] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_mael_Parameters[SA_MAEL_PARAM_ENV1_DEC_CURVE] ));
  APlugin->appendParameter( new SAT_Parameter(    &sa_mael_Parameters[SA_MAEL_PARAM_ENV1_REL_CURVE] ));

  return true;
}

//----------------------------------------------------------------------
#endif
