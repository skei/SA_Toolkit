#ifndef sa_tyr_parameters_included
#define sa_tyr_parameters_included
//----------------------------------------------------------------------

#include "base/sat.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

enum sa_tyr_osc_type_enum {
  SA_TYR_OSC_TYPE_MORPH = 0,
  SA_TYR_OSC_TYPE_COUNT
};

const char* sa_tyr_osc_type_text[SA_TYR_OSC_TYPE_COUNT] = {
  "Morph"
};

enum sa_tyr_res_type_enum {
  SA_TYR_RES_TYPE_PLUCK = 0,
  SA_TYR_RES_TYPE_REP,
  SA_TYR_RES_TYPE_COUNT
};

const char* sa_tyr_res_type_text[SA_TYR_RES_TYPE_COUNT] = {
  "Pluck",
  "REP"
};

enum sa_tyr_flt_type_enum {
  SA_TYR_FLT_TYPE_OFF = 0,
  SA_TYR_FLT_TYPE_LOWPASS,
  SA_TYR_FLT_TYPE_HIGHPASS,
  SA_TYR_FLT_TYPE_BANDPASS,
  SA_TYR_FLT_TYPE_NOTCH,
  SA_TYR_FLT_TYPE_COUNT
};

const char* sa_tyr_flt_type_text[SA_TYR_FLT_TYPE_COUNT] = {
  "Off",
  "Lowpass",
  "Highpass",
  "Bandpass",
  "Notch"
};

enum sa_tyr_pm_type_enum {
  SA_TYR_PM_TYPE_OFF = 0,
  SA_TYR_PM_TYPE_CURVE,
  SA_TYR_PM_TYPE_SYNC,
  SA_TYR_PM_TYPE_SYNC_CLAMP,
  SA_TYR_PM_TYPE_PHASE_MOD,
  SA_TYR_PM_TYPE_FREQ_MOD,
  SA_TYR_PM_TYPE_DIFF,
  SA_TYR_PM_TYPE_INT,
  SA_TYR_PM_TYPE_COUNT
};

const char* sa_tyr_pm_type_text[SA_TYR_PM_TYPE_COUNT] = {
  "Off",
  "Curve",
  "Sync",
  "Sync Clamp",
  "Phase Mod",
  "Freq Mod",
  "Diff",
  "Int"
};

enum sa_tyr_wm_type_enum {
  SA_TYR_WM_TYPE_OFF = 0,
  SA_TYR_WM_TYPE_CURVE,
  SA_TYR_WM_TYPE_FOLD,
  SA_TYR_WM_TYPE_AMPL_MOD,
  SA_TYR_WM_TYPE_RING_MOD,
  SA_TYR_WM_TYPE_RAMP_DOWN,
  SA_TYR_WM_TYPE_DIFF,
  SA_TYR_WM_TYPE_INT,
  SA_TYR_WM_TYPE_COUNT
};

const char* sa_tyr_wm_type_text[SA_TYR_WM_TYPE_COUNT] = {
  "Off",
  //"Const",
  "Curve",
  "Fold",
  "Ampl Mod",
  "Ring Mod",
  "Ramp Down",
  "Diff",
  "Int"
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------



/*
  todo: we can test the param_id, and the index in MParameters when
  we append the parameters, if they differ, the ordering is wrong..
  any better way, so we don't need those enums?

*/

enum sa_tyr_eparameter {
  PAR_MASTER_VOL = 0,
  PAR_MASTER_PAN ,
  PAR_MASTER_OSC1_OUT,
  PAR_MASTER_OSC2_OUT,
  PAR_MASTER_RES1_OUT,
  PAR_MASTER_RES2_OUT,

//  PAR_OSC_MIX_TYPE,
//  PAR_OSC_MIX_AMOUNT,

// osc 1

  PAR_OSC1_IN_O1,
  PAR_OSC1_IN_O2,
  PAR_OSC1_IN_R1,
  PAR_OSC1_IN_R2,
  PAR_OSC1_IN_N,
  PAR_OSC1_IN_S,
  PAR_OSC1_IN_I,
  PAR_OSC1_IN_A,

  PAR_OSC1_TYPE,
  PAR_OSC1_SHAPE,
  PAR_OSC1_WIDTH,

  PAR_OSC1_PM_TYPE,
  PAR_OSC1_PM_AMOUNT,
  PAR_OSC1_WM_TYPE,
  PAR_OSC1_WM_AMOUNT,

  PAR_OSC1_OCT,
  PAR_OSC1_SEMI,
  PAR_OSC1_CENT,

// osc 2

  PAR_OSC2_IN_O1,
  PAR_OSC2_IN_O2,
  PAR_OSC2_IN_R1,
  PAR_OSC2_IN_R2,
  PAR_OSC2_IN_N,
  PAR_OSC2_IN_S,
  PAR_OSC2_IN_I,
  PAR_OSC2_IN_A,

  PAR_OSC2_TYPE,
  PAR_OSC2_SHAPE,
  PAR_OSC2_WIDTH,

  PAR_OSC2_PM_TYPE,
  PAR_OSC2_PM_AMOUNT,
  PAR_OSC2_WM_TYPE,
  PAR_OSC2_WM_AMOUNT,

  PAR_OSC2_OCT,
  PAR_OSC2_SEMI,
  PAR_OSC2_CENT,

// res 1

  PAR_RES1_IN_O1,
  PAR_RES1_IN_O2,
  PAR_RES1_IN_R1,
  PAR_RES1_IN_R2,
  PAR_RES1_IN_N,
  PAR_RES1_IN_S,
  PAR_RES1_IN_I,
  PAR_RES1_IN_A,

  PAR_RES1_TYPE,
  PAR_RES1_SPEED,
  PAR_RES1_SHAPE,
  PAR_RES1_FB,
  PAR_RES1_DAMP,
  PAR_RES1_ROUGH,
  PAR_RES1_OCT,
  PAR_RES1_SEMI,
  PAR_RES1_CENT,

// res 2

  PAR_RES2_IN_O1,
  PAR_RES2_IN_O2,
  PAR_RES2_IN_R1,
  PAR_RES2_IN_R2,
  PAR_RES2_IN_N,
  PAR_RES2_IN_S,
  PAR_RES2_IN_I,
  PAR_RES2_IN_A,

  PAR_RES2_TYPE,
  PAR_RES2_SPEED,
  PAR_RES2_SHAPE,
  PAR_RES2_FB,
  PAR_RES2_DAMP,
  PAR_RES2_ROUGH,
  PAR_RES2_OCT,
  PAR_RES2_SEMI,
  PAR_RES2_CENT,

// flt1

  PAR_FLT1_TYPE,
  PAR_FLT1_FREQ,
  PAR_FLT1_RES,

// env1

  PAR_ENV1_ATT,
  PAR_ENV1_DEC,
  PAR_ENV1_SUS,
  PAR_ENV1_REL,

//----------

  PARAM_COUNT
};

//----------------------------------------------------------------------

clap_param_info_t sa_tyr_parameters[PARAM_COUNT] = {

  //---------- global ----------

  { PAR_MASTER_VOL,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE,
    nullptr,
    "Vol",
    "Master",
    0.0,
    1.0,
    0.5
  },
  { PAR_MASTER_PAN,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE,
    nullptr,
    "Pan",
    "Master",
    0.0,
    1.0,
    0.5
  },

  { PAR_MASTER_OSC1_OUT,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "O1",
    "Master",
    0.0,
    1.0,
    0.5
  },
  { PAR_MASTER_OSC2_OUT,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "O2",
    "Master",
    0.0,
    1.0,
    0.0
  },
  { PAR_MASTER_RES1_OUT,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "R1",
    "Master",
    0.0,
    1.0,
    0.0
  },
  { PAR_MASTER_RES2_OUT,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "R2",
    "Master",
    0.0,
    1.0,
    0.0
  },

  //---------- osc1 ----------

  // inputs

  { PAR_OSC1_IN_O1,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "O1",
    "Osc1/In",
    0.0,
    1.0,
    0.0
  },
  { PAR_OSC1_IN_O2,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "O2",
    "Osc1/In",
    0.0,
    1.0,
    0.0
  },
  { PAR_OSC1_IN_R1,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "R1",
    "Osc1/In",
    0.0,
    1.0,
    0.0
  },
  { PAR_OSC1_IN_R2,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "R2",
    "Osc1/In",
    0.0,
    1.0,
    0.0
  },
  { PAR_OSC1_IN_N,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "N",
    "Osc1/In",
    0.0,
    1.0,
    0.0
  },
  { PAR_OSC1_IN_S,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "S",
    "Osc1/In",
    0.0,
    1.0,
    0.0
  },
  { PAR_OSC1_IN_I,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "I",
    "Osc1/In",
    0.0,
    1.0,
    0.0
  },
  { PAR_OSC1_IN_A,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "A",
    "Osc1/In",
    0.0,
    1.0,
    0.0
  },

  //

  { PAR_OSC1_TYPE,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_STEPPED
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Type",
    "Osc1/Osc",
    0.0,
    SA_TYR_OSC_TYPE_COUNT - 1,//2.0,
    0.0
  },
  { PAR_OSC1_SHAPE,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Shape",
    "Osc1/Osc",
    0.0,
    1.0,
    0.0
  },
  { PAR_OSC1_WIDTH,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Width",
    "Osc1/Osc",
    0.0,
    1.0,
    0.5
  },

  // pm, wm

  { PAR_OSC1_PM_TYPE,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_STEPPED
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Phase",
    "Osc1/PM",
    0,
    SA_TYR_PM_TYPE_COUNT - 1,//6,
    0
  },
  { PAR_OSC1_PM_AMOUNT,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Phase",
    "Osc1/PM",
    0.0,
    1.0,
    0.0
  },

  { PAR_OSC1_WM_TYPE,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_STEPPED
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Wave",
    "Osc1/WM",
    0,
    SA_TYR_WM_TYPE_COUNT - 1,//8,
    0
  },
  { PAR_OSC1_WM_AMOUNT,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Wave",
    "Osc1/WM",
    0.0,
    1.0,
    0.0
  },

  { PAR_OSC1_OCT,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_STEPPED
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Oct",
    "Osc1/Tuning",
   -4,
    4,
    0
  },
  { PAR_OSC1_SEMI,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_STEPPED
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Semi",
    "Osc1/Tuning",
   -12,
    12,
    0
  },
  { PAR_OSC1_CENT,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Cent",
    "Osc1/Tuning",
   -1.0,
    1.0,
    0.
  },

  //---------- osc2 ----------

  // inputs

  { PAR_OSC2_IN_O1,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "O1",
    "",
    0.0,
    1.0,
    0.0
  },
  { PAR_OSC2_IN_O2,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "O2",
    "",
    0.0,
    1.0,
    0.0
  },
  { PAR_OSC2_IN_R1,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "R1",
    "",
    0.0,
    1.0,
    0.0
  },
  { PAR_OSC2_IN_R2,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "R2",
    "",
    0.0,
    1.0,
    0.0
  },
  { PAR_OSC2_IN_N,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "N",
    "",
    0.0,
    1.0,
    0.0
  },
  { PAR_OSC2_IN_S,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "S",
    "",
    0.0,
    1.0,
    0.0
  },
  { PAR_OSC2_IN_I,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "I",
    "",
    0.0,
    1.0,
    0.0
  },
  { PAR_OSC2_IN_A,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "A",
    "",
    0.0,
    1.0,
    0.0
  },

  //

  { PAR_OSC2_TYPE,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_STEPPED
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Type",
    "",
    0.0,
    SA_TYR_OSC_TYPE_COUNT - 1,//2.0,
    0.0
  },
  { PAR_OSC2_SHAPE,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Shape",
    "",
    0.0,
    1.0,
    0.0
  },
  { PAR_OSC2_WIDTH,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Width",
    "",
    0.0,
    1.0,
    0.5
  },

  // pm, wm

  { PAR_OSC2_PM_TYPE,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_STEPPED
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Phase",
    "",
    0,
    SA_TYR_PM_TYPE_COUNT - 1,//6,
    0
  },
  { PAR_OSC2_PM_AMOUNT,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Phase",
    "",
    0.0,
    1.0,
    0.0
  },

  { PAR_OSC2_WM_TYPE,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_STEPPED
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Wave",
    "",
    0,
    SA_TYR_WM_TYPE_COUNT - 1,//8,
    0
  },
  { PAR_OSC2_WM_AMOUNT,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Wave",
    "",
    0.0,
    1.0,
    0.0
  },

  { PAR_OSC2_OCT,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_STEPPED
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Oct",
    "",
   -4,
    4,
    0
  },
  { PAR_OSC2_SEMI,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_STEPPED
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Semi",
    "",
   -12,
    12,
    0
  },
  { PAR_OSC2_CENT,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Cent",
    "",
   -1.0,
    1.0,
    0.
  },

  //---------- res1 ----------

  // inputs

  { PAR_RES1_IN_O1,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "O1",
    "",
    0.0,
    1.0,
    0.0
  },
  { PAR_RES1_IN_O2,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "O2",
    "",
    0.0,
    1.0,
    0.0
  },
  { PAR_RES1_IN_R1,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "R1",
    "",
    0.0,
    1.0,
    0.0
  },
  { PAR_RES1_IN_R2,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "R2",
    "",
    0.0,
    1.0,
    0.0
  },
  { PAR_RES1_IN_N,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "N",
    "",
    0.0,
    1.0,
    1.0
  },
  { PAR_RES1_IN_S,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "S",
    "",
    0.0,
    1.0,
    0.0
  },
  { PAR_RES1_IN_I,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "I",
    "",
    0.0,
    1.0,
    0.0
  },
  { PAR_RES1_IN_A,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "A",
    "",
    0.0,
    1.0,
    0.0
  },

  //

  { PAR_RES1_TYPE,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_STEPPED
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Type",
    "",
    0.0,
    SA_TYR_RES_TYPE_COUNT - 1,//2.0,
    0.0
  },
  { PAR_RES1_SPEED,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Speed",
    "",
    0.0,
    1.0,
    0.0
  },
  { PAR_RES1_SHAPE,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Shape",
    "",
    0.0,
    1.0,
    0.0
  },
  { PAR_RES1_FB,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "FB",
    "",
    0.0,
    1.0,
    0.9
  },
  { PAR_RES1_DAMP,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Damp",
    "",
    0.0,
    1.0,
    0.5
  },

  { PAR_RES1_ROUGH,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Rough",
    "",
    0.0,
    1.0,
    0.0
  },

  { PAR_RES1_OCT,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_STEPPED
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Oct",
    "",
   -4,
    4,
    0
  },
  { PAR_RES1_SEMI,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_STEPPED
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Semi",
    "",
   -12,
    12,
    0
  },
  { PAR_RES1_CENT,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Cent",
    "",
   -1.0,
    1.0,
    0.0
  },

  //---------- res2 ----------

  // inputs

  { PAR_RES2_IN_O1,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "O1",
    "",
    0.0,
    1.0,
    0.0
  },
  { PAR_RES2_IN_O2,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "O2",
    "",
    0.0,
    1.0,
    0.0
  },
  { PAR_RES2_IN_R1,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "R1",
    "",
    0.0,
    1.0,
    0.0
  },
  { PAR_RES2_IN_R2,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "R2",
    "",
    0.0,
    1.0,
    0.0
  },
  { PAR_RES2_IN_N,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "N",
    "",
    0.0,
    1.0,
    1.0
  },
  { PAR_RES2_IN_S,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "S",
    "",
    0.0,
    1.0,
    0.0
  },
  { PAR_RES2_IN_I,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "I",
    "",
    0.0,
    1.0,
    0.0
  },
  { PAR_RES2_IN_A,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "A",
    "",
    0.0,
    1.0,
    0.0
  },

  //

  { PAR_RES2_TYPE,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_STEPPED
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Type",
    "",
    0.0,
    SA_TYR_RES_TYPE_COUNT - 1,//2.0,
    0.0
  },
  { PAR_RES2_SPEED,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Speed",
    "",
    0.0,
    1.0,
    0.0
  },
  { PAR_RES2_SHAPE,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Shape",
    "",
    0.0,
    1.0,
    0.0
  },
  { PAR_RES2_FB,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "FB",
    "",
    0.0,
    1.0,
    0.9
  },
  { PAR_RES2_DAMP,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Damp",
    "",
    0.0,
    1.0,
    0.5
  },

  { PAR_RES2_ROUGH,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Rough",
    "",
    0.0,
    1.0,
    0.0
  },

  { PAR_RES2_OCT,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_STEPPED
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Oct",
    "",
   -4,
    4,
    0
  },
  { PAR_RES2_SEMI,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_STEPPED
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,

    nullptr,
    "Semi",
    "",
   -12,
    12,
    0
  },
  { PAR_RES2_CENT,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Cent",
    "",
   -1.0,
    1.0,
    0.0
  },

  //---------- flt1 ----------

  { PAR_FLT1_TYPE,
    CLAP_PARAM_IS_AUTOMATABLE,
    nullptr,
    "Type",
    "",
    0.0,
    SA_TYR_FLT_TYPE_COUNT - 1,//4.0,
    1.0
  },

  { PAR_FLT1_FREQ,
    CLAP_PARAM_IS_AUTOMATABLE
    | CLAP_PARAM_IS_MODULATABLE
    | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Freq",
    "",
    0.0,
    1.0,
    1.0
  },

  { PAR_FLT1_RES,
    CLAP_PARAM_IS_AUTOMATABLE
    | CLAP_PARAM_IS_MODULATABLE
    | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Res",
    "",
    0.0,
    1.0,
    0.0
  },

  //---------- env1 ----------

  { PAR_ENV1_ATT,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Att",
    "",
    0.0,
    1.0,
    0.05
  },

  { PAR_ENV1_DEC,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Dec",
    "",
    0.0,
    1.0,
    0.0
  },

  { PAR_ENV1_SUS,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Sus",
    "",
    0.0,
    1.0,
    1.0
  },

  { PAR_ENV1_REL,
    CLAP_PARAM_IS_AUTOMATABLE
      | CLAP_PARAM_IS_MODULATABLE
      | CLAP_PARAM_IS_MODULATABLE_PER_NOTE_ID,
    nullptr,
    "Rel",
    "",
    0.0,
    1.0,
    0.5
  }

};

//----------------------------------------------------------------------
#endif
